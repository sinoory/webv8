# Created by jiayu for building web-extension 
set(EXTENSIONDIR "/usr/local/lib/${MIDORI_PROJECT_NAME}/web-extension")
set(MIDORI_DIR "${CMAKE_SOURCE_DIR}/Source/BrowserApp")
set(EXTRALIB_DIR_secret "/usr/include/libsecret-1")
set(EXTRALIB_DIR_xml "/usr/include/libxml2/")
set(CMAKE_BUILD_TYPE Debug)


set(BrowserWebextension_SOURCE
    ${MIDORI_DIR}/web-extension/ephy-embed-form-auth.c
    ${MIDORI_DIR}/web-extension/ephy-embed-form-auth.h
    ${MIDORI_DIR}/web-extension/ephy-web-extension.c
    ${MIDORI_DIR}/web-extension/ephy-web-extension.h
    ${MIDORI_DIR}/web-extension/ephy-web-extension-main.c
    ${MIDORI_DIR}/web-extension/ephy-web-extension-names.h
    ${MIDORI_DIR}/web-extension/ephy-web-overview.c
    ${MIDORI_DIR}/web-extension/ephy-web-overview.h
    ${MIDORI_DIR}/web-extension/ephy-web-overview-model.c
    ${MIDORI_DIR}/web-extension/ephy-web-overview-model.h
    
    ${MIDORI_DIR}/web-extension-base/ephy-form-auth-data.c
    ${MIDORI_DIR}/web-extension-base/ephy-form-auth-data.h
    ${MIDORI_DIR}/web-extension-base/ephy-debug.h
    ${MIDORI_DIR}/web-extension-base/ephy-debug.c
    ${MIDORI_DIR}/web-extension-base/ephy-string.h
    ${MIDORI_DIR}/web-extension-base/ephy-string.c
    ${MIDORI_DIR}/web-extension-base/ephy-sqlite-connection.c
    ${MIDORI_DIR}/web-extension-base/ephy-sqlite-connection.h
    ${MIDORI_DIR}/web-extension-base/ephy-sqlite.h
    ${MIDORI_DIR}/web-extension-base/ephy-sqlite-statement.c
    ${MIDORI_DIR}/web-extension-base/ephy-sqlite-statement.h
    ${MIDORI_DIR}/web-extension-base/ephy-uri-helpers.c
    ${MIDORI_DIR}/web-extension-base/ephy-uri-helpers.h
    ${MIDORI_DIR}/web-extension-base/ephy-web-dom-utils.c
    ${MIDORI_DIR}/web-extension-base/ephy-web-dom-utils.h
    ${MIDORI_DIR}/web-extension-base/uri-tester.h
    ${MIDORI_DIR}/web-extension-base/uri-tester.c
)

set(BrowserApp_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${EXTRALIB_DIR_secret}
    ${EXTRALIB_DIR_xml}
    ${DERIVED_SOURCES_DIR}
    ${DERIVED_SOURCES_MIDORI_DIR}
    ${DERIVED_SOURCES_WEBKIT2GTK_DIR}
    ${FORWARDING_HEADERS_WEBKIT2GTK_DIR}
    ${FORWARDING_HEADERS_WEBKIT2GTK_EXTENSION_DIR}
    ${FORWARDING_HEADERS_DIR}
    ${CMAKE_SOURCE_DIR}/Source
    ${GTK3_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${LIBSOUP_INCLUDE_DIRS}

    ${MIDORI_DIR}/web-extension
    ${MIDORI_DIR}/web-extension-base
)

add_definitions(-DWEBKIT_EXEC_PATH="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
add_definitions(-DWEBKIT_INJECTED_BUNDLE_PATH="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
add_definitions(-DBROWSER_WEB_EXTENSION_PATH="${EXTENSIONDIR}")

#contruct a shared library
include_directories(${BrowserApp_INCLUDE_DIRECTORIES})
add_library(browserwebextension MODULE ${BrowserWebextension_SOURCE})
set_target_properties(browserwebextension PROPERTIES FOLDER "web-extension")
add_dependencies(browserwebextension WebKit2)
install(TARGETS browserwebextension
            LIBRARY DESTINATION ${EXTENSIONDIR}
                )
