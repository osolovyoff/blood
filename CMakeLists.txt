cmake_minimum_required(VERSION 3.0)
project(blood)
# _________________________________________ Project name
set(PRODUCT test)
# _________________________________________ Options
option(BUILD_LIBRARY "Switching between library and executable" OFF)
option(64-Bit "Build x64 platform" ON)
option(STORE "Create Windows 8.1 or more; Store application" OFF)
# _________________________________________ Use technology
set(USE_DEFINITION
    #USE_CURL
    #USE_OPENSSL
    #USE_OCULUS
    #USE_SDL2
    #USE_BOOST
    #USE_DIRECTX
    #USE_OPENCV
)
# _________________________________________ Directories
set(BLOOD_ROOT ${CMAKE_HOME_DIRECTORY})
set(BLOOD_INTERMEDIATE ${BLOOD_ROOT}/build/intermediate/)
set(PLATFROM_NAME ${CMAKE_SYSTEM_NAME})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BLOOD_ROOT}/build/lib/${PLATFORM_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BLOOD_ROOT}/build/lib/${PLATFORM_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BLOOD_ROOT}/build/bin/${PLATFORM_NAME})

set(CURL_INCLUDE_DIR ${BLOOD_ROOT}/extern/curl/include/)

set(64-Bit ${64-Bit} CACHE STRING "" FORCE)
# _________________________________________ Target machine
IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

if(64-Bit)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(MACHINE_TARGET x86)
        set(x86 1)
    else()
        set(MACHINE_TARGET x64)
        set(x64 1)
    endif()    
endif()

if(STORE)
	set(APP_MANIFEST_NAME Package.appxmanifest)
	set(CMAKE_SYSTEM_NAME WindowsStore CACHE STRING "" FORCE)
	set(CMAKE_SYSTEM_VERSION 8.1  CACHE STRING "" FORCE)
	SET(CMAKE_VS_TARGET_PLATFORM 8.1)
	set(CMAKE_VS_PLATFORMS Win32;x64;ARM)
	set(PLATFROM STORE)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	set(EXE_NAME   ${PRODUCT})
	set(SHORT_NAME ${PRODUCT})
endif()

if(win32)
    set(BLOOD_PLATFORM ${BLOOD_PLATFORM} BLOOD_32)
endif()
if(UNIX)
    set(BLOOD_PLATFORM ${BLOOD_PLATFORM} BLOOD_UNIX)
endif()
if(MINGW)
    set(BLOOD_PLATFORM ${BLOOD_PLATFORM} BLOOD_MINGW)
endif()
if(SYGWIN)
    set(BLOOD_PLATFORM ${BLOOD_PLATFORM} BLOOD_SYGWIN)
endif()
if(MSVC)
    set(BLOOD_PLATFORM ${BLOOD_PLATFORM} BLOOD_VS)
    if(STORE)
    	set(BLOOD_STORE ${BLOOD_PLATFROM} BLOOD_STORE)
    endif()
endif()
message(STATUS Platform = ${BLOOD_PLATFORM})
# _________________________________________ Use libraries
foreach(USING ${USE_DEFINITION})
	set(${USING} ON)
endforeach()
# _________________________________________ Library find
if(USE_OPENSSL)
	set(LIBRARIES ${LIBRARIES} openssl)
endif()
if(USE_CURL)
	set(LIBRARIES ${LIBRARIES} curl)	
endif()
if(USE_OCULUS)
	set(LIBRARIES ${LIBRARIES} oculus)	
endif()
if(USE_SDL2)
	set(LIBRARIES ${LIBRARIES} sdl2)	
endif()
if(USE_BOOST)
	set(LIBRARIES ${LIBRARIES} boost)	
endif()
if(USE_DIRECTX)
	set(LIBRARIES ${LIBRARIES} directx)	
endif()
if(USE_OPENCV)
    set(LIBRARIES ${LIBRARIES} opencv)
endif()

foreach (LIBRARY ${LIBRARIES})
    add_subdirectory(${BLOOD_ROOT}/extern/${LIBRARY}/ ${BLOOD_ROOT}/build/intermediate/${LIBRARY}/)
endforeach()
message(STATUS)
message(STATUS "Connect libraries: ${LIBRARIES}")
message(STATUS)
# _________________________________________ Project configurations LINK and COMPILE
foreach(USING ${USE_DEFINITION})
	add_definitions(-D${USING})
endforeach()

add_definitions(-D${BLOOD_PLATFORM})

if(MSVC)
	add_definitions(-D _CRT_SECURE_NO_WARNINGS)
	add_definitions(/sdl /EHsc /Od /RTC1 /GS)
	add_definitions(/MTd /Zi)
endif()

if(USE_CURL)
	add_definitions(-D BUILDING_LIBCURL)
    add_definitions(-D CURL_STATICLIB)
endif()

if(USE_OPENSSL)
	add_definitions(-D USE_SSLEAY)
endif()

if(UNIX)
    #add_definitions(-fpermissive -std=c++11)
endif()
# _________________________________________ 
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "DEBUG;RELEASE;")
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)
endif()

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -std=c++11")
	set(CMAKE_C++_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE}") 
endif()

if(MSVC)
	if(STORE)
		set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /MDd /ZW /Ob0 /Od /EHsc /RTC1 /D_DEBUG")
    	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /O2 /Ob2 /D NDEBUG")
	else()
		set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /MTd /Zi /Ob0 /Od /EHsc /RTC1 /D_DEBUG")
    	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /O2 /Ob2 /D NDEBUG")	
	endif()
endif()
# _________________________________________ Source code
set(BLOOD_INCLUDE_DIR "${BLOOD_ROOT}/include/blood/")
set(BLOOD_SOURCE_DIR  "${BLOOD_ROOT}/source/")

file(GLOB CODE_COMMON   "${BLOOD_INCLUDE_DIR}common/*.hxx"   "${BLOOD_SOURCE_DIR}engine/common/*.cxx")
file(GLOB CODE_CHRONO   "${BLOOD_INCLUDE_DIR}chrono/*.hxx"   "${BLOOD_SOURCE_DIR}engine/chrono/*.cxx")
file(GLOB CODE_CRYPT    "${BLOOD_INCLUDE_DIR}crypt/*.hxx"    "${BLOOD_SOURCE_DIR}engine/crypt/*.cxx")
file(GLOB CODE_PARSER   "${BLOOD_INCLUDE_DIR}parse/*.hxx"    "${BLOOD_SOURCE_DIR}engine/parse/*.cxx")
file(GLOB CODE_NET      "${BLOOD_INCLUDE_DIR}net/*.hxx"      "${BLOOD_SOURCE_DIR}engine/net/*.cxx")
file(GLOB CODE_SYS      "${BLOOD_INCLUDE_DIR}sys/*.hxx"      "${BLOOD_SOURCE_DIR}engine/sys/*.cxx")
file(GLOB CODE_PATTERN  "${BLOOD_INCLUDE_DIR}pattern/*.hxx"  "${BLOOD_SOURCE_DIR}engine/pattern/*.cxx")
file(GLOB CODE_VISION   "${BLOOD_INCLUDE_DIR}vision/*.hxx"   "${BLOOD_SOURCE_DIR}engine/vision/*cxx")
file(GLOB CODE_GRAPHIC "${BLOOD_INCLUDE_DIR}gfx/*.hxx"      "${BLOOD_SOURCE_DIR}engine/gfx/*cxx")

file(GLOB CODE_CODE
    "${BLOOD_SOURCE_DIR}code/*.h*"
    "${BLOOD_SOURCE_DIR}code/*.c*"
)

source_group(common                           FILES ${CODE_COMMON})
source_group(chrono                           FILES ${CODE_CHRONO})
source_group(crypt                            FILES ${CODE_CRYPT})
source_group(parse                            FILES ${CODE_PARSER})
source_group(network                          FILES ${CODE_NET})
source_group(system                           FILES ${CODE_SYS} )
source_group(pattern                          FILES ${CODE_PATTERN})
source_group(vision                           FILES ${CODE_VISION})
source_group(graphic                          FILES ${CODE_GRAPHIC})
source_group(""                               FILES ${CODE_CODE})

set(BLOOD_SOURCES_FILES
    ${CODE_COMMON}
    ${CODE_CHRONO}
    ${CODE_CRYPT}
    ${CODE_PARSER}
    ${CODE_GRAPHICS}
    ${CODE_NET}
    ${CODE_SYS}
    ${CODE_PATTERN}    
    ${CODE_VISION}
)

# __________________________________________ Include and Link directories
set(BLOOD_INCLUDES  ${BLOOD_INCLUDE_DIR} ${BLOOD_ROOT}/include/)
set(BLOOD_LIBRARIES ${BLOOD_LIBRARIES} ${BLOOD_ROOT})

if(USE_CURL)
    set(BLOOD_INCLUDES ${BLOOD_INCLUDES} ${CURL_INCLUDE_DIR})
endif()
if(USE_SDL2)
    set(BLOOD_INCLUDES ${BLOOD_INCLUDES} ${SDL2_INCLUDE_DIR})
endif()
if(USE_OCULUS)
    set(BLOOD_INCLUDES ${BLOOD_INCLUDES} ${OCULUS_INCLUDE_DIR})
endif()
if(USE_OPENSSL)
    set(BLOOD_INCLUDES ${BLOOD_INCLUDES} ${OPENSSL_INCLUDE_DIR})
endif()
if(USE_DIRECTX)
    set(BLOOD_INCLUDES ${BLOOD_INCLUDES} ${D3DX_INCLUDE_DIR})
    set(BLOOD_LIBRARIES ${BLOOD_INCLUDES} ${D3DX_LIBRARIES_DIR})
endif()
if(USE_BOOST)
    set(BLOOD_INCLUDES ${BLOOD_INCLUDES} ${BOOST_ROOT_DIR} ${BOOST_INCLUDE_DIRS})
    set(BLOOD_LIBRARIES ${BLOOD_INCLUDES} ${BOOST_LIBRARY_DIR})
endif()
if(USE_OPENCV)
	set(BLOOD_INCLUDES ${BLOOD_INCLUDES} ${OpenCV_SOURCE_DIR}/include/)
	foreach(_module ${OpenCVModules_TARGETS})
		set(_module_path ${${_module}_SOURCE_DIR}/)
		set(BLOOD_INCLUDES ${BLOOD_INCLUDES} ${_module_path}include/)
        set(BLOOD_LIBRARIES ${BLOOD_INTERMEDIATE}/opencv/lib/)
        set(BLOOD_LIBRARIES ${BLOOD_INTERMEDIATE}/opencv/3rdparty/)
	endforeach()
endif()

include_directories(${BLOOD_INCLUDES} )
link_directories(${BLOOD_LIBRARIES})

# __________________________________________ Build Project
if(BUILD_LIBRARY)
    message(STATUS "blood compile like library...")

    add_library(blood STATIC
        ${BLOOD_SOURCES_FILES})
else()
    message(STATUS "blood compile like executable...")
    source_group(root ${BLOOD_SOURCE_DIR}main.cxx)
    add_executable(blood
        ${CODE_CODE}
	    ${BLOOD_SOURCES_FILES})
endif()
# _________________________________________ set libraries
set_target_properties(blood PROPERTIES LINKER_LANGUAGE C++)

if(USE_CURL)
    target_link_libraries(blood libcurl)
endif()
if(USE_OCULUS)
    target_link_libraries(blood oculus)
endif()
if(USE_SDL2)
    target_link_libraries(blood sdl2)	
endif()

if(USE_BOOST)
    target_link_libraries(blood ${BOOST_LIBRARIES})
endif()
if(USE_OPENCV)
    target_link_libraries(blood IlmImf libjasper libjpeg libpng libtiff libwebp zlib)
    target_link_libraries(blood comctl32 comdlg32 gdi32 vfw32 user32 oleaut32)
    target_link_libraries(blood opencv_calib3d opencv_core opencv_features2d opencv_flann opencv_hal
                                opencv_highgui opencv_imgcodecs opencv_imgproc opencv_ts opencv_shape
                                opencv_photo opencv_ml opencv_stitching opencv_superres opencv_video
                                opencv_videoio opencv_videostab opencv_objdetect)
endif()

if(USE_OPENSSL)
	if(MSVC)
	    set(LIB_TO_LINK
	    	Shell32 Advapi32 ws2_32 wldap32 winmm mswsock
	    	user32 gdi32 imm32 ole32 oleaut32 uuid version)
	    target_link_libraries(blood ${LIB_TO_LINK})
        target_link_libraries(blood ${OPENSSL_LIBRARIES})
	elseif(UNIX)
        target_link_libraries(blood crypto)
	    target_link_libraries(blood ${OPENSSL_LIBRARIES})
	endif()
endif()

# _________________________________________ Template project
project(blood)