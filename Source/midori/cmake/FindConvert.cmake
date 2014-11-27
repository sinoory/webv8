# Copyright (C) 2013 Christian Dywan
# Copyright (C) 2013 Olivier Duchateau
# 将应用/data下的svg文件转换成png图片，定制将其生成在bin/res下，安装目录待定制

find_program (RSVG_CONVERT rsvg-convert)

if (RSVG_CONVERT)
    set (CONVERT_FOUND TRUE)
    macro (SVG2PNG filename install_destination)
        string(REPLACE "/" "_" target ${filename})
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${filename}")
        file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res/${filename}")
        message(STATUS "ZRL midori CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}. filename = ${filename} CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR} install_destination = ${install_destination} CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
# ZRL Make all resources into bin/res path.
#        add_custom_target ("${target}.png" ALL
#            ${RSVG_CONVERT} --keep-aspect-ratio --format=png "${CMAKE_CURRENT_SOURCE_DIR}/${filename}.svg"
#                --output "${CMAKE_CURRENT_BINARY_DIR}/${filename}.png"
#        )
#        install (FILES "${CMAKE_CURRENT_BINARY_DIR}/${filename}.png"
#            DESTINATION ${install_destination})
        add_custom_target ("${target}.png" ALL
            ${RSVG_CONVERT} --keep-aspect-ratio --format=png "${CMAKE_CURRENT_SOURCE_DIR}/${filename}.svg"
                --output "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res/${filename}.png"
        )
        install (FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res/${filename}.png"
            DESTINATION ${install_destination})
    endmacro (SVG2PNG filename)
endif ()

