include_directories(
    ..
    ../codegenerators
    ../codeimpwizard
    ../debug
    ../uml1model
    ../umlwidgets
    ../dialogs
    ../dialogs
    ../../lib/cppparser/
    ../../lib/interfaces/
)

set(libcodeimport_SRCS
    adaimport.cpp
    classimport.cpp
    cppimport.cpp
    idlimport.cpp
    import_utils.cpp
    javaimport.cpp
    nativeimportbase.cpp
    pascalimport.cpp
    pythonimport.cpp
    sqlimport.cpp
    csharp/csharpimport.cpp
)

if(BUILD_PHP_IMPORT)
    list(APPEND libcodeimport_SRCS
        phpimport.cpp
    )
endif()

set(libkdevcppparser_SRCS
    ${CMAKE_SOURCE_DIR}/lib/cppparser/ast.cpp
    ${CMAKE_SOURCE_DIR}/lib/cppparser/ast_utils.cpp
    ${CMAKE_SOURCE_DIR}/lib/cppparser/cachemanager.cpp
    ${CMAKE_SOURCE_DIR}/lib/cppparser/driver.cpp
    ${CMAKE_SOURCE_DIR}/lib/cppparser/errors.cpp
    ${CMAKE_SOURCE_DIR}/lib/cppparser/lexer.cpp
    ${CMAKE_SOURCE_DIR}/lib/cppparser/lexercache.cpp
    ${CMAKE_SOURCE_DIR}/lib/cppparser/lookup.cpp
    ${CMAKE_SOURCE_DIR}/lib/cppparser/parser.cpp
    ${CMAKE_SOURCE_DIR}/lib/cppparser/tree_parser.cpp
    ${CMAKE_SOURCE_DIR}/lib/interfaces/hashedstring.cpp
    kdevcppparser/cpptree2uml.cpp
)

add_library(codeimport STATIC ${libcodeimport_SRCS} ${libkdevcppparser_SRCS})
if(NOT BUILD_KF5)
    if(BUILD_PHP_IMPORT)
        target_link_libraries(codeimport
            ${KDEVPLATFORM_INTERFACES_LIBRARIES}
            ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
            ${KDEVPLATFORM_OUTPUTVIEW_LIBRARIES}
            ${KDEVPLATFORM_PROJECT_LIBRARIES}
            ${KDEVPLATFORM_SUBLIME_LIBRARIES}
            ${KDEVPLATFORM_SHELL_LIBRARIES}
            ${KDEVPLATFORM_UTIL_LIBRARIES}
            ${KDEVPLATFORM_VCS_LIBRARIES}
            ${KDEVPLATFORM_DEBUGGER_LIBRARIES}
            ${KDEVPLATFORM_DOCUMENTATION_LIBRARIES}
            ${KDEVPLATFORM_TESTS_LIBRARIES}
            ${KDEVPLATFORM_JSONTESTS_LIBRARIES}
            kdev4phpparser
        )
    endif()
else()
    target_link_libraries(codeimport
        KF5::KDELibs4Support
    )
    if(BUILD_PHP_IMPORT)
        target_link_libraries(codeimport
            KDev::Interfaces
            KDev::Language
            KDev::Tests
            kdevphpparser
        )
    endif()
endif()

target_include_directories(codeimport
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/csharp
)
