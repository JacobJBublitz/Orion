cmake_minimum_required(VERSION 3.13)

project(OrionEngine)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

include(GetGitRevisionDescription)

git_describe(GIT_DESC --abbrev=0)
if ("${GIT_DESC}" MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)$")
    # Major.Minor.Patch
    set(ORION_VERSION_MAJOR ${CMAKE_MATCH_1})
    set(ORION_VERSION_MINOR ${CMAKE_MATCH_2})
    set(ORION_VERSION_PATCH ${CMAKE_MATCH_3})
    set(ORION_VERSION_PRERELEASE "")
elseif ("${GIT_DESC}" MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)\\-([0-9A-Za-z\\-\\.]+)$")
    # Major.Minor.Patch-Prerelease
    set(ORION_VERSION_MAJOR ${CMAKE_MATCH_1})
    set(ORION_VERSION_MINOR ${CMAKE_MATCH_2})
    set(ORION_VERSION_PATCH ${CMAKE_MATCH_3})
    set(ORION_VERSION_PRERELEASE ${CMAKE_MATCH_4})
else ()
    message(WARNING "Unable to parse latest git tag for version information (${GIT_DESC})")
    set(ORION_VERSION_MAJOR 0)
    set(ORION_VERSION_MINOR 0)
    set(ORION_VERSION_PATCH 0)
    set(ORION_VERSION_PRERELEASE "no.version.info")
endif ()

get_git_head_revision(GIT_REFSPEC GIT_HASH)
string(SUBSTRING "${GIT_HASH}" 0 7 GIT_HASH)
list(APPEND ORION_VERSION_BUILDINFO ${GIT_HASH})

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


