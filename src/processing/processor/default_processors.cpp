//
// Created by EjatjonAmar on 2025/7/2.
//

#include "processing/processor/default_processors.h"

#include <utility>
#include "tools/number.h"
#include "tools/text.h"

DefaultNumberProcessor::DefaultNumberProcessor(ProcesClass&& p_class)
    :Processor(p_class,ProcesType::NUMBER,
        "DefaultNumberProcessor") {

}

DefaultNumberProcessor::DefaultNumberProcessor(ProcesClass &&p_class,
                                               std::vector<std::u32string> exclude_before,
                                               std::vector<std::u32string> exclude_after)
                                               :Processor(p_class,ProcesType::NUMBER,
                                                          "DefaultNumberProcessor"),
                                                          int_exclude_after(exclude_after),int_exclude_before(exclude_after),
                                                          decimal_exclude_after(std::move(exclude_after)),decimal_exclude_before(std::move(exclude_before))
                                                          {
}

DefaultNumberProcessor::DefaultNumberProcessor(ProcesClass &&p_class,std::vector<std::u32string> int_exclude_before,
                                               std::vector<std::u32string> int_exclude_after,
                                               std::vector<std::u32string> decimal_exclude_before,
                                               std::vector<std::u32string> decimal_exclude_after)
                                               :Processor(p_class,ProcesType::NUMBER,
                                                          "DefaultNumberProcessor"),
                                                          int_exclude_before(std::move(int_exclude_before)),int_exclude_after(std::move(int_exclude_after)),
                                                          decimal_exclude_before(std::move(decimal_exclude_before)),decimal_exclude_after(std::move(decimal_exclude_after))
                                                          {
}


std::u32string DefaultNumberProcessor::process(std::u32string text) {
    text = process_integer_number(text,static_cast<int>(getClass())+1,&int_exclude_before,&int_exclude_after);
    text = process_decimal_number(text,static_cast<int>(getClass())+1,&decimal_exclude_before,&decimal_exclude_after);
    return text;
}

std::u32string DefaultNumberProcessor::reverse(std::u32string text) {
    text = process_reverse_decimal_number(text,static_cast<int>(getClass())+1);
    text = process_reverse_integer_number(text,static_cast<int>(getClass())+1);
    return text;
}



// 音节划分

DefaultDivideSyllablesProcessor::DefaultDivideSyllablesProcessor(ProcesClass &&p_class):
Processor(p_class,ProcesType::SYLLABLES,"DefaultDivideSyllablesProcessor"),split_vowel(true),delimiter(U" ") {

}

DefaultDivideSyllablesProcessor::DefaultDivideSyllablesProcessor(ProcesClass &&p_class, bool split_vowel,
                                                                 std::u32string&& delimiter):
                                                                 Processor(p_class,ProcesType::SYLLABLES,"DefaultDivideSyllablesProcessor"),
                                                                 split_vowel(split_vowel),delimiter(delimiter) {

}

std::u32string DefaultDivideSyllablesProcessor::process(std::u32string text) {

    return process_syllables(text,static_cast<int>(getClass())+1,split_vowel,delimiter);
}

std::u32string DefaultDivideSyllablesProcessor::reverse(std::u32string text) {
    return reverse_syllables(text,static_cast<int>(getClass())+1,delimiter);
}

// 标准化

DefaultNormalizeProcessor::DefaultNormalizeProcessor(ProcesClass &&p_class)
:Processor(p_class,ProcesType::NORMALIZATION,"DefaultNormalizeProcessor") {

}

std::u32string DefaultNormalizeProcessor::process(std::u32string text) {
    return string_norm(text,static_cast<int>(getClass()));
}

std::u32string DefaultNormalizeProcessor::reverse(std::u32string text) {
    return string_norm_reverse(text,static_cast<int>(getClass()));
}


DefaultSymbolProcessor::DefaultSymbolProcessor(ProcesClass &&p_class, bool split_vowel, std::u32string delimiter)
: Processor(p_class,ProcesType::SYLLABLES,"DefaultSymbolProcessor"),split_vowel(split_vowel),delimiter(std::move(delimiter)) {

}

std::u32string DefaultSymbolProcessor::process(std::u32string text) {
    return process_syllables(text,static_cast<int>(getClass()),split_vowel,delimiter);
}

std::u32string DefaultSymbolProcessor::reverse(std::u32string text) {
    return reverse_syllables(text,static_cast<int>(getClass()),delimiter);
}


DefaultLetterConversionProcessor::DefaultLetterConversionProcessor(ProcesClass &&p_class,int src_letter_type,int dst_letter_type)
: Processor(p_class,ProcesType::CONVERSION,"DefaultLetterConversionProcessor") {

}

std::u32string DefaultLetterConversionProcessor::process(std::u32string text) {
    text=punctuation_convert(text,src_letter_type,dst_letter_type);
    return string_convert(text,src_letter_type,dst_letter_type);
}

std::u32string DefaultLetterConversionProcessor::reverse(std::u32string text) {
    text=punctuation_convert(text,dst_letter_type,src_letter_type);
    return string_convert(text,dst_letter_type,src_letter_type);
}


DefaultDateProcessor::DefaultDateProcessor(ProcesClass &&p_class, const vector<DateRegexInfo> info=get_default_date_regex_info())
: Processor(p_class,ProcesType::DATE,"DefaultDateProcessor"),info(info){

}

std::u32string DefaultDateProcessor::process(std::u32string text) {
    auto utf8_text= utf32_to_utf8(text);
    text= utf8_to_utf32(process_date(utf8_text,info,static_cast<int>(getClass())));
    return text;
}

std::u32string DefaultDateProcessor::reverse(std::u32string text) {
    return text;
}
