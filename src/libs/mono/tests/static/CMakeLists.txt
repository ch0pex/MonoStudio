
add_library(mono-lib-static-tests STATIC
        specialization_of.cpp
)

target_link_libraries(mono-lib-static-tests PRIVATE mono::mono-lib)
