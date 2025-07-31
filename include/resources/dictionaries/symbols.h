//
// Created by EjatjonAmar on 2025/6/29.
//

#pragma once

#include <string>
#include <map>

#include "macros.h"


struct SymbolData {
    std::u32string symbol;
    std::u32string symbol_strings[5];
};

struct PunctuationData {
    char32_t punctuation_strings[5];
};

// symbol,UEY,ULY,UYY,UKY,IPA
inline SymbolData SYMBOLS[] = {
        {U"%", U"پىرسەنت", U"pirsent", U"pirsənt", U"пирсәнт", U"pirsɛnt"},
        {U"$", U"دوللار", U"dollar", U"dollar", U"доллар", U"dollɑr"},
        {U"€", U"يېۋرو", U"yëwro", U"yewro", U"йевро", U"jewro"},
        {U"£", U"فونىستىرلىڭ", U"fonistirling", U"fonistirling", U"фонистирлиң", U"fonistirliŋ"},
        {U"¥", U"يۇئەن", U"yu’en", U"yuən", U"йуән", U"juɛn"},
        {U"℃", U"سېلسىيە گرادۇس", U"sëlsiye gradu", U"selsiyə gradus", U"селсийә градус", U"selsijɛ ɡrɑdus"},
        {U"°", U"گرادۇس", U"gradu", U"gradus", U"градус", U"ɡrɑdus"}
};
inline std::map<std::u32string,std::u32string> getSymbolMap(int letter_type)
{
    if(letter_type <1 || letter_type >5)
        return {};
    std::map<std::u32string,std::u32string> symbolMap;
    for(auto& symbol : SYMBOLS){
        symbolMap[symbol.symbol] = symbol.symbol_strings[letter_type-1];
    }
    return symbolMap;
}
inline std::map<std::u32string ,std::u32string> getReverseSymbolMap(int letter_type)
{
    if(letter_type <1 || letter_type >5)
        return {};
    std::map<std::u32string,std::u32string> reverseSymbolMap;
    for(auto& symbol : SYMBOLS){
        reverseSymbolMap[symbol.symbol_strings[letter_type-1]] = symbol.symbol;
    }
    return reverseSymbolMap;
}



// UEY,ULY,UYY,UKY,IPA

inline PunctuationData PUNCTUATIONS[] = {
        {U')', U'(', U'(', U'(', U'('},
        {U'(', U')', U')', U')', U')'},
        {U']', U'[', U'[', U'[', U']'},
        {U'[', U']', U']', U']', U']'},
        {U'}', U'{', U'{', U'{', U'}'},
        {U'{', U'}', U'}', U'}', U'}'},
        {U'>', U'<', U'<', U'<', U'<'},
        {U'<', U'>', U'>', U'>', U'>'},
        {U'»', U'«', U'«', U'«', U'«'},
        {U'«', U'»', U'»', U'»', U'»'},
        {U'؟', U'?', U'?', U'?', U'?'},
        {U'؛', U';', U';', U';', U';'},
        {U'،', U',', U',', U',', U','}
};

inline std::vector<std::vector<std::u32string>> sentence_ending_separator ={
        {U"؟", U"»", U")", U"؛", U"!", U"."},
        {U"?", U"«", U"(", U";", U"!", U"."},
        {U"?", U"«", U"(", U";", U"!", U"."},
        {U"?", U"«", U"(", U";", U"!", U"."},
        {U"?", U"«", U"(", U";", U"!", U"."}
};

inline std::map<char32_t,char32_t> getPunctuationMap(int src_letter_type,int dst_letter_type)
{
    if (src_letter_type<1 || src_letter_type>5 || dst_letter_type<1 || dst_letter_type>5)
        return {};
    std::map<char32_t,char32_t> punctuation_map;
    for(auto &punctuation:PUNCTUATIONS){
        punctuation_map[punctuation.punctuation_strings[src_letter_type-1]]=punctuation.punctuation_strings[dst_letter_type-1];
    }
    return punctuation_map;
}


