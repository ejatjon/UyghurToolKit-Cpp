//
// Created by EjatjonAmar on 2025/7/2.
//

#pragma once

#include <vector>
#include "processor.h"
#include "tools/date.h"

/**
 * @brief 默认数字处理器
 *
 * 用于处理文本中的整数和小数，支持根据配置排除特定前后缀的数字。
 *
 * 公共成员变量：
 * - int_exclude_before: 排除整数前缀列表
 * - int_exclude_after: 排除整数后缀列表
 * - decimal_exclude_before: 排除小数前缀列表
 * - decimal_exclude_after: 排除小数后缀列表
 */
class DefaultNumberProcessor : public Processor{
public:
    explicit DefaultNumberProcessor(ProcesClass&& p_class);
    DefaultNumberProcessor(ProcesClass &&p_class,std::vector<std::u32string> exclude_before,
                           std::vector<std::u32string> exclude_after);
    DefaultNumberProcessor(ProcesClass &&p_class,std::vector<std::u32string> int_exclude_before,
                           std::vector<std::u32string> int_exclude_after,
                           std::vector<std::u32string> decimal_exclude_before,
                           std::vector<std::u32string> decimal_exclude_after);


    std::u32string process(std::u32string text) override;
    std::u32string reverse(std::u32string text) override;

public:
    // 要排除特殊的的前后缀，如添加前后缀“-” 则2025-6-1这种数将会被忽略
    std::vector<std::u32string> int_exclude_before{},int_exclude_after{};
    std::vector<std::u32string> decimal_exclude_before{},decimal_exclude_after{};
};

/**
 * @brief 默认音节分割处理器
 *
 * 用于将文本按音节划分规则（详情见自述文件）进行分割。
 * 公共成员变量：
 @param split_vowels: 是否切割两个并列元音(VV)。如果文字中包含了太多汉语借词时可以设置成False
 @param delimiter: 词之间的分隔符
 */
class DefaultDivideSyllablesProcessor : public Processor{
public:
    explicit DefaultDivideSyllablesProcessor(ProcesClass&& p_class);
    DefaultDivideSyllablesProcessor(ProcesClass &&p_class,bool split_vowel,std::u32string&& delimiter);
    std::u32string process(std::u32string text) override;
    std::u32string reverse(std::u32string text) override; //当delimiter不是为空字符或空格时，reverse()方法才会返回正确的结果
public:
    bool split_vowel= true;
    std::u32string delimiter=U" ";
};

/**
 * @brief 默认归一化处理器
 *
 * 用于对文本进行标准化处理，如统一字符形式。
 * 无额外配置参数，构造函数仅接收处理器类别信息。
 */
class DefaultNormalizeProcessor : public Processor{
public:
    explicit DefaultNormalizeProcessor(ProcesClass&& p_class);
    std::u32string process(std::u32string text) override;
    std::u32string reverse(std::u32string text) override;
};

/**
 * @brief 默认日期处理器
 * 用于识别和处理文本中的日期格式，支持多种日期格式转换。
 * 无额外配置参数，构造函数仅接收处理器类别信息。
 */
class DefaultDateProcessor : public Processor{
public:
    explicit DefaultDateProcessor(ProcesClass&& p_class,const std::vector<DateRegexInfo> info);
    std::u32string process(std::u32string text) override;
private:
    std::u32string reverse(std::u32string text) override;
public:
    const std::vector<DateRegexInfo> info{};
    int letter_type=0;
};

/**
 * @brief 默认符号处理器
 * 用于处理文本中的特殊符号，如物理、数学符号等。
 * 无额外配置参数，构造函数仅接收处理器类别信息。
 */
class DefaultSymbolProcessor : public Processor{
public:
    explicit DefaultSymbolProcessor(ProcesClass&& p_class,bool split_vowel=true,std::u32string delimiter=U" ");
    std::u32string process(std::u32string text) override;
    std::u32string reverse(std::u32string text) override;
public:
    bool split_vowel= true;
    std::u32string delimiter=U" ";
};

/**
 * @brief 默认字母转换处理器
 * 用于在不同字母系统之间(UEY,ULY,UYY,UKY,IPA)进行转换。
 * 无额外配置参数，构造函数仅接收处理器类别信息。
 */
class DefaultLetterConversionProcessor : public Processor{
public:
    explicit DefaultLetterConversionProcessor(ProcesClass&& p_class,int src_letter_type,int dst_letter_type);
    std::u32string process(std::u32string text) override;
    std::u32string reverse(std::u32string text) override;
public:
    int src_letter_type=0;
    int dst_letter_type=0;
};

