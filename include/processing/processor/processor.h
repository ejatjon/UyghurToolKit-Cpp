//
// Created by EjatjonAmar on 2025/6/29.
//
#pragma once

#include <string>
#include <utility>
#include "tools/other.h"
#include "macros.h"

/**
   文本处理操作类型枚举
   使用位运算组合多个处理类型（如: NORMALIZATION | REVERSE）
   每个枚举值代表不同的文本处理操作
 */
enum class ProcesType {
    UNDEFINED = 0,           // 未定义操作
    NORMALIZATION = BIT(0),  //: 文本标准化处理（统一字符形式）
    NUMBER = BIT(1),         //: 数字格式转换（阿拉伯数字↔维文数字）
    SYMBOL = BIT(2),         //: 符号格式转换（标点/货币符号等）
    DATE = BIT(3),           //: 日期格式转换（数字公历<->维文日期）
    SYLLABLES = BIT(4),      //: 划分音节
    CONVERSION = BIT(5)      //: 字符类型的转换（UEY <-> ULY）
};

inline int operator|(ProcesType lhs, ProcesType rhs)
{
    return static_cast<int>(lhs) | static_cast<int>(rhs);
}

inline int operator&(ProcesType lhs, ProcesType rhs)
{
    return static_cast<int>(lhs) & static_cast<int>(rhs);
}


/**
 * @brief 枚举类：文本编码格式标准枚举
 */
enum class ProcesClass {
    UNDEFINED=0,UEY,ULY,UYY,UKY,IPA
};

class Processor {
/**
    处理器抽象基类，为具体处理器提供统一接口
            职责：
    1. 定义处理器的标准调用接口
    2. 封装处理器的基础属性和方法

            参数：
    p_class: 处理器所属分类（ProcesClass枚举）
    p_type: 处理器类型标识符
            name:   处理器名称

            关键方法：
    process(): 抽象方法，需在子类实现具体处理逻辑
**/
public:
    Processor(ProcesClass&& p_class,int&& p_type,std::string name):m_class(p_class),m_type(p_type),m_name(std::move(name)){}
    Processor(ProcesClass&& p_class,ProcesType&& p_type,std::string name):m_class(p_class),m_type(static_cast<int>(p_type)),m_name(std::move(name)){}
    Processor(ProcesClass p_class,int&& p_type,std::string name):m_class(p_class),m_type(p_type),m_name(std::move(name)){}
    Processor(ProcesClass p_class,ProcesType&& p_type,std::string name):m_class(p_class),m_type(static_cast<int>(p_type)),m_name(std::move(name)){}


    /**
     * 执行处理操作，需在子类实现具体逻辑
     * @param text 需要处理的文本
     * @return 处理结果（由子类实现决定）
     */
    virtual std::u32string process(std::u32string text)=0;

    /**
     * 执行还原操作，需在子类实现具体逻辑
     * @param text 需要处理的文本
     * @return 处理结果（由子类实现决定）
     */
    virtual std::u32string reverse(std::u32string text)=0;


    std::string getName(){
        return m_name;
    }

    ProcesClass getClass(){
        return m_class;
    }

    int getType() const{
        return m_type;
    }

private:
    int m_type= static_cast<int>(ProcesType::UNDEFINED);
    ProcesClass m_class=ProcesClass::UNDEFINED;
    std::string m_name{};
};


