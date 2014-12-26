# Copyright (C) 2013 Christian Dywan <christian@twotoasts.de>

macro(install_icon name cat size)
    file(GLOB FILENAME RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${size}/${name}.*")
    install(FILES ${FILENAME} DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/${size}/${cat})
endmacro()

foreach(SIZE 16x16 22x22 32x32 128x128 256x256)
    install_icon(${MIDORI_PROJECT_NAME} apps ${SIZE})
    install_icon(extension categories ${SIZE})
    install_icon(bookmarked categories ${SIZE})
    install_icon(unbookmark categories ${SIZE})
    install_icon(authensite categories ${SIZE})
    install_icon(certifiedwebsite categories ${SIZE})
endforeach()

