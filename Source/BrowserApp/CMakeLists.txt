# Created by ZRL for building Browser App 

set(BROWSERAPP_DIR "${CMAKE_SOURCE_DIR}/Source/BrowserApp")

set(DERIVED_SOURCES_BROWSERAPP_DIR "${CMAKE_BINARY_DIR}/DerivedSources/BrowserApp")

file(MAKE_DIRECTORY ${DERIVED_SOURCES_BROWSERAPP_DIR})

set(BrowserApp_SOURCES
    ${DERIVED_SOURCES_BROWSERAPP_DIR}/BrowserMarshal.c

    #${BROWSERAPP_DIR}/download/BrowserDownloadsBar.c
    #${BROWSERAPP_DIR}/download/BrowserDownloadsBar.h
    ${BROWSERAPP_DIR}/download/BrowserDownloadDialog.c
    ${BROWSERAPP_DIR}/download/BrowserDownloadDialog.h
    ${BROWSERAPP_DIR}/download/BrowserDownloadFile.c
    ${BROWSERAPP_DIR}/download/BrowserDownloadFile.h

    ${BROWSERAPP_DIR}/settings/BrowserSettingsDialog.c
    ${BROWSERAPP_DIR}/settings/BrowserSettingsDialog.h

    ${BROWSERAPP_DIR}/main/BrowserWindow.c
    ${BROWSERAPP_DIR}/main/BrowserWindow.h
    ${BROWSERAPP_DIR}/main/main.c
    ${BROWSERAPP_DIR}/main/BrowserCellRendererVariant.c
    ${BROWSERAPP_DIR}/main/BrowserCellRendererVariant.h
    ${BROWSERAPP_DIR}/main/BrowserSearchBar.c
    ${BROWSERAPP_DIR}/main/BrowserSearchBar.h
    ${BROWSERAPP_DIR}/main/BrowserMenuBar.c
    ${BROWSERAPP_DIR}/main/BrowserMenuBar.h
    ${BROWSERAPP_DIR}/main/TabMng.h
    ${BROWSERAPP_DIR}/main/TabMng.c
)

set(BrowserApp_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${DERIVED_SOURCES_BROWSERAPP_DIR}
    ${DERIVED_SOURCES_WEBKIT2GTK_DIR}
    ${FORWARDING_HEADERS_WEBKIT2GTK_DIR}
    ${FORWARDING_HEADERS_DIR}
    ${CMAKE_SOURCE_DIR}/Source
    ${GTK3_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${LIBSOUP_INCLUDE_DIRS}

    ${BROWSERAPP_DIR}/main
    ${BROWSERAPP_DIR}/settings
    ${BROWSERAPP_DIR}/download
)

set(BrowserApp_LIBRARIES
    ${JavaScriptCore_LIBRARY_NAME}
    WebKit2
    ${GTK3_LIBRARIES}
    ${GLIB_LIBRARIES}
)

add_custom_command(
    OUTPUT ${DERIVED_SOURCES_BROWSERAPP_DIR}/BrowserMarshal.c
           ${DERIVED_SOURCES_BROWSERAPP_DIR}/BrowserMarshal.h
    MAIN_DEPENDENCY ${BROWSERAPP_DIR}/main/browser-marshal.list
    COMMAND glib-genmarshal --prefix=browser_marshal ${BROWSERAPP_DIR}/main/browser-marshal.list --body > ${DERIVED_SOURCES_BROWSERAPP_DIR}/BrowserMarshal.c
    COMMAND glib-genmarshal --prefix=browser_marshal ${BROWSERAPP_DIR}/main/browser-marshal.list --header > ${DERIVED_SOURCES_BROWSERAPP_DIR}/BrowserMarshal.h
    VERBATIM)

add_definitions(-DWEBKIT_EXEC_PATH="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
add_definitions(-DWEBKIT_INJECTED_BUNDLE_PATH="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

include_directories(${BrowserApp_INCLUDE_DIRECTORIES})
add_executable(CuprumBrowser ${BrowserApp_SOURCES})
target_link_libraries(CuprumBrowser ${BrowserApp_LIBRARIES})
set_target_properties(CuprumBrowser PROPERTIES FOLDER "BrowserApp")
# copy all resources into dest dir where browser app be.
if (EXISTS ${BROWSERAPP_DIR}/resources)
    add_custom_command(TARGET CuprumBrowser COMMAND cp -rf ${BROWSERAPP_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
else ()
    message(STATUS "ZRL No Resources!!!!")
endif ()
install(TARGETS CuprumBrowser DESTINATION "${EXEC_INSTALL_DIR}")
