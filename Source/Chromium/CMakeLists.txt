# ZRL create the file for compiling Chromium Codes

# set Chromium Source root dir
#set(CHROMIUM_SOURCES_DIR "${CMAKE_SOURCE_DIR}/Source/Chromium") #move to the upper CMakeList.txt 

# CHROMIUM_SOURCES holds all sources codes required by compiling.
set(CHROMIUM_SOURCES "")

set(CHROMIUM_FILES_SOURCES
    "${CHROMIUM_SOURCES_DIR}/base/files/file.cc"
    "${CHROMIUM_SOURCES_DIR}/base/files/file_enumerator.cc"
    "${CHROMIUM_SOURCES_DIR}/base/files/file_enumerator_posix.cc"
    "${CHROMIUM_SOURCES_DIR}/base/files/file_path.cc"
    "${CHROMIUM_SOURCES_DIR}/base/files/file_path_constants.cc"
#    "${CHROMIUM_SOURCES_DIR}/base/files/file_path_watcher.cc"
#    "${CHROMIUM_SOURCES_DIR}/base/files/file_path_watcher_stub.cc"
    "${CHROMIUM_SOURCES_DIR}/base/files/file_posix.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/files/file_proxy.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/files/file_util_proxy.cc"
   "${CHROMIUM_SOURCES_DIR}/base/files/important_file_writer.cc"
#    "${CHROMIUM_SOURCES_DIR}/base/files/memory_mapped_file.cc"
#    "${CHROMIUM_SOURCES_DIR}/base/files/memory_mapped_file_posix.cc"
    "${CHROMIUM_SOURCES_DIR}/base/files/scoped_file.cc"
    "${CHROMIUM_SOURCES_DIR}/base/files/scoped_platform_file_closer.cc"
    "${CHROMIUM_SOURCES_DIR}/base/files/scoped_temp_dir.cc"
)

set(CHROMIUM_STRINGS_SOURCES
    "${CHROMIUM_SOURCES_DIR}/base/strings/string_number_conversions.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/latin1_string_conversions.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/nullable_string16.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/safe_sprintf.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/string16.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/string_piece.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/stringprintf.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/string_split.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/string_util.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/string_util_constants.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/sys_string_conversions_posix.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/utf_offset_string_conversions.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/utf_string_conversions.cc"
    "${CHROMIUM_SOURCES_DIR}/base/strings/utf_string_conversion_utils.cc"
)

set(CHROMIUM_PREFS_SOURCES
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/value_map_pref_store.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/scoped_user_pref_update.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/pref_value_store.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/pref_value_map.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/pref_store.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/pref_service_factory.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/pref_service.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/pref_registry_simple.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/pref_registry.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/pref_notifier_impl.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/pref_member.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/pref_change_registrar.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/overlay_user_pref_store.cc"
    "${CHROMIUM_SOURCES_DIR}/base/prefs/json_pref_store.cc"
    #"${CHROMIUM_SOURCES_DIR}/base/prefs/default_pref_store.cc"
)

set(CHROMIUM_JSON_SOURCES
    "${CHROMIUM_SOURCES_DIR}/base/json/json_file_value_serializer.cc"
    "${CHROMIUM_SOURCES_DIR}/base/json/json_parser.cc"
    "${CHROMIUM_SOURCES_DIR}/base/json/json_reader.cc"
    "${CHROMIUM_SOURCES_DIR}/base/json/json_string_value_serializer.cc"
    "${CHROMIUM_SOURCES_DIR}/base/json/json_writer.cc"
    "${CHROMIUM_SOURCES_DIR}/base/json/string_escape.cc"
)

set(CHROMIUM_THIRDPARTY_SOURCES
    "${CHROMIUM_SOURCES_DIR}/base/third_party/dmg_fp/dtoa_wrapper.cc"
    "${CHROMIUM_SOURCES_DIR}/base/third_party/dmg_fp/g_fmt.cc"

    "${CHROMIUM_SOURCES_DIR}/base/third_party/icu/icu_utf.cc"

    "${CHROMIUM_SOURCES_DIR}/base/third_party/nspr/prtime.cc"
)

set(CHROMIUM_MISC_SOURCES
    "${CHROMIUM_SOURCES_DIR}/base/values.cc"

    "${CHROMIUM_SOURCES_DIR}/base/file_util_posix.cc"
    "${CHROMIUM_SOURCES_DIR}/base/file_util.cc"
    "${CHROMIUM_SOURCES_DIR}/base/platform_file.cc"
    "${CHROMIUM_SOURCES_DIR}/base/platform_file_posix.cc"
    
    #"${CHROMIUM_SOURCES_DIR}/base/atomicops_internals_x86_gcc.cc"
    "${CHROMIUM_SOURCES_DIR}/base/pickle.cc"
    
    #"${CHROMIUM_SOURCES_DIR}/base/memory/singleton.cc"
)

set(CHROMIUM_TIME_SOURCES
    "${CHROMIUM_SOURCES_DIR}/base/time/clock.cc"

    "${CHROMIUM_SOURCES_DIR}/base/time/tick_clock.cc"
    "${CHROMIUM_SOURCES_DIR}/base/time/time.cc"
    "${CHROMIUM_SOURCES_DIR}/base/time/time_posix.cc"
)

list(APPEND CHROMIUM_SOURCES
    ${CHROMIUM_JSON_SOURCES}
    ${CHROMIUM_STRINGS_SOURCES}
    ${CHROMIUM_THIRDPARTY_SOURCES}
    ${CHROMIUM_MISC_SOURCES}
    ${CHROMIUM_FILES_SOURCES}
    ${CHROMIUM_TIME_SOURCES}
    ${CHROMIUM_PREFS_SOURCES}
)

# all .h files required by .cc files in the dirs.
set(CHROMIUM_INCLUDE_DIRECTORIES
    "${CHROMIUM_SOURCES_DIR}"
    "${CHROMIUM_SOURCES_DIR}/base"
    "${CHROMIUM_SOURCES_DIR}/base/files"
    
    "${CMAKE_SOURCE_DIR}/Source/ThirdParty"
)

# set chromium library name
set(Chromium_OUTPUT_NAME ChromiumGTK)

# set chromium sub-project library type as static
set(Chromium_LIBRARY_TYPE STATIC)
#set(Chromium_LIBRARY_TYPE SHARED)

include_directories(${CHROMIUM_INCLUDE_DIRECTORIES})
add_library(Chromium ${Chromium_LIBRARY_TYPE} ${CHROMIUM_SOURCES})
set_target_properties(Chromium PROPERTIES COMPILE_FLAGS "-fPIC")
set_target_properties(Chromium PROPERTIES COMPILE_DEFINITIONS "BUILDING_Chromium")
set_target_properties(Chromium PROPERTIES FOLDER "ChromiumCodes")

if (Chromium_OUTPUT_NAME)
    set_target_properties(Chromium PROPERTIES OUTPUT_NAME ${Chromium_OUTPUT_NAME})
endif ()
