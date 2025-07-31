//
// Created by EjatjonAmar on 2025/6/29.
//

#pragma once
#include <string>
#include <iterator>
#include <map>
#include "macros.h"


// number,UEY,ULY,UYY,UKY,IPA

struct NumberData {
    int number;
    const std::u32string string_numbers[5];
};


const NumberData NUMBERS[] = {
        NumberData{0, U"نۆل", U"nöl", U"nɵl", U"нөл", U"nøl"},
        NumberData{1, U"بىر", U"bir", U"bir", U"бир", U"bir"},
        NumberData{2, U"ئىككى", U"ikki", U"ikki", U"икки", U"ikki"},
        NumberData{3, U"ئۈچ", U"üch", U"üq", U"үч", U"yt͡ʃ"},
        NumberData{4, U"تۆت", U"töt", U"tɵt", U"төт", U"tøt"},
        NumberData{5, U"بەش", U"besh", U"bəx", U"бәш", U"bɛʃ"},
        NumberData{6, U"ئالتە", U"alte", U"altə", U"алтә", U"ɑltɛ"},
        NumberData{7, U"يەتتە", U"yette", U"yəttə", U"йәттә", U"jɛttɛ"},
        NumberData{8, U"سەككىز", U"sekkiz", U"səkkiz", U"сәккиз", U"sɛkkiz"},
        NumberData{9, U"توققۇز", U"toqquz", U"toⱪⱪuz", U"тоққуз", U"toqquz"},
        NumberData{10, U"ئون", U"on", U"on", U"он", U"on"},
        NumberData{20, U"يىگىرمە", U"yigirme", U"yigirmə", U"йигирмә", U"jiɡirmɛ"},
        NumberData{30, U"ئوتتۇز", U"ottuz", U"ottuz", U"оттуз", U"ottuz"},
        NumberData{40, U"قىرىق", U"qiriq", U"ⱪiriⱪ", U"қириқ", U"qiriq"},
        NumberData{50, U"ئەللىك", U"ellik", U"əllik", U"әллик", U"ɛllik"},
        NumberData{60, U"ئاتمىش", U"atmish", U"atmix", U"атмиш", U"ɑtmiʃ"},
        NumberData{70, U"يەتمىش", U"yetmish", U"yətmix", U"йәтмиш", U"jɛtmiʃ"},
        NumberData{80, U"سەكسەن", U"seksen", U"səksən", U"сәксән", U"sɛksɛn"},
        NumberData{90, U"توقسان", U"toqsan", U"toⱪsan", U"тоқсан", U"toqsɑn"},
        NumberData{100, U"يۈز", U"yüz", U"yüz", U"йүз", U"jyz"},
        NumberData{1000, U"مىڭ", U"ming", U"ming", U"миң", U"miŋ"},
        NumberData{1000000, U"مىليون", U"milyon", U"milyon", U"милйон", U"miljon"},
        NumberData{1000000000, U"مىليارد", U"milyard", U"milyard", U"милйард", U"miljɑrd"},
};


const std::tuple<std::u32string ,std::u32string> decimal_adverbs[]={
        std::make_tuple(U"پۈتۈن",U"دە"),
        std::make_tuple(U"pütün",U"de"),
        std::make_tuple(U"pütün",U"də"),
        std::make_tuple(U"пүтүн",U"дә"),
        std::make_tuple(U"pytyn",U"dɛ"),
};

const std::vector<std::tuple<std::u32string,std::u32string>> number_hyphen_prefix_map={
        {U"\u0649",U"\u06d5"},
        {U"\u0069",U"\u0065"},
        {U"\u0069",U"\u0259"},
        {U"\u0438",U"\u04d9"},
        {U"\u0069",U"\u025b"},
};
const std::vector<std::u32string> number_hyphen_map={
        U"ىنچى",
        U"inchi",
        U"inqi",
        U"инчи",
        U"int͡ʃi"
};

inline std::map<int,std::u32string> getNumberMapWithUtf32(int letter_type){
    if (letter_type >5 || letter_type <1)
        return {};
    std::map<int,std::u32string> numberMap;
    for(const auto& number:NUMBERS){
        numberMap[number.number] = number.string_numbers[letter_type - 1];
    }
    return numberMap;
}

inline std::map<int,std::u16string> getNumberMapWithUtf16(int letter_type){
    if (letter_type >5 || letter_type <1)
        return {};
    std::map<int,std::u16string> numberMap;
    for(const auto& number:NUMBERS){
        numberMap[number.number] = utf32_to_utf16(number.string_numbers[letter_type - 1]);
    }
    return numberMap;
}

inline std::map<int,std::string> getNumberMap(int letter_type){
    if (letter_type >5 || letter_type <1)
        return {};
    std::map<int,std::string> numberMap;
    for(const auto& number:NUMBERS){
        numberMap[number.number] = utf32_to_utf8(number.string_numbers[letter_type - 1]);
    }
    return numberMap;
}


inline std::map<std::u32string,int> getReverseNumberMapWithUtf32(int letter_type){
    if (letter_type >5 || letter_type <1)
        return {};
    std::map<std::u32string,int> numberReverseMap;
    for(const auto& number:NUMBERS){
        numberReverseMap[number.string_numbers[letter_type - 1]] = number.number;
    }
    return numberReverseMap;
}

inline std::map<std::u16string,int> getReverseNumberMapWithUtf16(int letter_type){
    if (letter_type >5 || letter_type <1)
        return {};
    std::map<std::u16string,int> numberReverseMap;
    for(const auto& number:NUMBERS){
        numberReverseMap[utf32_to_utf16(number.string_numbers[letter_type - 1])] = number.number;
    }
    return numberReverseMap;
}

inline std::map<std::string,int> getReverseNumberMap(int letter_type){
    if (letter_type >5 || letter_type <1)
        return {};
    std::map<std::string,int> numberReverseMap;
    for(const auto& number:NUMBERS){
        numberReverseMap[utf32_to_utf8(number.string_numbers[letter_type - 1])] = number.number;
    }
    return numberReverseMap;
}
