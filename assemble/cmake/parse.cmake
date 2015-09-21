function(parse_add_source_groups)
	set(BLOOD_INCLUDE_DIR "${BLOOD_ROOT}/include/blood/")
	set(BLOOD_SOURCE_DIR  "${BLOOD_ROOT}/source/engine/")

	set(SCAN_FOLDERS
		pattern
		common
		chrono
		vision
		crypt
		parse
		math
		gui
		net
		sys
		gfx
	)
	
	foreach(folder ${SCAN_FOLDERS})
		file(GLOB CODE_${folder}   "${BLOOD_INCLUDE_DIR}${folder}/*.hxx"   "${BLOOD_SOURCE_DIR}${folder}/*.cxx" )
		source_group(${folder} FILES ${CODE_${folder}})
		set(BLOOD_SOURCES_FILES ${BLOOD_SOURCES_FILES} ${CODE_${folder}})
	endforeach()

	#file(GLOB CODE_COMMON   "${BLOOD_INCLUDE_DIR}common/*.hxx"   "${BLOOD_SOURCE_DIR}common/*.cxx")
	#file(GLOB CODE_CHRONO   "${BLOOD_INCLUDE_DIR}chrono/*.hxx"   "${BLOOD_SOURCE_DIR}chrono/*.cxx")
	#file(GLOB CODE_CRYPT    "${BLOOD_INCLUDE_DIR}crypt/*.hxx"    "${BLOOD_SOURCE_DIR}crypt/*.cxx")
	#file(GLOB CODE_PARSER   "${BLOOD_INCLUDE_DIR}parse/*.hxx"    "${BLOOD_SOURCE_DIR}parse/*.cxx")
	#file(GLOB CODE_NET      "${BLOOD_INCLUDE_DIR}net/*.hxx"      "${BLOOD_SOURCE_DIR}net/*.cxx")
	#file(GLOB CODE_SYS      "${BLOOD_INCLUDE_DIR}sys/*.hxx"      "${BLOOD_SOURCE_DIR}sys/*.cxx")
	#file(GLOB CODE_PATTERN  "${BLOOD_INCLUDE_DIR}pattern/*.hxx"  "${BLOOD_SOURCE_DIR}pattern/*.cxx")
	#file(GLOB CODE_VISION   "${BLOOD_INCLUDE_DIR}vision/*.hxx"   "${BLOOD_SOURCE_DIR}vision/*cxx")
	#file(GLOB CODE_GRAPHIC  "${BLOOD_INCLUDE_DIR}gfx/*.hxx"      "${BLOOD_SOURCE_DIR}gfx/*cxx")
	#
	#source_group(common                           FILES ${CODE_COMMON})
	#source_group(chrono                           FILES ${CODE_CHRONO})
	#source_group(crypt                            FILES ${CODE_CRYPT})
	#source_group(parse                            FILES ${CODE_PARSER})
	#source_group(net                              FILES ${CODE_NET})
	#source_group(text                             FILES ${CODE_TEXT})
	#source_group(system                           FILES ${CODE_SYS} )
	#source_group(pattern                          FILES ${CODE_PATTERN})
	#source_group(vision                           FILES ${CODE_VISION})
	#source_group(graphic                          FILES ${CODE_GRAPHIC})
	#
	#set(BLOOD_SOURCES_FILES
	#    ${CODE_COMMON}
	#    ${CODE_CHRONO}
	#    ${CODE_CRYPT}
	#    ${CODE_PARSER}
	#    ${CODE_GRAPHICS}
	#    ${CODE_NET}
	#    ${CODE_FUNC}
	#    ${CODE_SYS}
	#    ${CODE_PATTERN}    
	#    ${CODE_VISION}
    #)

    set(BLOOD_SOURCE_FILES  ${BLOOD_SOURCE_FILES}  PARENT_SCOPE)
    set(BLOOD_INCLUDE_DIR   ${BLOOD_INCLUDE_DIR}   PARENT_SCOPE)
    set(BLOOD_SOURCES_FILES ${BLOOD_SOURCES_FILES} PARENT_SCOPE)
endfunction()

