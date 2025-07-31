//
// Created by EjatjonAmar on 2025/6/29.
//

#pragma once
#include <map>
#include <string>

#include "scripts.h"
#include "tools/text.h"

enum class UEY_LETTER_FROM{
    Base,Isolated,Simple_Isolated,Initial,Simple_Initial,Medial,Separated_Medial,Final,Separated_Final
};


CORE_API std::map<std::u32string,std::u32string> getNormalizeMap(int letter_type);

CORE_API std::map<std::u32string,std::u32string> getCaseMapExcludeUeyAndIpa(int letter_type);

CORE_API std::u32string getUeyForm(std::u32string base_letter,UEY_LETTER_FROM  from_type);

CORE_API bool haveUeyFrom(std::u32string base_letter,UEY_LETTER_FROM  from_type);