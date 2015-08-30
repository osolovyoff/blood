function(submodule_fill_technology_list)
	if(USE_BOOST)
		set(USE_DEFINITION ${USE_DEFINITION} USE_BOOST)
		set(SUBMODULE_FOLDER ${SUBMODULE_FOLDER} boost)	
	endif()
	if(USE_CURL)
		set(USE_DEFINITION ${USE_DEFINITION} USE_CURL)
		set(SUBMODULE_FOLDER ${SUBMODULE_FOLDER} curl)	
	endif()
	if(USE_OPENSSL)
		set(USE_DEFINITION ${USE_DEFINITION} USE_OPENSSL)
		set(SUBMODULE_FOLDER ${SUBMODULE_FOLDER} openssl)
	endif()
	if(USE_OPENCV)
		set(USE_DEFINITION ${USE_DEFINITION} USE_OPENCV)
		set(SUBMODULE_FOLDER ${SUBMODULE_FOLDER} opencv)
	endif()
	if(USE_OCULUS)
		set(USE_DEFINITION ${USE_DEFINITION} USE_OCULUS)
		set(SUBMODULE_FOLDER ${SUBMODULE_FOLDER} oculus)	
	endif()
	if(USE_SDL2)
		set(USE_DEFINITION ${USE_DEFINITION} USE_SDL2)
		set(SUBMODULE_FOLDER ${SUBMODULE_FOLDER} sdl2)	
	endif()
	if(USE_DIRECTX)
		set(USE_DEFINITION ${USE_DEFINITION} USE_DIRECTX)
		set(SUBMODULE_FOLDER ${SUBMODULE_FOLDER} directx)	
	endif()
	set(USE_DEFINITION ${USE_DEFINITION} PARENT_SCOPE)
	set(SUBMODULE_FOLDER ${SUBMODULE_FOLDER} PARENT_SCOPE)	
endfunction()

function(submodule_initialize_engine_paths)
	set(BLOOD_ROOT ${CMAKE_HOME_DIRECTORY} PARENT_SCOPE)
	set(BLOOD_INTERMEDIATE ${BLOOD_ROOT}/assemble/build/intermediate/ PARENT_SCOPE)
	set(PLATFROM_NAME ${CMAKE_SYSTEM_NAME} PARENT_SCOPE)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BLOOD_ROOT}/assemble/build/lib/${PLATFORM_NAME} PARENT_SCOPE)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BLOOD_ROOT}/assemble/build/lib/${PLATFORM_NAME} PARENT_SCOPE)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BLOOD_ROOT}/assemble/build/bin/${PLATFORM_NAME} PARENT_SCOPE)
	#set(CURL_INCLUDE_DIR ${BLOOD_ROOT}/extern/curl/include/ PARENT_SCOPE)
	set(64-Bit ${64-Bit} PARENT_SCOPE)
endfunction()

function(submodule_link_dependence)
	if(USE_CURL)
	    target_link_libraries(${project_name} libcurl)
	endif()
	if(USE_OCULUS)
	    target_link_libraries(${project_name} oculus)
	endif()
	if(USE_SDL2)
	    target_link_libraries(${project_name} sdl2)	
	endif()
	
	if(USE_BOOST)
	    target_link_libraries(${project_name} ${BOOST_LIBRARIES})
	endif()
	if(USE_OPENCV)
	    target_link_libraries(${project_name} IlmImf libjasper libjpeg libpng libtiff libwebp zlib)
	    target_link_libraries(${project_name} comctl32 comdlg32 gdi32 vfw32 user32 oleaut32)
	    target_link_libraries(${project_name} opencv_calib3d opencv_core opencv_features2d opencv_flann opencv_hal
	                                opencv_highgui opencv_imgcodecs opencv_imgproc opencv_ts opencv_shape
	                                opencv_photo opencv_ml opencv_stitching opencv_superres opencv_video
	                                opencv_videoio opencv_videostab opencv_objdetect)
	endif()
	
	if(USE_OPENSSL)
		if(MSVC)
		    set(LIB_TO_LINK
		    	Shell32 Advapi32 ws2_32 wldap32 winmm mswsock
		    	user32 gdi32 imm32 ole32 oleaut32 uuid version)
		    target_link_libraries(${project_name} ${LIB_TO_LINK})
	        target_link_libraries(${project_name} ${OPENSSL_LIBRARIES})
		elseif(UNIX)
	        target_link_libraries(${project_name} crypto)
		    target_link_libraries(${project_name} ${OPENSSL_LIBRARIES})
		endif()
	endif()
endfunction()

function(submodule_connect_external_projects)
	foreach(SUBMODULE ${SUBMODULE_FOLDERS})
		add_subdirectory(${BLOOD_ROOT}/extern/${SUBMODULE}/ ${BLOOD_ROOT}/assemble/build/intermediate/${SUBMODULE}/)
	endforeach()
endfunction()

