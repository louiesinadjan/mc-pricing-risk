# Compiler warning settings

if(NOT MSVC)
    add_compile_options(
        -Wdouble-promotion
        -Wformat=2
        -Wnull-dereference
        -Wshadow
        -Wunused-parameter
        -Wconversion
    )
endif()
