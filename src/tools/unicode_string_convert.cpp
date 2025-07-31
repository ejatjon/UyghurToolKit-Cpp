//
// Created by EjatjonAmar on 2025/6/28.
//

#include <vector>
#include "tools/unicode_string_convert.h"

CORE_API std::string utf32_to_utf8(const std::u32string& str) {
    std::string result;

    for(char32_t c : str) {
        // 处理 ASCII (1字节)
        if(c <= 0x7F) {
            result += static_cast<char>(c);
        }
            // 2字节序列
        else if(c <= 0x7FF) {
            result += static_cast<char>(0xC0 | ((c >> 6) & 0x1F));
            result += static_cast<char>(0x80 | (c & 0x3F));
        }
            // 3字节序列
        else if(c <= 0xFFFF) {
            result += static_cast<char>(0xE0 | ((c >> 12) & 0x0F));
            result += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
            result += static_cast<char>(0x80 | (c & 0x3F));
        }
            // 4字节序列 (U+10000 及以上)
        else if(c <= 0x10FFFF) {
            result += static_cast<char>(0xF0 | ((c >> 18) & 0x07));
            result += static_cast<char>(0x80 | ((c >> 12) & 0x3F));
            result += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
            result += static_cast<char>(0x80 | (c & 0x3F));
        }
            // 无效 Unicode 替换字符
        else {
            result += "\xEF\xBF\xBD"; // U+FFFD REPLACEMENT CHARACTER
        }
    }

    return result;
}
CORE_API std::u16string utf32_to_utf16(const std::u32string& input) {
    std::u16string output;
    output.reserve(input.size()); // 预分配空间（实际可能更多）

    for (char32_t code_point : input) {
        // 处理 BMP（基本多语言平面）字符 (U+0000 到 U+D7FF 和 U+E000 到 U+FFFF)
        if (code_point <= 0xFFFF) {
            // 检查代理区无效字符
            if (code_point >= 0xD800 && code_point <= 0xDFFF) {
                // 替换为 Unicode 替换字符 U+FFFD
                output.push_back(0xFFFD);
            } else {
                output.push_back(static_cast<char16_t>(code_point));
            }
        }
            // 处理辅助平面字符 (U+10000 到 U+10FFFF)
        else if (code_point <= 0x10FFFF) {
            // 计算代理对
            code_point -= 0x10000;
            auto high_surrogate = static_cast<char16_t>((code_point >> 10) + 0xD800);
            auto low_surrogate = static_cast<char16_t>((code_point & 0x3FF) + 0xDC00);

            output.push_back(high_surrogate);
            output.push_back(low_surrogate);
        }
            // 处理无效 Unicode 码点
        else {
            output.push_back(0xFFFD); // Unicode 替换字符
        }
    }

    return output;
}
CORE_API std::u32string utf16_to_utf32(const std::u16string& input) {
    std::u32string output;
    output.reserve(input.size()); // 预分配空间

    for (size_t i = 0; i < input.size(); ++i) {
        char16_t c = input[i];

        // 处理基本多语言平面 (BMP) 字符
        if (c < 0xD800 || c > 0xDFFF) {
            output.push_back(static_cast<char32_t>(c));
        }
            // 处理高位代理 (high surrogate)
        else if (c <= 0xDBFF) {
            // 检查是否有配对的低位代理
            if (i + 1 >= input.size()) {
                // 不完整代理对
                output.push_back(0xFFFD); // 替换字符
                break;
            }

            char16_t next = input[i + 1];
            // 检查低位代理是否有效
            if (next >= 0xDC00 && next <= 0xDFFF) {
                // 计算完整码点
                char32_t code_point = 0x10000;
                code_point += (static_cast<char32_t>(c) - 0xD800) << 10;
                code_point += static_cast<char32_t>(next) - 0xDC00;
                output.push_back(code_point);
                ++i; // 跳过低位代理
            } else {
                // 无效的低位代理
                output.push_back(0xFFFD);
            }
        }
            // 孤立的低位代理 (无效)
        else {
            output.push_back(0xFFFD);
        }
    }

    return output;
}
CORE_API std::string utf16_to_utf8(const std::u16string& input) {
    std::string output;
    // 最大可能需要4倍空间（每个UTF-16字符最多转为4字节UTF-8）
    output.reserve(input.size() * 4);

    auto encode_utf8 = [&](char32_t code_point) {
        if (code_point <= 0x7F) {
            // 1字节序列
            output.push_back(static_cast<char>(code_point));
        } else if (code_point <= 0x7FF) {
            // 2字节序列
            output.push_back(static_cast<char>(0xC0 | (code_point >> 6)));
            output.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
        } else if (code_point <= 0xFFFF) {
            // 3字节序列
            output.push_back(static_cast<char>(0xE0 | (code_point >> 12)));
            output.push_back(static_cast<char>(0x80 | ((code_point >> 6) & 0x3F)));
            output.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
        } else if (code_point <= 0x10FFFF) {
            // 4字节序列
            output.push_back(static_cast<char>(0xF0 | (code_point >> 18)));
            output.push_back(static_cast<char>(0x80 | ((code_point >> 12) & 0x3F)));
            output.push_back(static_cast<char>(0x80 | ((code_point >> 6) & 0x3F)));
            output.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
        } else {
            // 无效码点 u8"\uFFFD"
            output.append("\xEF\xBF\xBD");
        }
    };

    for (size_t i = 0; i < input.size(); ++i) {
        char16_t c = input[i];

        if (c < 0xD800 || c > 0xDFFF) {
            // BMP字符
            encode_utf8(static_cast<char32_t>(c));
        } else if (c <= 0xDBFF) {
            // 高位代理
            if (i + 1 < input.size()) {
                char16_t next = input[i + 1];
                if (next >= 0xDC00 && next <= 0xDFFF) {
                    // 计算完整码点
                    char32_t code_point = 0x10000;
                    code_point += (static_cast<char32_t>(c) - 0xD800) << 10;
                    code_point += static_cast<char32_t>(next) - 0xDC00;
                    encode_utf8(code_point);
                    ++i; // 跳过低位代理
                    continue;
                }
            }
            // 无效代理对
            encode_utf8(0xFFFD);
        } else {
            // 孤立的低位代理
            encode_utf8(0xFFFD);
        }
    }

    return output;
}
CORE_API std::u16string utf8_to_utf16(const std::string& input) {
    std::u16string output;
    output.reserve(input.size()); // 最小预分配

    for (size_t i = 0; i < input.size(); ) {
        auto c = static_cast<unsigned char>(input[i]);

        // 解码UTF-8序列
        char32_t code_point = 0;
        int bytes = 0;

        if (c < 0x80) {
            // 1字节序列 (0xxxxxxx)
            code_point = c;
            bytes = 1;
        } else if ((c & 0xE0) == 0xC0) {
            // 2字节序列 (110xxxxx)
            if (i + 1 >= input.size()) {
                output.push_back(0xFFFD);
                break;
            }
            code_point = (c & 0x1F) << 6;
            code_point |= (input[i + 1] & 0x3F);
            bytes = 2;
        } else if ((c & 0xF0) == 0xE0) {
            // 3字节序列 (1110xxxx)
            if (i + 2 >= input.size()) {
                output.push_back(0xFFFD);
                break;
            }
            code_point = (c & 0x0F) << 12;
            code_point |= (input[i + 1] & 0x3F) << 6;
            code_point |= (input[i + 2] & 0x3F);
            bytes = 3;
        } else if ((c & 0xF8) == 0xF0) {
            // 4字节序列 (11110xxx)
            if (i + 3 >= input.size()) {
                output.push_back(0xFFFD);
                break;
            }
            code_point = (c & 0x07) << 18;
            code_point |= (input[i + 1] & 0x3F) << 12;
            code_point |= (input[i + 2] & 0x3F) << 6;
            code_point |= (input[i + 3] & 0x3F);
            bytes = 4;
        } else {
            // 无效UTF-8起始字节
            output.push_back(0xFFFD);
            ++i;
            continue;
        }

        // 验证连续字节
        for (int j = 1; j < bytes; ++j) {
            if ((input[i + j] & 0xC0) != 0x80) {
                // 无效连续字节
                code_point = 0xFFFD;
                break;
            }
        }

        // 移动到下一个序列
        i += bytes;

        // 编码为UTF-16
        if (code_point <= 0xFFFF) {
            // 检查代理区无效字符
            if (code_point >= 0xD800 && code_point <= 0xDFFF) {
                output.push_back(0xFFFD);
            } else {
                output.push_back(static_cast<char16_t>(code_point));
            }
        } else if (code_point <= 0x10FFFF) {
            // 转换为代理对
            code_point -= 0x10000;
            char16_t high_surrogate = 0xD800 + (code_point >> 10);
            char16_t low_surrogate = 0xDC00 + (code_point & 0x3FF);
            output.push_back(high_surrogate);
            output.push_back(low_surrogate);
        } else {
            // 无效码点
            output.push_back(0xFFFD);
        }
    }

    return output;
}
CORE_API std::u32string utf8_to_utf32(const std::string& input) {
    std::u32string output;
    output.reserve(input.size()); // 最小预分配

    for (size_t i = 0; i < input.size(); ) {
        auto c = static_cast<unsigned char>(input[i]);
        char32_t code_point = 0;
        int bytes = 0;

        if (c < 0x80) {
            // 1字节序列
            code_point = c;
            bytes = 1;
        } else if ((c & 0xE0) == 0xC0) {
            // 2字节序列
            if (i + 1 >= input.size()) {
                output.push_back(0xFFFD);
                break;
            }
            code_point = (c & 0x1F) << 6;
            code_point |= (input[i + 1] & 0x3F);
            bytes = 2;
        } else if ((c & 0xF0) == 0xE0) {
            // 3字节序列
            if (i + 2 >= input.size()) {
                output.push_back(0xFFFD);
                break;
            }
            code_point = (c & 0x0F) << 12;
            code_point |= (input[i + 1] & 0x3F) << 6;
            code_point |= (input[i + 2] & 0x3F);
            bytes = 3;
        } else if ((c & 0xF8) == 0xF0) {
            // 4字节序列
            if (i + 3 >= input.size()) {
                output.push_back(0xFFFD);
                break;
            }
            code_point = (c & 0x07) << 18;
            code_point |= (input[i + 1] & 0x3F) << 12;
            code_point |= (input[i + 2] & 0x3F) << 6;
            code_point |= (input[i + 3] & 0x3F);
            bytes = 4;
        } else {
            // 无效UTF-8起始字节
            output.push_back(0xFFFD);
            ++i;
            continue;
        }

        // 验证连续字节
        bool valid = true;
        for (int j = 1; j < bytes; ++j) {
            if ((input[i + j] & 0xC0) != 0x80) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            // 无效序列
            output.push_back(0xFFFD);
            i += bytes; // 跳过无效字节
            continue;
        }

        // 检查过短编码（非最短形式）
        if ((bytes == 2 && code_point < 0x80) ||
            (bytes == 3 && code_point < 0x800) ||
            (bytes == 4 && code_point < 0x10000))
        {
            output.push_back(0xFFFD);
        }
            // 检查有效Unicode范围
        else if (code_point > 0x10FFFF) {
            output.push_back(0xFFFD);
        }
            // 检查代理区码点
        else if (code_point >= 0xD800 && code_point <= 0xDFFF) {
            output.push_back(0xFFFD);
        }
        else {
            output.push_back(code_point);
        }

        i += bytes;
    }

    return output;
}

std::string u8str_to_utf8(const std::u8string &input) {
    return std::string(reinterpret_cast<const char*>(input.data()), input.size());
}
