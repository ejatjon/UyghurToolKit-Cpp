//
// Created by EjatjonAmar on 2025/6/29.
//

#pragma once

#include <regex>
#include <map>
#include <unordered_set>


#include "processing/processor/processor.h"
#include "resources/dictionaries/vowels.h"
#include "unicode_string_convert.h"
#include "resources/dictionaries/numbers.h"
#include "resources/dictionaries/symbols.h"

/**
 * 判断一个字符是否是元音
 * @param c 字符
 * @param letter_type 字符类型
 * @return
 */
CORE_API bool is_vowel(const char32_t* c, int letter_type);
CORE_API bool is_vowel(const std::u32string& letter,int letter_type);

/**
 * 替换数字前的中划线（-），text是一个维语表示的数，此函数会给他添加 inqi 后缀
 * @param s: 是一个维语表示的数
 * @param letter_type 字符类型
 * @return: 替换后的字符串
*/
CORE_API std::u32string number_hyphen_converter(std::u32string text, int letter_type);

/**
 * 将字符inqi 转换为中划线
 * @param text: 要处理的字符串
 * @param letter_type: 字符类型
 * @return: 处理结果
*/
CORE_API std::string number_hyphen_reverse(std::string text, int letter_type);
CORE_API std::u32string number_hyphen_reverse(const std::u32string& text, int letter_type);

/**
 * 转换标点符号
 * @param text: 要处理的字符串
 * @param src_letter_type: 源字符类型
 * @param dst_letter_type: 目标字符类型
 * @return: 处理结果
*/
CORE_API std::u32string punctuation_convert(const std::u32string& text, int src_letter_type, int dst_letter_type);



/**
 * 将字符串映射到另一个字符类型。这个函数只会单纯的映射字符，不能直接当作字符串转换函数。
 * @param text 待处理字符串
 * @param src_letter_type 源字符串字符类型
 * @param dst_letter_type 目标字符类型
 * @return 目标字符串
 */
CORE_API std::u32string convert_mapping(std::u32string text,int src_letter_type,int dst_letter_type);
/**
 * 将字符串映射到另一个字符类型。
 * @param text 待处理的字符串
 * @param src_letter_type 源字符串字符类型
 * @param dst_letter_type 目标字符类型
 * @return 目标字符串
 */
CORE_API std::u32string string_convert(std::u32string text,int src_letter_type,int dst_letter_type);
CORE_API std::u32string capitalize(const std::u32string& str,int letter_type,const std::vector<std::u32string>& endingSeparator);

CORE_API std::u32string string_norm(const std::u32string& text,int letter_type);
CORE_API std::u32string string_norm_reverse(const std::u32string& text,int letter_type);


/**
 * 获取有效的音素格式列表
 * @return: 音素列表
*/
CORE_API std::unordered_set<std::u32string> get_valid_syllables();
CORE_API bool is_valid_syllable(const std::u32string& pattern);
CORE_API std::u32string mk_syllables_vc_block(const std::vector<std::u32string>& letter_list,int letter_type);
CORE_API std::vector<std::u32string> mk_multi_char_list(int letter_type);
CORE_API std::vector<std::u32string> mk_letter_list(
        const std::u32string& word,
        const std::vector<std::u32string>& multi_chars
);

CORE_API std::vector<std::u32string> reverse_iterate_syllable(
        const std::u32string& vc_pattern,
        bool split_vowels
);

CORE_API std::vector<std::u32string> forward_correct_syllables(
        const std::vector<std::u32string>& syllable_list
);


CORE_API std::u32string process_syllables(
        const std::u32string& text,
        int letter_type,
        bool split_vowel,
        const std::u32string& delimiter=U" "
);

CORE_API std::u32string reverse_syllables(
        const std::u32string& text,
        int letter_type,
        const std::u32string& delimiter
);