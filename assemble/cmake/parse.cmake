function(parse_add_source_groups)
	set(BLOOD_INCLUDE_DIR "${BLOOD_ROOT}/include/blood/")
	set(BLOOD_SOURCE_DIR  "${BLOOD_ROOT}/source/engine/")

	set(SCAN_FOLDERS
		chrono
		common
		crypt
		device
		gui
		math
		net
		parse
		pattern
		render
		sys
		vision
	)
	
	foreach(folder ${SCAN_FOLDERS})
		file(GLOB CODE_${folder}   "${BLOOD_INCLUDE_DIR}${folder}/*.hxx"   "${BLOOD_SOURCE_DIR}${folder}/*.cxx" )
		source_group(${folder} FILES ${CODE_${folder}})
		set(BLOOD_SOURCES_FILES ${BLOOD_SOURCES_FILES} ${CODE_${folder}})
		set(BLOOD_INCLUDES_DIR ${BLOOD_INCLUDES_DIR} ${BLOOD_INCLUDE_DIR}${folder})
	endforeach()

    set(BLOOD_SOURCE_FILES  ${BLOOD_SOURCE_FILES}  PARENT_SCOPE)
    set(BLOOD_INCLUDE_DIR   ${BLOOD_INCLUDES_DIR}   PARENT_SCOPE)
    set(BLOOD_SOURCES_FILES ${BLOOD_SOURCES_FILES} PARENT_SCOPE)
endfunction()

