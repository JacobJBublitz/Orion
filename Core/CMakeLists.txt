configure_file(Private/BuildInfo.cc.in Private/BuildInfo.cc @ONLY)

list(APPEND PRIVATE_SOURCES
        "${CMAKE_CURRENT_BINARY_DIR}/Private/BuildInfo.cc"
        Private/Version.cc
        Private/Stl/String.cc
        )

if (WIN32)
    list(APPEND PRIVATE_SOURCES
            Private/Win32/Platform.cc
            )
else ()
    list(APPEND PRIVATE_SOURCES
            Private/Posix/Platform.cc
            )
endif ()

target_sources(
        OrionEngine_Core
        PUBLIC
        Public/Orion/BuildInfo.hh
        Public/Orion/Platform.hh
        Public/Orion/Version.hh
        Public/Orion/Stl/String.hh
        PRIVATE
        ${PRIVATE_SOURCES}
)
target_include_directories(
        OrionEngine_Core
        PUBLIC
        Public
        PRIVATE
        Private
)
target_compile_features(
        OrionEngine_Core
        PUBLIC
        cxx_std_20
)
set_target_properties(
        OrionEngine_Core PROPERTIES
        OUTPUT_NAME "OrionEngine.Core"
)

find_package(GTest)
if (TARGET GTest::gtest_main)
    add_executable(OrionEngine_Core_Tests
            Test/Version.cc
            )
    target_link_libraries(OrionEngine_Core_Tests PRIVATE OrionEngine::Core GTest::gtest_main)
endif ()