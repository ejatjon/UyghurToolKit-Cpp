//
// Created by EjatjonAmar on 2025/6/28.
//

#pragma once
#include <string>
#include "macros.h"


CORE_API std::string utf32_to_utf8(const std::u32string& str);
CORE_API std::u16string utf32_to_utf16(const std::u32string& input);
CORE_API std::u32string utf16_to_utf32(const std::u16string& input);
CORE_API std::string utf16_to_utf8(const std::u16string& input);
CORE_API std::u16string utf8_to_utf16(const std::string& input);
CORE_API std::u32string utf8_to_utf32(const std::string& input);
CORE_API std::string u8str_to_utf8(const std::u8string& input);

