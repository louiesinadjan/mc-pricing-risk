# Address and Undefined Behavior Sanitizers

option(ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)

if(ENABLE_SANITIZERS)
    if(NOT MSVC)
        add_compile_options(-fsanitize=address -fsanitize=undefined)
        add_link_options(-fsanitize=address -fsanitize=undefined)
    endif()
endif()
