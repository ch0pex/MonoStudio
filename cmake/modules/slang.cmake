function(add_slang_shader_target TARGET)
    cmake_parse_arguments("SHADER" "" "" "SOURCES" ${ARGN})
    set(SHADERS_DIR ${CMAKE_CURRENT_LIST_DIR}/assets/shaders)
    set(ENTRY_POINTS -entry vertMain -entry fragMain)

    if (WIN32)
        set(SLANG_COMPILER "slangc.exe")
    else ()
        set(SLANG_COMPILER "slangc")
    endif ()

    add_custom_command(
            OUTPUT ${SHADERS_DIR}/bin/${TARGET}.spv
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DIR}/bin
            COMMAND ${SLANG_COMPILER} ${SHADER_SOURCES} -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name ${ENTRY_POINTS} -o ${SHADERS_DIR}/bin/${TARGET}.spv

            WORKING_DIRECTORY ${SHADERS_DIR}
            DEPENDS ${SHADER_SOURCES}
            COMMENT "Compiling Slang Shaders (${TARGET})"
            VERBATIM
    )
    add_custom_target(${TARGET} DEPENDS ${SHADERS_DIR}/bin/${TARGET}.spv)
endfunction()