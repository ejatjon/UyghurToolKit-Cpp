//
// Created by EjatjonAmar on 2025/6/28.
//
#include <stdexcept>
#include <functional>
#include "resources/dictionaries/scripts.h"

//  这些字符是特殊字符，通常充当辅助角色
//  特殊字符 \uFE8C(ﺌ) \uFE8B(ﺋ) \uFEFC(ﻼ) \uFEFB(ﻻ)
//  当要输入 ئا ئۆ 等字符时，使用 ئ 代替 ﺌ  ﺋ
//  当要输入 ﻻ  时，使用 ل+ا
//  \uFEFC('ﻼ') 是 uFEFB('ﻻ') 的前连式 形式
const vector<LetterData> uey_special_scripts = {
        LetterData{
                {U""}, // ئ
                {U"", U""},
                {U"", U""},
                {
                    U"\u0626",  // ئ
                    U"\u0626",  // ئ
                    U"",
                    U"\uFE8B",  // ﺋ
                    U"",
                    U"\uFE8C",  // ﺌ
                    U"",
                    U"",
                    U"",
                },
                {U"", U""},
        },
        LetterData{
                {U"\u006c\u0251"}, // lɑ
                {U"\u004c", U"\u006c"}, // {U"LA", U"la"}
                {U"\u004c", U"\u006c"}, // {U"LA", U"la"}
                {
                    U"\uFEFB",  // ﻻ
                    U"\uFEFB",  // ﻻ
                    U"",
                    U"",
                    U"",
                    U"",
                    U"",
                    U"\uFEFC",  // ﻼ
                    U"",
                },
                {U"\u041b\u0410", U"\u043b\u0430"},  // {U"ЛА", U"ла"}
        }
};

const std::vector<LetterData> uky_special_scripts={
        LetterData{
                {U"\u006a\u0075"}, // ju
                {U"\u0059\u0055", U"\u0079\u0075"},  // {"YU", U"yu"}
                {U"\u0059\u0055", U"\u0079\u0075"},  // {"YU", U"yu"}
                {
                     U"\u064a\u06c7",  // يۇ
                     U"\u064a\u06c7",  // يۇ
                     U"",
                     U"",
                     U"",
                     U"",
                     U"",
                     U"\ufef4\ufbd8",  // ﻴﯘ
                     U"",
                },
                {U"Ю", U"ю"},
        },
        LetterData{
                {U"\u006a\u0251"}, // jɑ
                {U"\u0059\u0041", U"\u0079\u0061"},  // {"YA", U"ya"}
                {U"\u0059\u0041", U"\u0079\u0061"},  // {"YA", U"ya"}
                {
                     U"\u064a\u0627",  //يا
                     U"\u064a\u0627",  //يا
                     U"",
                     U"",
                     U"",
                     U"\ufef4\ufe8e",  // ﻴﺎ
                     U"",  //
                     U"\ufef2",  // ﻲ
                     U"",  //
                },
                {U"Я", U"я"},
        }
};

void getLetterMap(map<u32string, LetterData> &letterMap,int type) {
    const vector<LetterData> scripts = getScripts();
    switch (type) {
        case LETTER_TYPE_UEY:
            for (const LetterData &script: scripts) {
                letterMap[script.uey.Base] = script;
            }
            for (const LetterData &script: uey_special_scripts) {
                letterMap[script.uey.Base] = script;
            }
            break;
        case LETTER_TYPE_ULY:
            for (const LetterData &script: scripts) {
                letterMap[script.uly.other] = script;
                letterMap[script.uly.sentence_start] = script;
            }
            break;
        case LETTER_TYPE_UYY:
            for (const LetterData &script: scripts) {
                letterMap[script.uyy.other] = script;
                letterMap[script.uyy.sentence_start] = script;
            }
            break;
        case LETTER_TYPE_UKY:
            for (const LetterData &script: scripts) {
                letterMap[script.uky.other] = script;
                letterMap[script.uky.sentence_start] = script;
            }
            for (const LetterData &script: uky_special_scripts) {
                letterMap[script.uky.other] = script;
                letterMap[script.uky.sentence_start] = script;
            }
            break;
        case LETTER_TYPE_IPA:
            for (const LetterData &script: scripts) {
                letterMap[script.ipa[0]] = script;
                if (script.ipa.size() > 1)
                    letterMap[script.ipa[1]] = script;
            }
            break;
        default:
            throw std::invalid_argument("Invalid letter type!");

    }
}

CORE_API vector<LetterData> &getScripts() {
    static vector<LetterData> scripts = {
            LetterData{
                    {U"\u0251"},  // ɑ
                    {U"\u0041", U"\u0061"},  // {"A", U"a"}
                    {U"\u0041", U"\u0061"},  // {"A", U"a"}
                    {
                     U"\u0627",  // ا
                                U"\uFE8B\ufe8e",  // ئا
                            U"\ufe8d",  // ﺍ
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"\ufe8e",  // ﺎ
                            U"\ufe8c\ufe8e",  // ﺌﺎ
                    },
                    {U"\u0410", U"\u0430"},  // {U"А", U"а"}
            },

            LetterData{
                    {U"\u025b", U"\u00e6"}, // ɛ
                    {U"\u0045", U"\u0065"},  // {U"E", U"e"}
                    {U"\u018f", U"\u0259"},  // {U"Ə", U"ə"}
                    {
                     U"\u06d5",  // ە
                                U"\uFE8B\ufeea",  // ئە
                            U"\ufee9",  // ﻩ
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"\ufeea",  // ﻪ
                            U"\ufe8c\ufeea",  // ﺌﻪ
                    },
                    {U"\u04d8", U"\u04d9"},  // {U"Ә", U"ә"}
            },


            LetterData{
                    {U"\u006f"}, // o
                    {U"\u004f", U"\u006f"},  // {U"O", U"o"}
                    {U"\u004f", U"\u006f"},  // {U"O", U"o"}
                    {
                     U"\u0648",  // و
                                U"\uFE8B\ufeee",  // ئو
                            U"\ufeed",  // ﻭ
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"\ufeee",  // ﻮ
                            U"\ufe8c\ufeee",  // ﺌﻮ
                    },
                    {U"\u041e", U"\u043e"},  // {U"О", U"о"}
            },

            LetterData{
                    {U"\u0075"}, // u
                    {U"\u0055", U"\u0075"},  // {U"U", U"u"}
                    {U"\u0055", U"\u0075"},  // {U"U", U"u"}
                    {
                     U"\u06c7",  // ۇ
                                U"\uFE8B\ufbd8",  // ئۇ
                            U"\ufbd7",  // ﯗ
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"\ufbd8",  // ﯘ
                            U"\ufe8c\ufbd8",  // ﺌﯘ
                    },
                    {U"\u0423", U"\u0443"},  // {U"У", U"у"}
            },

            LetterData{
                    {U"\u00f8"}, // ø
                    {U"\u00d6", U"\u00f6"},  // {U"Ö", U"ö"}
                    {U"\u019f", U"\u0275"},  // {U"Ɵ", U"ɵ"}
                    {
                     U"\u06c6",  // ۆ
                                U"\uFE8B\ufbda",  // ئۆ
                            U"\ufbd9",  // ﯙ
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"\ufbda",  // ﯚ
                            U"",  // (ﺌﯚ) 维语好像没有这种用法的词其他也一样
                    },
                    {U"\u04e8", U"\u04e9"},  // {U"Ө", U"ө"}
            },

            LetterData{
                    {U"\u0079"}, // y
                    {U"\u00dc", U"\u00fc"},  // {U"Ü", U"ü"}
                    {U"\u00dc", U"\u00fc"},  // {U"Ü", U"ü"}
                    {
                     U"\u06c8",  // ۈ
                                U"\uFE8B\ufbdc",  // ئۈ
                            U"\ufbdb",  // ﯛ
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"\ufbdc",  // ﯜ
                            U"",  //
                    },
                    {U"\u04ae", U"\u04af"},  // {U"Ү", U"ү"}
            },

            LetterData{
                    {U"\u0065"}, // e
                    {U"\u00cb", U"\u00eb"},  // {U"Ë", U"ë"}
                    {U"\u0045", U"\u0065"},  // {U"E", U"e"}
                    {
                     U"\u06d0",  // ې
                                U"\uFE8B\ufbe5",  // ئې
                            U"\u06d0",  // ې
                            U"\ufe8b\ufbe6",  // ﺋﯦ
                            U"\ufbe6",  // ﯦ
                            U"\ufbe7",  // ﯧ
                            U"",  //
                            U"\ufbe5",  // ﯥ
                            U"",  //
                    },
                    {U"\u0415", U"\u0435"},  // {U"Е", U"е"}
            },

            LetterData{
                    {U"\u0069", U"\u0268"}, // i
                    {U"\u0049", U"\u0069"},  // {U"I", U"i"}
                    {U"\u0049", U"\u0069"},  // {U"I", U"i"}
                    {
                     U"\u0649",  // ى
                                U"\uFE8B\ufef0",  // ئى
                            U"\ufeef",  // ﻯ
                            U"\ufe8b\ufbe8",  // ﺋﯨ
                            U"\ufbe8",  // ﯨ
                            U"\ufbe9",  // ﯩ
                            U"\ufe8c\ufbe9",  // ﺌﯩ
                            U"\ufef0",  // ﻰ
                            U"\ufe8c\ufef0",  // ﺌﻰ
                    },
                    {U"\u0418", U"\u0438"},  // {U"И", U"и"}
            },


            LetterData{
                    {U"\u0062"}, // b
                    {U"\u0042", U"\u0062"},  // {U"B", U"b"}
                    {U"\u0042", U"\u0062"},  // {U"B", U"b"}
                    {
                     U"\u0628",  // ب
                                U"\ufe8f",  // ﺏ
                            U"",  //
                            U"\ufe91",  // ﺑ
                            U"",  //
                            U"\ufe92",  // ﺒ
                            U"",  //
                            U"\ufe90",  // ﺐ
                            U"",  //
                    },
                    {U"\u0411", U"\u0431"},  // {U"Б", U"б"}
            },

            LetterData{
                    {U"\u0070"}, // p
                    {U"\u0050", U"\u0070"},  // {U"P", U"p"}
                    {U"\u0050", U"\u0070"},  // {U"P", U"p"}
                    {
                     U"\u067e",  // پ
                                U"\ufb56",  // ﭖ
                            U"",  //
                            U"\ufb58",  // ﭘ
                            U"",  //
                            U"\ufb59",  // ﭙ
                            U"",  //
                            U"\ufb57",  // ﭗ
                            U"",  //
                    },
                    {U"\u041f", U"\u043f"},  // {U"П", U"п"}
            },

            LetterData{
                    {U"\u0074"},// t
                    {U"\u0054", U"\u0074"},  // {U"T", U"t"}
                    {U"\u0054", U"\u0074"},  // {U"T", U"t"}
                    {
                     U"\u062a",  // ت
                                U"\ufe95",  // ﺕ
                            U"",  //
                            U"\ufe97",  // ﺗ
                            U"",  //
                            U"\ufe98",  // ﺘ
                            U"",  //
                            U"\ufe96",  // ﺖ
                            U"",  //
                    },
                    {U"\u0422", U"\u0442"},  // {U"Т", U"т"}
            },

            LetterData{
                    {U"\u0064\u0361\u0292"},// d͡ʒ
                    {U"\u004a", U"\u006a"},  // {U"J", U"j"}
                    {U"\u004a", U"\u006a"},  // {U"J", U"j"}
                    {
                     U"\u062c",  // ج
                                U"\ufe9d",  // ﺝ
                            U"",  //
                            U"\ufe9f",  // ﺟ
                            U"",  //
                            U"\ufea0",  // ﺠ
                            U"",  //
                            U"\ufe9e",  // ﺞ
                            U"",  //
                    },
                    {U"\u0496", U"\u0497"},  // {U"Җ", U"җ"}
            },

            LetterData{
                    {U"\u0074\u0361\u0283"},// t͡ʃ
                    {U"\u0043\u0068", U"\u0063\u0068"}, // {U"Ch", U"ch"}
                    {U"\u0051",       U"\u0071"},  // {U"Q", U"q"}
                    {
                     U"\u0686",  // چ
                                      U"\ufb7a",  // ﭺ
                            U"",  //
                            U"\ufb7c",  // ﭼ
                            U"",  //
                            U"\ufb7d",  // ﭽ
                            U"",  //
                            U"\ufb7b",  // ﭻ
                            U"",  //
                    },
                    {U"\u0427",       U"\u0447"},  // {U"Ч", U"ч"}
            },
            LetterData{
                    {U"\u03c7"}, // χ
                    {U"\u0058", U"\u0078"},  // {U"X", U"x"}
                    {U"\u0048", U"\u0068"},  // {U"H", U"h"}
                    {
                     U"\u062e",  // خ
                                U"\ufea5",  // ﺥ
                            U"",  //
                            U"\ufea7",  // ﺧ
                            U"",  //
                            U"\ufea8",  // ﺨ
                            U"",  //
                            U"\ufea6",  // ﺦ
                            U"",  //
                    },
                    {U"\u0425", U"\u0445"},  // {U"Х", U"х"}
            },

            LetterData{
                    {U"\u0064"}, // d
                    {U"\u0044", U"\u0064"},  // {U"D", U"d"}
                    {U"\u0044", U"\u0064"},  // {U"D", U"d"}
                    {
                     U"\u062f",  // د
                                U"\u062f",  // د
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"\ufeaa",  // ﺪ
                            U"",  //
                    },
                    {U"\u0414", U"\u0434"},  // {U"Д", U"д"}
            },

            LetterData{
                    {U"\u0072"}, // r
                    {U"\u0052", U"\u0072"},  // {U"R", U"r"}
                    {U"\u0052", U"\u0072"},  // {U"R", U"r"}
                    {
                     U"\u0631",  // ر
                                U"\u0631",  // ر
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"\ufeae",  // ﺮ
                            U"",  //
                    },
                    {U"\u0420", U"\u0440"},  // {U"Р", U"р"}
            },

            LetterData{
                    {U"\u007a"}, // z
                    {U"\u005a", U"\u007a"},  // {U"Z", U"z"}
                    {U"\u005a", U"\u007a"},  // {U"Z", U"z"}
                    {
                     U"\u0632",  // ز
                                U"\ufeaf",  // ﺯ
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"\ufeb0",  // ﺰ
                            U"",  //
                    },
                    {U"\u0417", U"\u0437"},  // {U"З", U"з"}
            },

            LetterData{
                    {U"\u0292"}, // ʒ
                    {U"\u005a\u0068", U"\u007a\u0068"}, // {U"Zh", U"zh"}
                    {U"\u2c6b",       U"\u2c6c"},  // {U"Ⱬ", U"ⱬ"}
                    {
                     U"\u0698",  // ژ
                                      U"\ufb8a",  // ﮊ
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"\ufb8b",  // ﮋ
                            U"",  //
                    },
                    {U"\u0416",       U"\u0436"},  // {U"Ж", U"ж"}
            },
            LetterData{
                    {U"\u0073"}, // s
                    {U"\u0053", U"\u0073"},// {U"S", U"s"}
                    {U"\u0053", U"\u0073"},  // {U"S", U"s"}
                    {
                     U"\u0633",  // س
                                U"\ufeb1",  // ﺱ
                            U"",  //
                            U"\ufeb3",  // ﺳ
                            U"",  //
                            U"\ufeb4",  // ﺴ
                            U"",  //
                            U"\ufeb2",  // ﺲ
                            U"",  //
                    },
                    {U"\u0421", U"\u0441"},  // {U"С", U"с"}
            },

            LetterData{
                    {U"\u0283"}, // ʃ
                    {U"\u0053\u0068", U"\u0073\u0068"}, // {U"Sh", U"sh"}
                    {U"\u0058",       U"\u0078"},  // {U"X", U"x"}
                    {
                     U"\u0634",  // ش
                                      U"\ufeb5",  // ﺵ
                            U"",  //
                            U"\ufeb7",  // ﺷ
                            U"",  //
                            U"\ufeb8",  // ﺸ
                            U"",  //
                            U"\ufeb6",  // ﺶ
                            U"",  //
                    },
                    {U"\u0428",       U"\u0448"},  // {U"Ш", U"ш"}
            },

            LetterData{
                    {U"\u0281"}, // ʁ
                    {U"\u0047\u0068", U"\u0067\u0068"}, // {U"Gh", U"gh"}
                    {U"\u01a2",       U"\u01a3"},  // {U"Ƣ", U"ƣ"}
                    {
                     U"\u063a",  // غ
                                      U"\ufecd",  // ﻍ
                            U"",  //
                            U"\ufecf",  // ﻏ
                            U"",  //
                            U"\ufed0",  // ﻐ
                            U"",  //
                            U"\ufece",  // ﻎ
                            U"",  //
                    },
                    {U"\u0492",       U"\u0493"},  // {U"Ғ", U"ғ"}
            },

            LetterData{
                    {U"\u0066"}, // f
                    {U"\u0046", U"\u0066"},  // {U"F", U"f"}
                    {U"\u0046", U"\u0066"},  // {U"F", U"f"}
                    {
                     U"\u0641",  // ف
                                U"\ufed1",  // ﻑ
                            U"",  //
                            U"\ufed3",  // ﻓ
                            U"",  //
                            U"\ufed4",  // ﻔ
                            U"",  //
                            U"\ufed2",  // ﻒ
                            U"",  //
                    },
                    {U"\u0424", U"\u0444"},  // {U"Ф", U"ф"}
            },

            LetterData{
                    {U"\u0071"}, // q
                    {U"\u0051", U"\u0071"},  // {U"Q", U"q"}
                    {U"\u2c69", U"\u2c6a"},  // {U"Ⱪ", U"ⱪ"}
                    {
                     U"\u0642",  // ق
                                U"\ufed5",  // ﻕ
                            U"",  //
                            U"\ufed7",  // ﻗ
                            U"",  //
                            U"\ufed8",  // ﻘ
                            U"",  //
                            U"\ufed6",  // ﻖ
                            U"",  //
                    },
                    {U"\u049a", U"\u049b"},  // {U"Қ", U"қ"}
            },

            LetterData{
                    {U"\u006b"}, // k
                    {U"\u004b", U"\u006b"},  // {U"K", U"k"}
                    {U"\u004b", U"\u006b"},  // {U"K", U"k"}
                    {
                     U"\u0643",  // ك
                                U"\ufed9",  // ﻙ
                            U"",  //
                            U"\ufedb",  // ﻛ
                            U"",  //
                            U"\ufedc",  // ﻜ
                            U"",  //
                            U"\ufeda",  // ﻚ
                            U"",  //
                    },
                    {U"\u041a", U"\u043a"},  // {U"К", U"к"}
            },

            LetterData{
                    {U"\u0261"}, // ɡ
                    {U"\u0047", U"\u0067"},  // {U"G", U"g"}
                    {U"\u0047", U"\u0067"},  // {U"G", U"g"}
                    {
                     U"\u06af",  // گ
                                U"\ufb92",  // ﮒ
                            U"",  //
                            U"\ufb94",  // ﮔ
                            U"",  //
                            U"\ufb95",  // ﮕ
                            U"",  //
                            U"\ufb93",  // ﮓ
                            U"",  //
                    },
                    {U"\u0413", U"\u0433"},  // {U"Г", U"г"}
            },

            LetterData{
                    {U"\u014b"}, // ŋ
                    {U"\u004e\u0067", U"\u006e\u0067"}, // {U"Ng", U"ng"}
                    {U"\u004e\u0067", U"\u006e\u0067"}, // {U"Ng", U"ng"}
                    {
                     U"\u06ad",  // ڭ
                                      U"\ufbd3",  // ﯓ
                            U"",  //
                            U"\ufbd5",  // ﯕ
                            U"",  //
                            U"\ufbd6",  // ﯖ
                            U"",  //
                            U"\ufbd4",  // ﯔ
                            U"",  //
                    },
                    {U"\u04a2",       U"\u04a3"},  // {U"Ң", U"ң"}
            },

            LetterData{
                    {U"\u006c"}, // l
                    {U"\u004c", U"\u006c"},  // {U"L", U"l"}
                    {U"\u004c", U"\u006c"},  // {U"L", U"l"}
                    {
                     U"\u0644",  // ل
                                U"\ufedd",  // ﻝ
                            U"",  //
                            U"\ufedf",  // ﻟ
                            U"",  //
                            U"\ufee0",  // ﻠ
                            U"",  //
                            U"\ufede",  // ﻞ
                            U"",  //
                    },
                    {U"\u041b", U"\u043b"},  // {U"Л", U"л"}
            },

            LetterData{
                    {U"\u006d"}, // m
                    {U"\u004d", U"\u006d"},  // {U"M", U"m"}
                    {U"\u004d", U"\u006d"},  // {U"M", U"m"}
                    {
                     U"\u0645",  // م
                                U"\ufee1",  // ﻡ
                            U"",  //
                            U"\ufee3",  // ﻣ
                            U"",  //
                            U"\ufee4",  // ﻤ
                            U"",  //
                            U"\ufee2",  // ﻢ
                            U"",  //
                    },
                    {U"\u041c", U"\u043c"},  // {U"М", U"м"}
            },

            LetterData{
                    {U"\u006e"}, // n
                    {U"\u004e", U"\u006e"},  // {U"N", U"n"}
                    {U"\u004e", U"\u006e"},  // {U"N", U"n"}
                    {
                     U"\u0646",  // ن
                                U"\ufee5",  // ﻥ
                            U"",  //
                            U"\ufee7",  // ﻧ
                            U"",  //
                            U"\ufee8",  // ﻨ
                            U"",  //
                            U"\ufee6",  // ﻦ
                            U"",  //
                    },
                    {U"\u041d", U"\u043d"},  // {"Н", U"н"}
            },

            LetterData{
                    {U"\u0068"}, // h
                    {U"\u0048", U"\u0068"},  // {"H", U"h"}
                    {U"\u2c67", U"\u2c68"},  // {"Ⱨ", U"ⱨ"}
                    {
                     U"\u06be",  // ھ
                                U"\ufbaa",  // ﮪ
                            U"",  //
                            U"\ufbac",  // ﮬ
                            U"",  //
                            U"\ufbad",  // ﮭ
                            U"",  //
                            U"\ufbab",  // ﮫ
                            U"",  //
                    },
                    {U"\u04ba", U"\u04bb"},  // {"Һ", U"һ"}
            },

            LetterData{
                    {U"\u0077", U"\u0076"}, // w
                    {U"\u0057", U"\u0077"},  // {"W", U"w"}
                    {U"\u0057", U"\u0077"},  // {"W", U"w"}
                    {
                     U"\u06cb",  // ۋ
                                U"\ufbde",  // ﯞ
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"",  //
                            U"\ufbdf",  // ﯟ
                            U"",  //
                    },
                    {U"\u0412", U"\u0432"},  // {"В", U"в"}
            },

            LetterData{
                    {U"\u006a"}, // j
                    {U"\u0059", U"\u0079"},  // {"Y", U"y"}
                    {U"\u0059", U"\u0079"},  // {"Y", U"y"}
                    {
                     U"\u064a",  // ي
                                U"\ufef1",  // ﻱ
                            U"",  //
                            U"\ufef3",  // ﻳ
                            U"",  //
                            U"\ufef4",  // ﻴ
                            U"",  //
                            U"\ufef2",  // ﻲ
                            U"",  //
                    },
                    {U"\u0419", U"\u0439"},  // {"Й", U"й"}
            }
    };
    return scripts;
}


map<u32string, u32string> getConvertLetterMap(int src_letter_type,int dst_letter_type) {
    map<u32string, LetterData> letterMap{};
    getLetterMap(letterMap,src_letter_type);
    map<u32string, u32string> result{};
    std::function get_result=[&](){
        std::function getKey=[&](const LetterData& value,bool is_sentence_start){
            switch (src_letter_type) {
                case LETTER_TYPE_UKY:
                    if(is_sentence_start)
                        return value.uky.sentence_start;
                    return value.uky.other;
                case LETTER_TYPE_ULY:
                    if(is_sentence_start)
                        return value.uly.sentence_start;
                    return value.uly.other;
                case LETTER_TYPE_UYY:
                    if(is_sentence_start)
                        return value.uyy.sentence_start;
                    return value.uyy.other;
                default:
                    throw std::invalid_argument("Invalid letter type");

            }
        };
        if(src_letter_type==LETTER_TYPE_IPA || src_letter_type==LETTER_TYPE_UEY){
            if(dst_letter_type == LETTER_TYPE_IPA){
                for(auto [key,value]:letterMap){
                    result[key]=value.ipa[0];
                }
            }else if(dst_letter_type == LETTER_TYPE_UEY){
                for(auto [key,value]:letterMap){
                    result[key]=value.uey.Base;
                }
            }else if(dst_letter_type == LETTER_TYPE_ULY){
                for(auto [key,value]:letterMap){
                    result[key]=value.uly.other;
                }
            }else if(dst_letter_type == LETTER_TYPE_UYY){
                for(auto [key,value]:letterMap){
                    result[key]=value.uyy.other;
                }
            }else if(dst_letter_type == LETTER_TYPE_UKY){
                for(auto [key,value]:letterMap){
                    result[key]=value.uky.other;
                }
            }else{
                throw std::invalid_argument("Invalid letter type");
            }
        }else{
            if(dst_letter_type == LETTER_TYPE_IPA){
                for(auto [key,value]:letterMap){
                    result[key]=value.ipa[0];
                }
            }else if(dst_letter_type == LETTER_TYPE_UEY){
                for(auto [key,value]:letterMap){
                    result[key]=value.uey.Base;
                }
            }else if(dst_letter_type == LETTER_TYPE_ULY){
                for(auto [key,value]:letterMap){
                    result[getKey(value,true)]=value.uly.sentence_start;
                    result[getKey(value,false)]=value.uly.other;
                }
            }else if(dst_letter_type == LETTER_TYPE_UYY){
                for(auto [key,value]:letterMap){
                    result[getKey(value,true)]=value.uly.sentence_start;
                    result[getKey(value,false)]=value.uly.other;
                }
            }else if(dst_letter_type == LETTER_TYPE_UKY){
                for(auto [key,value]:letterMap){
                    result[getKey(value,true)]=value.uly.sentence_start;
                    result[getKey(value,false)]=value.uly.other;
                }
            }else{
                throw std::invalid_argument("Invalid letter type");
            }
        }

    };
    get_result();
    return result;
}
