//
// Created by EjatjonAmar on 2025/6/29.
//
#include "resources/dictionaries/letter_normalize.h"


CORE_API std::map<std::u32string,std::u32string> getNormalizeMap(int letter_type){
    if (letter_type<1 || letter_type>5)
        return {};
    std::map<std::u32string ,std::u32string> normalizeMap;
    auto scripts=getScripts();
    switch (letter_type) {
        case LETTER_TYPE_UEY:
            for (auto& script:scripts){
                if (!is_vowel(script.uey.Base.c_str() ,letter_type)){
                    normalizeMap[script.uey.Isolated]=script.uey.Base;
                    normalizeMap[script.uey.Final]=script.uey.Base;
                    if (!script.uey.Initial.empty()){
                        normalizeMap[script.uey.Initial]=script.uey.Base;
                    }
                    if (!script.uey.Medial.empty()){
                        normalizeMap[script.uey.Medial]=script.uey.Base;
                    }
                }else{
                    normalizeMap[script.uey.Isolated]=script.uey.Base;
                    normalizeMap[script.uey.Final]=script.uey.Base;
                    if (!script.uey.Initial.empty()){
                        normalizeMap[script.uey.Initial]=script.uey.Base;
                    }
                    if (!script.uey.Simple_Initial.empty()){
                        normalizeMap[script.uey.Simple_Initial]=script.uey.Base;
                    }
                    if (!script.uey.Medial.empty()){
                        normalizeMap[script.uey.Medial]=script.uey.Base;
                    }
                    if (!script.uey.Simple_Isolated.empty()){
                        normalizeMap[script.uey.Simple_Isolated]=script.uey.Base;
                    }
                }

            }
            return normalizeMap;
        case LETTER_TYPE_IPA:
            for (auto script:scripts){
                if (script.ipa.size()>1){
                    normalizeMap[script.ipa[1]]=script.ipa[0];
                }
            }
            return normalizeMap;
        case LETTER_TYPE_UKY:
            for (const auto& script:scripts){
                normalizeMap[script.uky.sentence_start]=script.uky.other;
            }
            return normalizeMap;
        case LETTER_TYPE_UYY:
            for (const auto& script:scripts){
                normalizeMap[script.uyy.sentence_start]=script.uyy.other;
            }
            return normalizeMap;
        case LETTER_TYPE_ULY:
            for (const auto& script:scripts){
                normalizeMap[script.uly.sentence_start]=script.uly.other;
            }
            return normalizeMap;
        default:
            return normalizeMap;
    }
}

CORE_API std::map<std::u32string,std::u32string> getCaseMapExcludeUeyAndIpa(int letter_type){
    if(letter_type<2 || letter_type>4)
        throw std::invalid_argument("letter_type must be between 2 and 4");
    std::map<std::u32string,std::u32string> caseMap;
    auto scripts=getScripts();
    for (auto &script:scripts){
        if (letter_type==LETTER_TYPE_UKY){
            if (!script.uky.other.empty()){
                caseMap[script.uky.other]=script.uky.sentence_start;
            }
        }else if (letter_type==LETTER_TYPE_UYY){
            if (!script.uyy.other.empty()){
                caseMap[script.uyy.other]=script.uyy.sentence_start;
            }
        }else if (letter_type==LETTER_TYPE_ULY){
            if (!script.uly.other.empty()){
                caseMap[script.uly.other]=script.uly.sentence_start;
            }
        }
    }
    return caseMap;
}

namespace {
    static map<u32string, LetterData> letterMap{};
}
std::u32string getUeyForm(std::u32string base_letter,UEY_LETTER_FROM  from_type){
    if(letterMap.empty())
        getLetterMap(letterMap,LETTER_TYPE_UEY);
    if(!letterMap.empty() && letterMap.contains(base_letter)){
        switch (from_type) {
            case UEY_LETTER_FROM::Base:
                return letterMap[base_letter].uey.Base;
            case UEY_LETTER_FROM::Isolated:
                if(letterMap[base_letter].uey.Isolated.empty()){
                    return letterMap[base_letter].uey.Base;
                }
                return letterMap[base_letter].uey.Isolated;
            case UEY_LETTER_FROM::Simple_Isolated:
                if(letterMap[base_letter].uey.Simple_Isolated.empty()){
                    return getUeyForm(base_letter,UEY_LETTER_FROM::Isolated);
                }
                return letterMap[base_letter].uey.Simple_Isolated;
            case UEY_LETTER_FROM::Initial:
                if(letterMap[base_letter].uey.Initial.empty()){
                    return getUeyForm(base_letter,UEY_LETTER_FROM::Simple_Isolated);
                }
                return letterMap[base_letter].uey.Initial;
            case UEY_LETTER_FROM::Simple_Initial:
                if(letterMap[base_letter].uey.Simple_Initial.empty()){
                    return getUeyForm(base_letter,UEY_LETTER_FROM::Initial);
                }
                return letterMap[base_letter].uey.Simple_Initial;
            case UEY_LETTER_FROM::Medial:
                if(letterMap[base_letter].uey.Medial.empty()){
                    return getUeyForm(base_letter,UEY_LETTER_FROM::Final);
                }
                return letterMap[base_letter].uey.Medial;
            case UEY_LETTER_FROM::Separated_Medial:
                if(letterMap[base_letter].uey.Separated_Medial.empty()){
                    return getUeyForm(base_letter,UEY_LETTER_FROM::Separated_Final);
                }
                return letterMap[base_letter].uey.Separated_Medial;
            case UEY_LETTER_FROM::Final:
                if(letterMap[base_letter].uey.Final.empty()){
                    return getUeyForm(base_letter,UEY_LETTER_FROM::Simple_Isolated);
                }
                return letterMap[base_letter].uey.Final;
            case UEY_LETTER_FROM::Separated_Final:
                if(letterMap[base_letter].uey.Separated_Final.empty()){
                    return getUeyForm(base_letter,UEY_LETTER_FROM::Final);
                }
                return letterMap[base_letter].uey.Separated_Final;
            default:
                return base_letter;
        }
    }
    return base_letter;
}
bool haveUeyFrom(std::u32string base_letter,UEY_LETTER_FROM  from_type){
    if(letterMap.empty())
        getLetterMap(letterMap,LETTER_TYPE_UEY);
    if(!letterMap.empty() && letterMap.contains(base_letter)){
        switch (from_type) {
            case UEY_LETTER_FROM::Base:
                return !letterMap[base_letter].uey.Base.empty();
            case UEY_LETTER_FROM::Isolated:
                return !letterMap[base_letter].uey.Isolated.empty();
            case UEY_LETTER_FROM::Simple_Isolated:
                return !letterMap[base_letter].uey.Simple_Isolated.empty();
            case UEY_LETTER_FROM::Initial:
                return !letterMap[base_letter].uey.Initial.empty();
            case UEY_LETTER_FROM::Simple_Initial:
                return !letterMap[base_letter].uey.Simple_Initial.empty();
            case UEY_LETTER_FROM::Medial:
                return !letterMap[base_letter].uey.Medial.empty();
            case UEY_LETTER_FROM::Separated_Medial:
                return !letterMap[base_letter].uey.Separated_Medial.empty();
            case UEY_LETTER_FROM::Final:
                return !letterMap[base_letter].uey.Final.empty();
            case UEY_LETTER_FROM::Separated_Final:
                return !letterMap[base_letter].uey.Separated_Final.empty();
            default:
                return false;
        }
    }
    return false;
}