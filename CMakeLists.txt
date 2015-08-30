cmake_minimum_required(VERSION 3.0)

include(assemble/cmake/parse.cmake)
include(assemble/cmake/submodule.cmake)
include(assemble/cmake/configuration.cmake)

set(project_name vbloodv)
project(${project_name})
# _________________________________________ Options


option(BUILD_LIBRARY "Switching between library and executable" OFF)
option(64-Bit "Build x64 platform" OFF)
option(STORE "Create Windows 8.1 or more; Store application" OFF)
# _________________________________________ Use technology

option(USE_BOOST   "" OFF)
option(USE_CURL    "" OFF)
option(USE_OPENSSL "" OFF)
option(USE_OPENCV  "" OFF)
option(USE_OCULUS  "" OFF)

# _________________________________________ Processing

submodule_fill_technology_list()
submodule_initialize_engine_paths()

configuration_project()
configuration_linking_flags()
configuration_project_types()

submodule_connect_external_projects()

parse_add_source_groups()
print_connection_libraries()

# _________________________________________ Processing

include_directories(${BLOOD_ROOT}/include/)
link_directories(${BLOOD_LIBRARIES})

add_library(blood STATIC ${BLOOD_SOURCES_FILES})

# _________________________________________ Create template project ${project_name}

file(GLOB PROJECT_CODE "${BLOOD_ROOT}/source/code/*.*")
source_group("" FILES ${PROJECT_CODE})
add_executable(${project_name} ${PROJECT_CODE})

# _________________________________________ Set targets

set_target_properties(blood           PROPERTIES LINKER_LANGUAGE C++)
set_target_properties(${project_name} PROPERTIES LINKER_LANGUAGE C++)
target_link_libraries(${project_name} blood)