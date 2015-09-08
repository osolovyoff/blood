if(NOT BOOST_FOUND)
	set(BOOST_ROOT $ENV{BOOST_ROOT})
	find_package(Boost 1.58 COMPONENTS ${Boost_components} REQUIRED)
	message(STATUS "Connect boost components : ${Boost_components}")
endif()

set(BOOST_LIBRARIES    ${Boost_LIBRARIES}    CACHE STRING "" FORCE)
set(BOOST_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} CACHE STRING "" FORCE)