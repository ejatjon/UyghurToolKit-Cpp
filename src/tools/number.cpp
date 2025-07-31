//
// Created by EjatjonAmar on 2025/6/29.
//
#include <functional>
#include <set>
#include "tools/number.h"
#include "tools/text.h"


std::vector<std::tuple<int, int>> decompose_number(int num) {
    std::vector<std::tuple<int,int>> result;
    for(int unit:UNITS_ORDER){
        if (num >= unit){
            int count= num / unit;
            result.emplace_back(unit, count);
            num -= count * unit;
            if (num == 0)
                break;
        }
    }
    return result;
}

std::tuple<int, int> decimal_places_and_integer(const std::u32string& num) {

    size_t dotPos = num.find('.');
    if (dotPos == std::string::npos) {
        return {0, 0}; // 没有小数点，整数
    }

    // 找到小数点后的非零尾部
    std::u32string decimalPart = num.substr(dotPos + 1);
    size_t nonZeroEnd = decimalPart.find_last_not_of('0');
    if (nonZeroEnd == std::string::npos) {
        return {0, 0}; // 全为0
    }
    size_t nonZeroStart = decimalPart.find_first_not_of('0');
    return {nonZeroEnd + 1, std::stoi(
            utf32_to_utf8(
                    decimalPart.substr(nonZeroStart, nonZeroEnd - nonZeroStart + 1)
            )
    )};
}

std::u32string integer_number_to_string(std::u32string num, int letter_type) {
    if (letter_type <1 || letter_type >5)
        return num;

    int int_number = std::stoi(utf32_to_utf8(num));

    std::map<int,std::u32string> number_map= getNumberMapWithUtf32(letter_type);


    if (number_map.find(int_number)!=number_map.end()) {
        return number_map.at(int_number) + U" ";
    }
    auto components = decompose_number(int_number);
    std::u32string result;

    for (const auto& [unit, count] : components) {
        if (unit == 1) {
            result += number_map.at(count) + U" ";
        } else if (unit == 10) {
            result += number_map.at(unit * count) + U" ";
        } else {
            std::u32string count_str = integer_number_to_string(utf8_to_utf32(std::to_string(count)), letter_type);
            result += count_str + number_map.at( unit) + U" ";
        }
    }
    return result;
}

std::u32string decimal_number_to_string(std::u32string num, int letter_type) {
    if (letter_type <1 || letter_type >5)
        return num;

    std::u32string integer_part = integer_number_to_string(num.substr(0, num.find('.')), letter_type);
    auto [decimal_places, decimal_part_int] = decimal_places_and_integer(num);
    if (decimal_part_int == 0)
        return integer_part;
    std::u32string decimal_part = integer_number_to_string(utf8_to_utf32(std::to_string(decimal_part_int)), letter_type);
    std::u32string decimal_places_unit_part = integer_number_to_string(utf8_to_utf32(std::to_string(std::pow(10,decimal_places))), letter_type);
    auto [p,d] = decimal_adverbs[letter_type-1];
    return integer_part + p + U" " + decimal_places_unit_part + U" " + d + U" " +decimal_part;
}

CORE_API std::u32string process_integer_number(std::u32string text, int letter_type,
                                                std::vector<std::u32string>* before_assertion,
                                                std::vector<std::u32string>* after_assertion) {
    if (letter_type < 1 || letter_type > 5)
        return text;

    // 构建断言字符集合，用于快速查找
    std::set<char32_t> before_chars;
    std::set<char32_t> after_chars;

    // 填充前置断言字符集合
    if (before_assertion) {
        for (const auto& str : *before_assertion) {
            if (!str.empty()) {
                before_chars.insert(str[0]);
            }
        }
    }

    // 填充后置断言字符集合
    if (after_assertion) {
        for (const auto& str : *after_assertion) {
            if (!str.empty()) {
                after_chars.insert(str[0]);
            }
        }
    }

    std::u32string result{};
    size_t i = 0;

    while (i < text.length()) {
        // 如果当前字符不是数字，直接添加到结果中
        if (text[i] < U'0' || text[i] > U'9') {
            result += text[i];
            i++;
            continue;
        }

        // 记录数字开始位置
        size_t num_start = i;

        // 提取完整的数字序列
        while (i < text.length() && text[i] >= U'0' && text[i] <= U'9') {
            i++;
        }

        // 检查前置条件
        bool before_valid = true;
        if (num_start > 0) {
            char32_t prev_char = text[num_start - 1];
            // 如果前一个字符在前置断言集合中，则不匹配
            if (!before_chars.empty() && before_chars.find(prev_char) != before_chars.end()) {
                before_valid = false;
            }
        }

        // 检查后置条件
        bool after_valid = true;
        if (i < text.length()) {
            char32_t next_char = text[i];
            // 如果后一个字符在后置断言集合中，则不匹配
            if (!after_chars.empty() && after_chars.find(next_char) != after_chars.end()) {
                after_valid = false;
            }
        }

        // 如果前后条件都满足，则进行数字转换
        if (before_valid && after_valid) {
            std::u32string num_str = text.substr(num_start, i - num_start);
            result += result.ends_with(U' ')?integer_number_to_string(num_str, letter_type):U' ' + integer_number_to_string(num_str, letter_type);
        } else {
            // 否则保持原数字不变
            result += text.substr(num_start, i - num_start);
        }
    }

    return result;
}


CORE_API std::u32string process_decimal_number(std::u32string text, int letter_type,
                                                std::vector<std::u32string>* before_assertion,
                                                std::vector<std::u32string>* after_assertion) {
    if (letter_type < 1 || letter_type > 5)
        return text;

    // 构建断言字符集合，用于快速查找
    std::set<char32_t> before_chars;
    std::set<char32_t> after_chars;

    // 填充前置断言字符集合
    if (before_assertion) {
        for (const auto& str : *before_assertion) {
            if (!str.empty()) {
                before_chars.insert(str[0]);
            }
        }
    }

    // 填充后置断言字符集合
    if (after_assertion) {
        for (const auto& str : *after_assertion) {
            if (!str.empty()) {
                after_chars.insert(str[0]);
            }
        }
    }

    std::u32string result{};
    size_t i = 0;

    while (i < text.length()) {
        // 如果当前字符不是数字，直接添加到结果中
        if (text[i] < U'0' || text[i] > U'9') {
            result += text[i];
            i++;
            continue;
        }

        // 记录数字开始位置
        size_t num_start = i;

        // 提取第一个数字序列
        while (i < text.length() && text[i] >= U'0' && text[i] <= U'9') {
            i++;
        }

        // 检查是否后跟小数点
        if (i >= text.length() || text[i] != U'.') {
            // 不是小数
            result += text.substr(num_start, i - num_start);
            continue;
        }
        else {
            // 跳过小数点
            i++;
            // 获取小数部分的开始位置
            size_t decimal_start = i;

            // 提取第二个数字序列（小数部分）
            while (i < text.length() && text[i] >= U'0' && text[i] <= U'9') {
                i++;
            }

            // 检查是否是合法的小数（小数部分至少有一位数字）
            if (i == decimal_start) {
                // 小数部分没有数字，不是合法的小数
                result += text.substr(num_start, i - num_start);
                continue;
            }

            // 检查前置条件（检查整数部分前面的字符）
            bool before_valid = true;
            if (num_start > 0) {
                char32_t prev_char = text[num_start - 1];
                // 如果前一个字符在前置断言集合中，则不匹配
                if (!before_chars.empty() && before_chars.find(prev_char) != before_chars.end()) {
                    before_valid = false;
                }
            }

            // 检查后置条件（检查小数部分后面的字符）
            bool after_valid = true;
            if (i < text.length()) {
                char32_t next_char = text[i];
                // 如果后一个字符在后置断言集合中，则不匹配
                if (!after_chars.empty() && after_chars.find(next_char) != after_chars.end()) {
                    after_valid = false;
                }
            }

            // 如果前后条件都满足，则进行数字转换
            if (before_valid && after_valid) {
                std::u32string num_str = text.substr(num_start, i - num_start);


                result += result.ends_with(U' ') ?
                          decimal_number_to_string(num_str, letter_type) :
                          U' ' + decimal_number_to_string(num_str, letter_type);
            } else {
                // 否则保持原数字不变
                result += text.substr(num_start, i - num_start);
            }
        }
    }

    return result;
}



// TODO:精度问题没有被解决
CORE_API double divide(int dividend,int divisor){
    return static_cast<double>(dividend) / divisor;
}

namespace {
    int reverse_integer_number(const std::vector<std::u32string>& text_list, int letter_type) {
        std::vector<int> pending_integer_numbers;
        std::vector<int> pending;
        int max_order = 0;
        int number = 0;
        int result = 0;
        auto reverse_number_map = getReverseNumberMapWithUtf32(letter_type);
        std::function<int(void)> evaluate_subarray = [&]() -> int {
            int current = 0;
            for (auto i: pending) {
                if (current == 0) { //第一个元素为current的初始值
                    current = i;
                    continue;
                }
                if (i < 100) {
                    current += i;
                } else {
                    current *= i;
                }
            }
            pending.clear();
            return current;
        };
        for (const auto& word: text_list) {
            number = reverse_number_map[word];
            if (number<100) {
                pending_integer_numbers.push_back(number);
                continue;
            }else if (number > max_order) {
                max_order = number;
                for (auto i: pending_integer_numbers)
                    pending.push_back(i);
                pending_integer_numbers.clear();
                pending.push_back( number);
                continue;
            }else{
                result += evaluate_subarray();
                for(auto i: pending_integer_numbers)
                    pending.push_back(i);
                pending_integer_numbers.clear();
                pending.push_back(number);
                max_order=number;
            }
        }
        if (!pending.empty()){
            for(auto i: pending_integer_numbers)
                pending.push_back(i);
            result += evaluate_subarray();
        }
        return result;
    }
}

CORE_API std::u32string process_reverse_decimal_number(const std::u32string& text, int letter_type) {
    if (letter_type <1 || letter_type >5)
        return text;
    auto [prefix_str,suffix_str] = decimal_adverbs[letter_type-1];
    std::vector<std::u32string> result;
    std::vector<std::u32string> pending_integer{};
    std::vector<std::u32string> pending_decimal{};
    std::vector<std::u32string> pending_order{}; // 量级
    bool prefix_pending{false},suffix_pending{false};
    // 将待处理数据重新添加到结果中
    std::function<void()> pending_add_to_result = [&]() -> void {
        std::function<void(std::vector<std::u32string>)> add_to_result = [&](const std::vector<std::u32string>& text_list) -> void {
            for(const auto& word: text_list){
                result.push_back(word);
            }
        };
        add_to_result(pending_integer);
        if (prefix_pending)
            result.push_back(prefix_str);
        add_to_result(pending_order);
        if (suffix_pending) // 不触发
            result.push_back(suffix_str);
        add_to_result(pending_decimal);
    };
    // 过滤掉非浮点数，吧浮点数还原
    std::function<void()> reverse_number_or_add_to_result = [&]() -> void {
        if (prefix_pending && suffix_pending){ // 如果浮点数的整数和小数部分都存在则才认为是一个浮点数
            int integer_num = reverse_integer_number(pending_integer,letter_type);
            int decimal_num = reverse_integer_number(pending_decimal,letter_type);
            int order_num = reverse_integer_number(pending_order,letter_type);
            result.push_back(utf8_to_utf32(std::to_string(integer_num+divide(decimal_num,order_num))));
        }else{ // 非浮点数不用还原
            pending_add_to_result();
        }
        // 清理和初始化
        pending_integer.clear();
        pending_decimal.clear();
        pending_order.clear();
        prefix_pending = false;
        suffix_pending = false;
    };


    auto reverse_number_map = getReverseNumberMapWithUtf32(letter_type);
    for(const auto& i:split_string<std::u32string>(text, U" ")){

        if(reverse_number_map.contains(i)){
            if(!prefix_pending)
                pending_integer.push_back(i);
            else if(prefix_pending && !suffix_pending)
                pending_order.push_back(i);
            else
                pending_decimal.push_back(i);
            continue;
        } else if((i == prefix_str) && !prefix_pending){
            prefix_pending = true;
            continue;
        } else if((i == suffix_str) && prefix_pending){
            suffix_pending = true;
            continue;
        }

        reverse_number_or_add_to_result();
        result.push_back(i);
    }
    reverse_number_or_add_to_result();
    return join_strings<std::u32string>(result, U" ");
}

CORE_API std::u32string process_reverse_integer_number(const std::u32string& text, int letter_type) {
    if (letter_type <1 || letter_type >5)
        return text;
    std::vector<std::u32string> result{};
    std::vector<std::u32string> pending_integer{};

    auto reverse_number_map = getReverseNumberMapWithUtf32(letter_type);
    for(const auto& word: split_string<std::u32string>(text, U" ")){
        if(reverse_number_map.contains(word)){
            pending_integer.push_back(word);
            continue;
        }

        if(!pending_integer.empty()){
            result.push_back(utf8_to_utf32(std::to_string(reverse_integer_number(pending_integer,letter_type))));
            pending_integer.clear();
        }
        result.push_back(word);
    }
    if(!pending_integer.empty()){
        result.push_back(utf8_to_utf32(std::to_string(reverse_integer_number(pending_integer,letter_type))));
    }
    return join_strings<std::u32string>(result, U" ");
}


