//
// Created by EjatjonAmar on 2025/7/3.
//

#pragma once

#include <string>
#include <functional>
#include <regex>
#include "core.h"

struct Date{
    std::string year{};
    std::string month{};
    std::string day{};
public:
    bool is_empty(){
        return year.empty() && month.empty() && day.empty();
    }
};

struct Time{
    std::string hour{};
    std::string minute{};
    std::string second{};
    std::string millisecond{};
public:
    bool is_empty(){
        return hour.empty() && minute.empty() && second.empty() && millisecond.empty();
    }
};


/**
 * 日期正则信息。
 * @param regex 正则
 * @param handler 处理函数,handler 会收到匹配的smatch，它负责处理smatch并返回包含 Date和Time 的元组。当Date和Time结构体中的某个字段为-1时，表示该字段不存在。
 */
struct DateRegexInfo{
    std::string regex;
    std::function<std::tuple<Date,Time>(const std::sub_match<std::string::const_iterator>&)> handler; // 处理函数
    std::function<std::string(std::tuple<Date,Time>,int)> to_string;

};

CORE_API std::vector<DateRegexInfo> get_default_date_regex_info();

/**
 * 创建日期正则
 * @param info 日期正则信息的列表
 * @return 正则表达式
 */
CORE_API std::regex mk_date_regex(const std::vector<DateRegexInfo> &info);


/**
 * 将日期数字转换为文本且添加后缀
 * @param text 处理的文本
 * @param letter_type 源文本类型
 * @return 处理结果
 */
CORE_API std::string default_date_time_to_string(Date date,Time time,int letter_type);

/**
 * 处理日期;将数字表示的日期转换为文字表示
 * @param text 处理的文本
 * @param info 日期正则信息
 * @param letter_type 源文本类型
 * @return 处理结果
 */
CORE_API std::string process_date(std::string text,const std::vector<DateRegexInfo> &info,int letter_type);









