//
// Created by EjatjonAmar on 2025/6/29.
//
#include <functional>
#include "tools/text.h"
#include "resources/dictionaries/scripts.h"
#include "resources/dictionaries/letter_normalize.h"


CORE_API bool is_vowel(const char32_t *c,int letter_type){
    switch (letter_type) {
        case LETTER_TYPE_IPA:
            return in_array(c,IPA_VOWELS,10);
        case LETTER_TYPE_UEY:
            return in_array(c,UEY_VOWELS,8);
        case LETTER_TYPE_UKY:
            return in_array(c,UKY_VOWELS,8);
        case LETTER_TYPE_ULY:
            return in_array(c,ULY_VOWELS,8);
        case LETTER_TYPE_UYY:
            return in_array(c,UYY_VOWELS,8);
        default:
            return false;
    }
}
CORE_API bool is_vowel(const std::u32string& letter,int letter_type){
    // 多字符字母默认为辅音
    if (letter.length() != 1) return false;
    return is_vowel(letter.c_str(),letter_type);

}

CORE_API std::u32string number_hyphen_converter(std::u32string text, int letter_type){
    if (letter_type<1 || letter_type>5)
        return text;
    const char32_t end_char = text.back();
    switch (letter_type) {
        case LETTER_TYPE_UEY:
            if(end_char == U'\u06d5' || end_char == U'\u0649')
                return text.substr(0,text.size()-1)+U"\u0649\u0646\u0686\u0649";
            else
                return text+U"\u0649\u0646\u0686\u0649";
        case LETTER_TYPE_UKY:
            if(end_char == U'\u04d9' || end_char == U'\u0438')
                return text.substr(0,text.size()-1)+U"\u0438\u043d\u0447\u0438";
            else
                return text+U"\u0438\u043d\u0447\u0438";
        case LETTER_TYPE_ULY:
            if(end_char == U'\u0065' || end_char == U'\u0069')
                return text.substr(0,text.size()-1)+U"\u0069\u006e\u0063\u0068\u0069";
            else
                return text+U"\u0069\u006e\u0063\u0068\u0069";
        case LETTER_TYPE_UYY:
            if(end_char == U'\u0259' || end_char == U'\u0069')
                return text.substr(0,text.size()-1)+U"\u0069\u006e\u0071\u0069";
            else
                return text+U"\u0069\u006e\u0071\u0069";
        case LETTER_TYPE_IPA:
            if(end_char == U'\u025b'
            || end_char == U'\u00e6'
            || end_char == U'\u0069'
            || end_char == U'\u0268')
                return text.substr(0,text.size()-1)+U"\u0069\u006e\u0074\u0361\u0283\u0069";
            else
                return text+U"\u0069\u006e\u0074\u0361\u0283\u0069";
        default:
            return text;

    }

}


CORE_API std::string number_hyphen_reverse(std::string text, int letter_type){
    if (letter_type<1 || letter_type>5)
        return text;
    std::string pattern;
    pattern = "(\\S+?)(" + utf32_to_utf8(number_hyphen_map[letter_type-1]) + ")(?=\\s|$)";
    auto [i,a] = number_hyphen_prefix_map[letter_type - 1];
    auto u8i= utf32_to_utf8(i);
    auto u8a = utf32_to_utf8(i);
    const std::map<std::string,int> reverseNumberMap = getReverseNumberMap(letter_type);
    std::regex regex(pattern);
    std::string result;

    auto words_begin = std::sregex_iterator(text.begin(), text.end(), regex);
    auto words_end = std::sregex_iterator();

    size_t last_pos = 0;
    for (auto it = words_begin; it != words_end; ++it) {
        const std::smatch& match = *it;
        std::string prefix = match[1].str();
        std::string suffix = match[3].str();

        // 添加匹配前的文本
        result += text.substr(last_pos, match.position() - last_pos);

        if (reverseNumberMap.find(prefix) != reverseNumberMap.end()) {
            result += " " + prefix + " - ";
        } else if (reverseNumberMap.find(prefix + u8i) != reverseNumberMap.end()) {
            result += ' ';
            result += prefix;
            result += u8i;
            result += " - ";
        } else if (reverseNumberMap.find(prefix + u8a) != reverseNumberMap.end()) {
            result += ' ';
            result += prefix;
            result += u8a;
            result += " - ";
        } else {
            result += match.str();
        }

        last_pos = match.position() + match.length();
    }

    // 添加剩余文本
    result += text.substr(last_pos);

    return result;
}

CORE_API std::u32string number_hyphen_reverse(const std::u32string& text, int letter_type){
    std::string utf8_text = utf32_to_utf8(text);
    return utf8_to_utf32(number_hyphen_reverse(utf8_text, letter_type));
}


CORE_API std::u32string convert_mapping(std::u32string text,int src_letter_type,int dst_letter_type){
    std::map<std::u32string,std::u32string> mapping = getConvertLetterMap(src_letter_type,dst_letter_type);
    std::u32string result;
    // 计算映射表中最长键的长度
    size_t max_key_len = 0;
    for (const auto& pair : mapping) {
        if (!pair.first.empty()) {
            max_key_len = std::max(max_key_len, pair.first.length());
        }
    }
    size_t i = 0;
    while (i < text.length()) {
        bool matched = false;
        size_t len = std::min(max_key_len, text.length() - i);
        while (len > 0) {
            std::u32string substr = text.substr(i, len);
            auto it = mapping.find(substr);
            if (it != mapping.end()) {
                result.append(it->second);
                i += len;
                matched = true;
                break;
            }
            len--;
        }
        if (!matched) {
            result.push_back(text[i]);
            i++;
        }
    }
    return result;
}

std::u32string string_convert(std::u32string text,int src_letter_type,int dst_letter_type){
    if(dst_letter_type==LETTER_TYPE_UEY || dst_letter_type==LETTER_TYPE_ULY){
        auto syllables = process_syllables(text,src_letter_type,true,U"#--**--#");
        if (src_letter_type == LETTER_TYPE_UEY){
            replace_substring<std::u32string>(syllables,U"\u0626",U"");
        }
        text = convert_mapping(syllables,src_letter_type,dst_letter_type);
        text = reverse_syllables(text, dst_letter_type,U"#--**--#");
    }else{
        if (src_letter_type == LETTER_TYPE_UEY){
            replace_substring<std::u32string>(text,U"\u0626",U"");
        }
        text = convert_mapping(text,src_letter_type,dst_letter_type);
    }
    return text;
}

std::u32string string_norm(const std::u32string& text,int letter_type){
    if (letter_type<1 || letter_type>5)
        throw std::invalid_argument("Invalid letter type");
    std::map<std::u32string ,std::u32string> norm_map=getNormalizeMap(letter_type);
    std::u32string result;
    size_t max_key_len = 0;
    for (const auto& pair : norm_map) {
        if (!pair.first.empty()) {
            max_key_len = std::max(max_key_len, pair.first.length());
        }
    }
    size_t i = 0;
    while (i < text.length()) {
        bool matched = false;
        size_t len = std::min(max_key_len, text.length() - i);
        while (len > 0) {
            std::u32string substr = text.substr(i, len);
            auto it = norm_map.find(substr);
            if (it != norm_map.end()) {
                result.append(it->second);
                i += len;
                matched = true;
                break;
            }
            len--;
        }
        if (!matched) {
            result.push_back(text[i]);
            i++;
        }
    }
    return result;
}

std::u32string capitalize(const std::u32string& str,int letter_type,const std::vector<std::u32string>& endingSeparator) {
    if(letter_type<1 || letter_type>5)
        throw std::invalid_argument("Invalid letter type");
    if (str.empty() || letter_type==1 || letter_type==5) return str;

    std::u32string result{};
    bool new_sentence = true; // 标记是否是新句子
    auto case_map = getCaseMapExcludeUeyAndIpa(letter_type);
    for (const auto& c: mk_letter_list(str, mk_multi_char_list(letter_type))) {
        if(new_sentence){
            result += case_map.at(c);
            new_sentence = false;
            continue;
        }
        result+= c;
        if(!endingSeparator.empty() && std::find(endingSeparator.begin(), endingSeparator.end(), c) != endingSeparator.end()){
            new_sentence = true;
        }
    }

    return result;
}

std::u32string string_norm_reverse(const std::u32string& text,int letter_type){
    if (letter_type < 1 || letter_type > 5)
        throw std::invalid_argument("Invalid letter type");
    if (letter_type>1){
        return capitalize(text,letter_type,sentence_ending_separator[letter_type-1]);
    }
    std::vector<std::u32string> result{};
    std::function integrate_letter=[](const std::vector<std::u32string>& letter_list) -> std::u32string{
        const auto& prev_letter = letter_list[1];
        auto letter = letter_list[2];
        const auto& next_letter = letter_list[3];

        if( prev_letter.empty() && next_letter.empty())
            return getUeyForm(letter,UEY_LETTER_FROM::Isolated);
        else if(prev_letter.empty() && !next_letter.empty())
            return getUeyForm(letter,UEY_LETTER_FROM::Initial);
        else if(!prev_letter.empty() && next_letter.empty()){
            if (haveUeyFrom(prev_letter,UEY_LETTER_FROM::Medial) || haveUeyFrom(prev_letter,UEY_LETTER_FROM::Initial)){
                return getUeyForm(letter,UEY_LETTER_FROM::Final);
            }else{
                return getUeyForm(letter,UEY_LETTER_FROM::Simple_Isolated);
            }
        }
        else{// !prev_letter.empty() || !next_letter.empty()
            if(haveUeyFrom(prev_letter,UEY_LETTER_FROM::Medial) || haveUeyFrom(prev_letter,UEY_LETTER_FROM::Initial))
                if(haveUeyFrom(next_letter,UEY_LETTER_FROM::Medial) || haveUeyFrom(next_letter,UEY_LETTER_FROM::Final))
                    return getUeyForm(letter,UEY_LETTER_FROM::Medial);
                else
                    return getUeyForm(letter,UEY_LETTER_FROM::Final);
            else
                if(haveUeyFrom(next_letter,UEY_LETTER_FROM::Medial) || haveUeyFrom(next_letter,UEY_LETTER_FROM::Final))
                    return getUeyForm(letter,UEY_LETTER_FROM::Simple_Initial);
                else
                    return getUeyForm(letter,UEY_LETTER_FROM::Simple_Isolated);
        }
    };
    for (auto word: split_string<std::u32string>(text,U" ")){
        if(word.length()<=1)
            return getUeyForm(word,UEY_LETTER_FROM::Isolated);
        std::vector<std::u32string> word_letter_list = mk_letter_list(word, mk_multi_char_list(letter_type));
        word_letter_list.insert(word_letter_list.begin(),U"");
        word_letter_list.insert(word_letter_list.begin(),U"");
        word_letter_list.push_back(U"");
        size_t index = 2;
        std::u32string word_result;
        while (index < word_letter_list.size()-1){
            std::vector<std::u32string> slice(word_letter_list.begin()+index-2, word_letter_list.begin()+index+2);
            word_result += integrate_letter(slice);
            index+=1;
        }
        result.push_back(word_result);
    }
    std::u32string result_str=join_strings<std::u32string>(result,U" ");
    replace_substring<std::u32string>(result_str,U"\ufedf\ufe8e",U"\uFEFB"); // ﻟ ﺎ -> ﻻ
    replace_substring<std::u32string>(result_str,U"\ufee0\ufe8e",U"\uFEFC"); // ﻠ ﺎ -> ﻼ
    return result_str;
}

// 标点符号转换
CORE_API std::u32string punctuation_convert(const u32string &text, int src_letter_type, int dst_letter_type) {
    std::map<char32_t, char32_t> punctuation_map = getPunctuationMap(src_letter_type, dst_letter_type);
    std::u32string result;
    for (char32_t c : text) {
        if (punctuation_map.find(c) != punctuation_map.end()) {
            result += punctuation_map.at(c);
            continue;
        }
        result += c;
    }
    return result;
}

std::unordered_set<std::u32string> get_valid_syllables() {
    static const std::unordered_set<std::u32string> VALID_SYLLABLES = {
            U"V", U"VC", U"CV", U"CVC", U"VCC", U"CVCC",
            U"CCV", U"CCVC", U"CCVCC", U"CVV", U"CVVC", U"CCCV"
    };
    return VALID_SYLLABLES;
}

std::u32string mk_syllables_vc_block(const std::vector<std::u32string>& letter_list,int letter_type) {
    if (letter_type<1 || letter_type>5)
        throw std::invalid_argument("Invalid letter type");
    std::u32string pattern;
    for (const auto& letter : letter_list) {
        if(is_vowel(letter,letter_type))
            pattern+=U"V";
        else
            pattern+=U"C";
    }
    return pattern;
}

std::vector<std::u32string> mk_letter_list(
        const std::u32string& word,
        const std::vector<std::u32string>& multi_chars
) {
    if (word.empty()) return {};

    size_t max_multi_len = 0;
    for (const auto& mc : multi_chars) {
        if (mc.length() > max_multi_len) {
            max_multi_len = mc.length();
        }
    }

    std::vector<std::vector<std::u32string>> len_map(max_multi_len + 1);
    for (const auto& mc : multi_chars) {
        if (!mc.empty()) {
            len_map[mc.length()].push_back(mc);
        }
    }

    std::vector<std::u32string> letters;
    size_t i = 0;
    size_t word_len = word.length();

    while (i < word_len) {
        size_t max_len = std::min(max_multi_len, word_len - i);
        bool found_multi = false;

        for (size_t len = max_len; len >= 1; len--) {
            if (len_map[len].empty()) continue;

            std::u32string substr = word.substr(i, len);
            for (const auto& mc : len_map[len]) {
                if (substr == mc) {
                    letters.push_back(substr);
                    i += len;
                    found_multi = true;
                    break;
                }
            }
            if (found_multi) break;
        }
        if (!found_multi) {
            letters.emplace_back(1, word[i]);
            i++;
        }
    }
    return letters;
}

std::vector<std::u32string> mk_multi_char_list(int letter_type) {
    if (letter_type<1 || letter_type>5)
        throw std::invalid_argument("Invalid letter type");
    vector<u32string> multi_chars;
    map<u32string, LetterData> letterMap{};
    getLetterMap(letterMap,letter_type);
    for(const auto& c:letterMap){
        if(c.first.length()>1)
            multi_chars.push_back(c.first);
    }
    return multi_chars;
}

// 判断音节模式是否有效
bool is_valid_syllable(const std::u32string& pattern) {
    auto VALID_SYLLABLES_SET = get_valid_syllables();
    return VALID_SYLLABLES_SET.find(pattern) != VALID_SYLLABLES_SET.end();
}

std::vector<std::u32string> reverse_iterate_syllable(
        const std::u32string& vc_pattern,
        bool split_vowels
) {
    std::vector<std::u32string> syllables;
    if (vc_pattern.empty()) return syllables;
    int start,end;
    start = end = (int)vc_pattern.size()-1;
    bool prev_vowel = false;

    while (start >= 0) {
        if (vc_pattern[start] == U'V') {
            if (prev_vowel) {
                if (split_vowels) {
                    syllables.push_back(vc_pattern.substr(start+1, end - start));
                    end = start;
                }
            } else {
                prev_vowel = true;
            }
        } else { // 'C'
            if (prev_vowel) {
                syllables.push_back(vc_pattern.substr(start, end - (start-1)));
                end = start-1;
                prev_vowel = false;
            }
        }
        start--;
    }
    if (start < end) {
        syllables.push_back(vc_pattern.substr(0, end-start));
    }
    std::reverse(syllables.begin(), syllables.end());
    return syllables;
}

std::vector<std::u32string> forward_correct_syllables(
        const std::vector<std::u32string>& syllable_list
) {
    std::vector<std::u32string> result;
    if (syllable_list.empty()) return result;
    std::function<std::vector<std::u32string>(const std::u32string&)> iterateSyllable=[&iterateSyllable](const std::u32string& b) {
        std::vector<std::u32string> r{};
        if (is_valid_syllable(b)) {
            r.push_back(b);
            return r;
        }
        for (const auto& i:get_valid_syllables()){
            if(i.length() >= b.length()){
                continue;
            }
            auto p=b.substr(0, i.length());
            auto s = iterateSyllable(b.substr(i.length()+1));
            if ( p== i && !s.empty()) {
                r.push_back(i);
                r.insert(r.end(), s.begin(), s.end());
                return r;
            }
        }
        return r;
    };
    // 使用滑动窗口处理音节序列

    int index = 0;

    std::u32string pending{};
    while (index < syllable_list.size()) {

        pending+=syllable_list[index];
        // 检查当前音节是否有效
        if (is_valid_syllable(pending)) {
            result.push_back(pending);
            index++;
            pending=U"";
            continue;
        }
        // 尝试与后续音节合并
        while (index < syllable_list.size()-1){
            index++;
            pending += syllable_list[index];
            auto n=iterateSyllable(pending);
            if (!n.empty()) {
                result.insert(result.end(), n.begin(), n.end());
                pending= U"";
                break;
            }
        }
        index++;
    }
    if(!pending.empty()){
        result.push_back(pending);
    }
    return result;
}



std::u32string process_syllables(
        const std::u32string& text,
        int letter_type,
        bool split_vowel,
        const std::u32string& delimiter
) {
    if(letter_type<1 || letter_type>5)
        throw std::invalid_argument("Invalid letter type");

    std::vector<std::u32string> syllable_list{};
    for(const auto& word: split_string<std::u32string>(text, U" ")){
        std::vector<std::u32string> word_blocks{};
        if (letter_type == 1){
            for(const auto& block: split_string<std::u32string>(word, U"\u0626")){
                word_blocks.push_back(block);
            }
        }
        else if (letter_type == 2){
            std::u32string prev{};
            for(auto block: split_string<std::u32string>(word, U"\u2019")){
                if (prev.empty()){
                    prev =block;
                    continue;
                }
                if (!block.empty() && !is_vowel(&prev.back(),letter_type) && is_vowel(&block[0],letter_type)){
                    word_blocks.push_back(prev);
                    prev=block;
                }
                else
                    prev+=U"\u2019"+block;
            }
            word_blocks.push_back(prev);
        }
        else{
            word_blocks.push_back(word);
        }

        std::u32string vc_pattern{};
        std::vector<std::u32string> word_syllables{};
        for(const auto& block: word_blocks){
            auto letters=mk_letter_list(block,mk_multi_char_list(letter_type));
            vc_pattern=mk_syllables_vc_block(letters,letter_type);
            auto vc_syllables= reverse_iterate_syllable(vc_pattern,split_vowel);
            vc_syllables=forward_correct_syllables(vc_syllables);
            size_t letter_index=0;
            for (const auto& vc:vc_syllables){
                std::u32string syllable{};
                for (size_t len=vc.length();len>0;len--){
                    syllable+=letters[letter_index];
                    letter_index++;
                }
                if(letter_type == 1)
                    word_syllables.push_back(is_vowel(syllable.substr(0,1),letter_type)? U"\u0626"+syllable:syllable);
                else
                    word_syllables.push_back(syllable);
            }
        }
        syllable_list.push_back(join_strings<u32string>(word_syllables,U" "));
    }
    return join_strings<u32string>(syllable_list,delimiter);
}

std::u32string reverse_syllables(
        const std::u32string& text,
        int letter_type,
        const std::u32string& delimiter
) {
    if(letter_type<1 || letter_type>5)
        throw std::invalid_argument("Invalid letter type");
    std::vector<std::u32string> result{};

        for(const auto& word: split_string<std::u32string>(text, delimiter)) {
            if (letter_type==2) {
                std::u32string prev{};

                for (auto &syllable: split_string<std::u32string>(word, U" ")) {
                    if (prev.empty()) {
                        prev = syllable;
                        continue;
                    }
                    if (!syllable.empty() && !is_vowel(&prev.back(), letter_type) &&
                        is_vowel(&syllable[0], letter_type)) {
                        prev += U"\u2019" + syllable;
                    } else
                        prev += syllable;
                }
                result.push_back(prev);
            }else {
                result.push_back(join_strings<std::u32string>(split_string<std::u32string>(word,U" "), U""));
            }
        }
    return join_strings<std::u32string>(result, U" ");
}




