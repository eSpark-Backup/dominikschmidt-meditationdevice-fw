idf_component_register(SRCS "main.cpp"
                       INCLUDE_DIRS ".")

# Suppress warnings for C++ specifically
target_compile_options(${COMPONENT_LIB} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wno-error
        -Wdeprecated-declarations
        -Wcpp
        -Wno-unused-variable
        -Wno-missing-field-initializers
        -Wno-format-truncation
        -Wno-c++11-narrowing
        -Wno-narrowing
        -Wno-conversion-null
    >
)
