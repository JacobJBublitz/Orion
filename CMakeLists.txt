cmake_minimum_required(VERSION 3.13)

project(OrionEngine)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

include(GetGitRevisionDescription)

set(ORION_VERSION_MAJOR 0)
set(ORION_VERSION_MINOR 1)
set(ORION_VERSION_PATCH 0)
set(ORION_VERSION_PRERELEASE)

git_describe(GIT_COMMIT_HASH)

list(APPEND ORION_VERSION_BUILDINFO ${GIT_COMMIT_HASH})

git_local_changes(GIT_LOCAL_CHANGES)
if (GIT_LOCAL_CHANGES STREQUAL "DIRTY")
    list(APPEND ORION_VERSION_BUILDINFO dirty)
endif ()

list(JOIN ORION_VERSION_BUILDINFO "." ORION_VERSION_BUILDINFO)

add_library(OrionEngine_Core STATIC)
add_library(OrionEngine::Core ALIAS OrionEngine_Core)
add_subdirectory(Core)

add_executable(OrionEngine)
add_subdirectory(Engine)


