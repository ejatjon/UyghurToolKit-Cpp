#pragma once

#include <string>
#include <map>
#include <vector>

#include "macros.h"

//  老维文UEY.符号ئ（\u0626） 拉丁ULY(’) 不存在UYY,UKY,IPA 的对应表示
using namespace std;
struct CORE_API LetterCategory {
    u32string sentence_start;
    u32string other;
};

struct UEYLetterCategory{
    u32string Base;
    u32string Isolated;
    u32string Simple_Isolated;
    u32string Initial;
    u32string Simple_Initial;
    u32string Medial;
    u32string Separated_Medial;
    u32string Final;
    u32string Separated_Final;
};
struct CORE_API LetterData {
    vector<u32string> ipa;
    LetterCategory uly;
    LetterCategory uyy;
    UEYLetterCategory uey;
    LetterCategory uky;
};

CORE_API void getLetterMap(map<u32string , LetterData>& latterMap,int type);

CORE_API  vector<LetterData>& getScripts();

CORE_API  map<u32string,u32string> getConvertLetterMap(int src_letter_type,int dst_letter_type);





