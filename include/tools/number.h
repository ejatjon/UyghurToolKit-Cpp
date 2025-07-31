//
// Created by EjatjonAmar on 2025/6/29.
//

#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include "unicode_string_convert.h"
#include "resources/dictionaries/numbers.h"

const int UNITS_ORDER[]={
        1000000000,1000000, 1000, 100, 10, 1
};

/**
 * 分解数字到维语数量单位
 * 示例：314 → 300 + 10 + 4 → [(100,3),(10,1),(1,4)]
 * @param num 要处理的数字
 * @return 处理结果
 */
CORE_API std::vector<std::tuple<int,int>> decompose_number(int num);


/**
 * 返回数字有效的小数位数和有效的整数
 * 示例：3.012300 -> tuple(4,123)
 * @param num 要处理的数字的字符串
 * @return 处理结果
 */
CORE_API std::tuple<int,int> decimal_places_and_integer(const std::u32string& num);


/**
 * 将整数字符串转换为文字表示
 * @param num 要处理的数字字符串
 * @return 处理结果
 */
CORE_API std::u32string integer_number_to_string(std::u32string num, int letter_type);

/**
 * 将浮点数字符串转换为文字表示
 * @param num 要处理的数字的字符串
 * @param p_class 源语言类型
 * @return 处理结果
 */
CORE_API std::u32string decimal_number_to_string(std::u32string num,int letter_type);


/**
 * 整数数字处理，将文本中阿拉伯数字转换为文字表示
 * @param text 要处理的数字的文字表示
 * @param letter_type 源语言类型
 * @return 处理结果
 */
CORE_API std::u32string process_integer_number(std::u32string text,int letter_type,
                                      std::vector<std::u32string>* before_assertion= nullptr,std::vector<std::u32string>* after_assertion= nullptr);

/**
 * 浮点数字处理，将文本中阿拉伯数字转换为文字表示
 * @param text 要处理的数字的文字表示
 * @param letter_type 源语言类型
 * @return 处理结果
 */
CORE_API std::u32string process_decimal_number(std::u32string text,int letter_type,std::vector<std::u32string>* before_assertion = nullptr,std::vector<std::u32string>* after_assertion= nullptr);


/**
 * 除法
 * @param divisor 除数
 * @param dividend 被除数
 * @return 处理结果
 */
CORE_API double divide(int dividend,int divisor);


/**
吧Uyghur 数字转换成阿拉伯数字会有点复杂，以下是详细的处理逻辑(注意 如果出现 以空格分割的数将会出现问题 如 ‘1234 23456’)
1. 吧Uyghur数字转换成 (数字（int），不是数量级(bool)) 格式的列表，下面称为pending（暂存数字）
例如：
بىر يۈز يىگىرمە ئۈچ مىڭ توققۇز يۈز ئەللىك بەش(123955)
-> [(1, True), (100, False), (20, True), (3, True), (1000, False), (9, True), (100, False), (50, True), (5, True)]
2. pending分成几个数量级单调递增的部分
这里说的数量级说的是为维语中表示数量级的数(维语中的万不常用因此没包含在里面) 如 百 100 千1000 百万100000 ...
例如：[(1, True), (100, False), (20, True), (3, True), (1000, False), (9, True), (100, False), (50, True), (5, True)]
-> [
    [(1, True), (100, False), (20, True), (3, True), (1000, False)],
    [(9, True), (100, False)], [(50, True)],
    [(5, True)]
   ]
每个部分的数量级都是单调递增的 (100 < 1000 )
3. 然后吧每个单调递增的数字列表整合成 阿拉伯数
整合的规则是： p/c -> p: 加法
            p/c -> c: 乘法
其中 p 表示 非数量级数  c 表示 数量级数   / 表示 或   (这个规则是我自己观察出来的，经过测试没发现问题)
例如：[(1, True), (100, False), (20, True), (3, True), (1000, False)]
第一步 1 和 100  1 是 p ，100 是 c 。通过规则 p->c 为乘法 结果为c 1*100=100
第二步 100（第一步结果，类型为c） 和 20 ，通过规则c->p 为加法 结果为p 20+100=120
第三步 120（第二步结果，类型为p） 和 3  ，通过规则p->p 为加法 结果为p 120+3=123
第四步 123（第三步结果，类型为p） 和 1000  ，通过规则p->c 为乘法 结果为c 123*1000=123000
返回123000
4. 吧各部分整合的结果求和
例如：sum([123000,950,5])=123955
*/

namespace {
/**
 * text_list 中的元素必须在reverse_number_map中存在,不然会出错。不应该自己使用这个函数
 */
    int reverse_integer_number(const std::vector<std::u32string>& text_list, int letter_type);
}


/**
 * 吧文本表示的数字转换为阿拉伯数字
 * @param text 要处理的文本
 * @param letter_type 源语言类型
 * @return
 */
CORE_API std::u32string process_reverse_decimal_number(const std::u32string& text,int letter_type);


CORE_API std::u32string process_reverse_integer_number(const std::u32string& text, int letter_type);


