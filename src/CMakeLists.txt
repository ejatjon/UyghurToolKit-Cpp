
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")

set(SOURCES resources/dictionaries/scripts.cpp
        tools/unicode_string_convert.cpp
        resources/dictionaries/letter_normalize.cpp
        tools/number.cpp
        tools/text.cpp
        processing/processor/default_processors.cpp
        processing/processor/default_processors.h
        tools/date.cpp
        tools/date.h
        resources/dictionaries/date.h
)
add_library(core SHARED ${SOURCES})
target_include_directories(core PUBLIC .)
target_compile_definitions(core PRIVATE CORE_EXPORT)

install(TARGETS core DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

set_target_properties(core PROPERTIES
        VERSION 1.0.0
        SOVERSION 1
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)  # 检测 64 位环境
    target_compile_definitions(core PRIVATE
            _WIN64
            _AMD64_
    )
endif()