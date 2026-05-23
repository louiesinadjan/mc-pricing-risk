# Compiler options for optimization and features

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

# Optimization flags
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        add_compile_options(/O2)
    else()
        add_compile_options(-O3 -march=native)
    endif()
endif()

# Profile build: optimized but with frame pointers + debug symbols for perf/eBPF stack unwinding
if(CMAKE_BUILD_TYPE STREQUAL "Profile")
    add_compile_options(-O2 -g -fno-omit-frame-pointer -march=native)
endif()
