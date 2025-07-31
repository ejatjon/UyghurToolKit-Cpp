//
// Created by EjatjonAmar on 2025/6/27.
//

#pragma once
#define LETTER_TYPE_UEY 1
#define LETTER_TYPE_ULY 2
#define LETTER_TYPE_UYY 3
#define LETTER_TYPE_UKY 4
#define LETTER_TYPE_IPA 5

#define BIT(x) (1 << x)

#ifdef _WIN32
#define API_EXPORT __declspec(dllexport)
#define API_IMPORT __declspec(dllimport)
#else
#define API_EXPORT __attribute__((visibility('default')))
#define API_IMPORT
#endif

#ifdef DLL_EXPORT
#ifdef CORE_EXPORT
#define CORE_API API_EXPORT
#else
#define CORE_API API_IMPORT
#endif
#else
#define CORE_API
#endif






