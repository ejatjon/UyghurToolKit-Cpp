//
// Created by EjatjonAmar on 2025/6/29.
//

#pragma once


#include <vector>
#include "macros.h"

template<typename T>
inline bool in_array(T* c,T* arr,int size){
    for (int i = 0; i < size; i++) {
        if (*c==arr[i])
            return true;
    }
    return false;
}

template <typename T>
concept String = requires(T a, const T& b, size_t pos, size_t count) {
    // 默认构造
    { T() } -> std::same_as<T>;

    // 支持 += 运算
    { a += b } -> std::same_as<T&>;

    // 查找操作
    { a.find(b) } -> std::convertible_to<size_t>;
    { a.find(b, pos) } -> std::convertible_to<size_t>;

    // 子串操作
    { a.substr(pos, count) } -> std::same_as<T>;
    { a.substr(pos) } -> std::same_as<T>;

    // 替换操作
    {a.replace(pos, count, b)} -> std::same_as<T&>;

    // 长度获取
    { a.size() } -> std::convertible_to<size_t>;
    { a.length() } -> std::same_as<size_t>;

    // 空状态检查
    { a.empty() } -> std::convertible_to<bool>;

    // 静态常量 npos
    { T::npos } -> std::convertible_to<size_t>;

} &&
                    // 确保有 value_type 成员
                    requires { typename T::value_type; } &&
                    // 可复制构造
                    std::copy_constructible<T>;


// 字符串连接
template<String T>
inline T join_strings(const std::vector<T>& strings, T delimiter) {
    if (strings.empty()) return T();

    T result;
    for (size_t i = 0; i < strings.size(); ++i) {
        if (i > 0) result += delimiter;
        result += strings[i];
    }
    return result;
}

// 子字符串替换
template<String T>
inline void replace_substring(T& str,
                       const T& from,
                       const T& to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::u32string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
}

// 字符串分割
template<String T>
inline std::vector<T> split_string(const T& str, T delimiter) {
    std::vector<T> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    T word{};
    while (end != T::npos) {
        word=str.substr(start, end - start);
        if (!word.empty())
            tokens.push_back(word);
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    if (start < str.length())
        tokens.push_back(str.substr(start));

    return tokens;
}


/**
将多个空格替换成一个空格
:@param text:
:@return: 处理后的字符串
*/
inline std::u32string replace_whitespace(const std::u32string& text){
    std::u32string new_text;
    bool is_space = false;
    for(const char32_t& c: text){
        if (is_space && c == U' '){
            continue;
        }
        if(c == U' '){
            is_space = true;
        }
        new_text+=c;
    }
    return new_text;
}
