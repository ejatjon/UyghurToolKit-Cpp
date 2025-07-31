//
// Created by EjatjonAmar on 2025/7/3.
//

#include <map>
#include "date.h"
#include "number.h"
#include "resources/dictionaries/date.h"
#include "text.h"


std::regex mk_date_regex(const std::vector<DateRegexInfo> &info) {
    std::string regex;
    for (auto &i: info) {
        std::string r = "(" + i.regex + ")";
        if (regex.empty())
            regex = r;
        else
            regex += "|" + r;
    }
    return std::regex(regex);
}

std::string default_date_time_to_string(Date date, Time time, int letter_type) {
    // 将时间转换为字符串
    if (date.is_empty() && time.is_empty())
        return "";
    std::u32string result;
    if (!date.is_empty()) {
        std::u32string number{};
        if (!date.year.empty()) {
            number=integer_number_to_string(
                    utf8_to_utf32(date.year), letter_type);
            number.pop_back();
            result += U" " + number;
            result = number_hyphen_converter(result, letter_type);
            result += U" " + data_prefix_suffix_map["year_unit"][letter_type - 1];
        }
        if (!date.month.empty()) {
            number=integer_number_to_string(
                    utf8_to_utf32(date.month), letter_type);
            number.pop_back();
            result += U" " + number;
            result = number_hyphen_converter(result, letter_type);
            result += U" " + data_prefix_suffix_map["month_unit"][letter_type - 1];
        }

        if (!date.day.empty()) {
            number=integer_number_to_string(
                    utf8_to_utf32(date.day), letter_type);
            number.pop_back();
            result += U" " + number;
            result = number_hyphen_converter(result, letter_type);
            result += U" " + data_prefix_suffix_map["day_unit"][letter_type - 1];
        }
    }
    if (!time.is_empty()) {
        if (!result.empty())
            result += U" " + data_prefix_suffix_map["hour_unit"][letter_type - 1];
        if (!time.hour.empty()) {
            result += U" " + integer_number_to_string(
                    utf8_to_utf32(time.hour), letter_type);
        }
        if (!time.minute.empty()) {
            result += data_prefix_suffix_map["hour_adverb"][letter_type - 1];
            result += U" " + integer_number_to_string(
                    utf8_to_utf32(time.minute), letter_type);
            result += data_prefix_suffix_map["minute_unit"][letter_type - 1];
        }
        if (!time.second.empty()) {
            result += U" " + integer_number_to_string(
                    utf8_to_utf32(time.second), letter_type);
            result += data_prefix_suffix_map["second_unit"][letter_type - 1];
        }
        if (!time.millisecond.empty()) {
            result += U" " + integer_number_to_string(
                    utf8_to_utf32(time.millisecond), letter_type);
            result += data_prefix_suffix_map["millisecond_unit"][letter_type - 1];
        }
        result += U" " + data_prefix_suffix_map["time_passes"][letter_type - 1];
    }


    return utf32_to_utf8(result);
}

std::vector<DateRegexInfo> get_default_date_regex_info() {
    std::vector<DateRegexInfo> date_regex_list;

    // YYYY{-/.}MM-DD HH:MM:SS,  格式 (例如: 2025-07-03 12:30:12, 2025/07/03 12:30:12, 2025.07.03 12:30:12)
    // 支持分隔符左右有空格的情况
    date_regex_list.push_back({
                                      R"(\b\d{4}\s*[-/.]\s*\d{1,2}\s*[-/.]\s*\d{1,2}\s+\d{1,2}:\d{1,2}:\d{1,2}\b)",
                                      [](const std::sub_match<std::string::const_iterator> &m) -> std::tuple<Date, Time> {
                                          std::string text = m.str();
                                          Date date;
                                          Time time;
                                          std::regex regex(
                                                  R"(\b(\d{4})\s*[-/.]\s*(\d{1,2})\s*[-/.]\s*(\d{1,2})\s+(\d{1,2}):(\d{1,2}):(\d{1,2})\b)");
                                          std::smatch match;
                                          std::regex_match(text, match, regex);
                                          date.year = match[1].str();
                                          date.month = match[2].str();
                                          date.day = match[3].str();
                                          time.hour = match[4].str();
                                          time.minute = match[5].str();
                                          time.second = match[6].str();
                                          return std::make_tuple(date, time);
                                      },
                                      [](const std::tuple<Date, Time> &date_time, int letter_type) -> std::string {
                                          auto [data, time] = date_time;
                                          return default_date_time_to_string(data, time, letter_type);
                                      }
                              });
    date_regex_list.push_back({
                                      R"(\b\d{4}\s*[-/.]\s*\d{1,2}\s*[-/.]\s*\d{1,2}\s+\d{1,2}:\d{1,2}\b)",
                                      [](const std::sub_match<std::string::const_iterator> &m) -> std::tuple<Date, Time> {
                                          std::string text = m.str();
                                          Date date;
                                          Time time;
                                          std::regex regex(
                                                  R"(\b(\d{4})\s*[-/.]\s*(\d{1,2})\s*[-/.]\s*(\d{1,2})\s+(\d{1,2}):(\d{1,2})\b)");
                                          std::smatch match;
                                          std::regex_match(text, match, regex);
                                          date.year = match[1].str();
                                          date.month = match[2].str();
                                          date.day = match[3].str();
                                          time.hour = match[4].str();
                                          time.minute = match[5].str();
                                          return std::make_tuple(date, time);
                                      },
                                      [](const std::tuple<Date, Time> &date_time, int letter_type) -> std::string {
                                          auto [data, time] = date_time;
                                          return default_date_time_to_string(data, time, letter_type);
                                      }
                              });
    date_regex_list.push_back({
                                      R"(\b\d{4}\s*[-/.]\s*\d{1,2}\s*[-/.]\s*\d{1,2}\b)",
                                      [](const std::sub_match<std::string::const_iterator> &m) -> std::tuple<Date, Time> {
                                          std::string text = m.str();
                                          Date date;
                                          Time time;
                                          std::regex regex(R"(\b(\d{4})\s*[-/.]\s*(\d{1,2})\s*[-/.]\s*(\d{1,2})\b)");
                                          std::smatch match;
                                          std::regex_match(text, match, regex);
                                          date.year = match[1].str();
                                          date.month = match[2].str();
                                          date.day = match[3].str();
                                          time.hour = match[4].str();
                                          time.minute = match[5].str();
                                          return std::make_tuple(date, time);
                                      },
                                      [](const std::tuple<Date, Time> &date_time, int letter_type) -> std::string {
                                          auto [data, time] = date_time;
                                          return default_date_time_to_string(data, time, letter_type);
                                      }
                              });

    return date_regex_list;
}

std::string process_date(std::string text,const std::vector<DateRegexInfo> &info, int letter_type) {
    if (letter_type < 1 || letter_type > 5)
        return text;

    auto regex = mk_date_regex(info);
    auto begin = std::sregex_iterator(text.begin(), text.end(), regex);
    auto end = std::sregex_iterator();
    std::smatch match;
    std::string result;
    for (auto i = begin; i != end; i++) {
        std::string_view text_view = std::string_view(text.data() + match.position() + match.length(),
                                                      i->position() - match.position() - match.length());
        result.append(text_view);
        match = *i;
        for (int index = 0; index < match.size(); index++) {
            if (index == 0)
                continue;
            if (match[index].matched) {
                const std::sub_match<std::string::const_iterator> &sm = match[index];
                result += info[index - 1].to_string(info[index - 1].handler(sm), letter_type);
                break;
            }
        }
    }
    result.append(text.data() + match.position() + match.length(), text.size() - match.position() - match.length());
    return result;
}



