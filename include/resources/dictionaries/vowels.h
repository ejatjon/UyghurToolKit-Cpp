//
// Created by EjatjonAmar on 2025/6/29.
//

#pragma once


//维语一共8个元音，IPA中的 ئى 和 ئە 对应着两个音标因此IPA_VOWELS 多了两个元素

const char32_t IPA_VOWELS[] = {U'\u0251', U'\u025b', U'\u00e6', U'\u006f', U'\u0075', U'\u00f8', U'\u0079', U'\u0065', U'\u0069', U'\u0268'};
const char32_t UEY_VOWELS[] = {U'\u0627', U'\u06d5', U'\u0648', U'\u06c7', U'\u06c6', U'\u06c8', U'\u06d0', U'\u0649'};
const char32_t ULY_VOWELS[] = {U'\u0061', U'\u0065', U'\u006f', U'\u0075', U'\u00f6', U'\u00fc', U'\u00eb', U'\u0069'};
const char32_t UYY_VOWELS[] = {U'\u0061', U'\u0259', U'\u006f', U'\u0075', U'\u0275', U'\u00fc', U'\u0065', U'\u0069'};
const char32_t UKY_VOWELS[] = {U'\u0430', U'\u04d9', U'\u043e', U'\u0443', U'\u04e9', U'\u04af', U'\u0435', U'\u0438'};