idf_component_register(SRCS "main.cpp"
                       INCLUDE_DIRS ".")

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wdeprecated-declarations)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wcpp)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-variable)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wmissing-field-initializers)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wconversion-null)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-format-truncation)

