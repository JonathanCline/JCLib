#
#	Common utility cmake functions and macros 
#

#
#	Adds a list of sources to a target, sourceList should be a list variable
#
macro(ADD_SOURCES_LIST targetName sourceList)
	list(TRANSFORM ${sourceList} PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
	set(lfefiles )
	foreach(lfe IN LISTS ${sourceList})
		set(lfefiles ${lfefiles} ${lfe})
	endforeach()
	target_sources(${targetName} PRIVATE ${lfefiles})
endmacro(ADD_SOURCES_LIST)

#
#	Returns the child DIRECTORY paths of a given directory
#
macro(SUBDIRLIST result curdir)
	file(GLOB children RELATIVE ${curdir} ${curdir}/*)
	set(dirlist "")
	foreach(child ${children})
		if(IS_DIRECTORY ${curdir}/${child})
			list(APPEND dirlist ${child})
		endif()
	endforeach()
	set(${result} ${dirlist})
endmacro()

#
#	Returns the child paths of a given directory
#
#	@param out_Result Output variable, a list of paths will be written into it
#	@param in_DirectoryPath Directory to get children of
#
macro(GET_DIRECTORY_CONTENTS out_Result in_DirectoryPath)

	# The evaluated absolute file path
	set(__get_directory_contents_Path ${in_DirectoryPath})

	# Holds all child paths
	set(__get_directory_contents_Children )
	file(GLOB __get_directory_contents_Children
		RELATIVE ${__get_directory_contents_Path} "${__get_directory_contents_Path}/*")

	# Write results to output variable
	set(${out_Result} ${__get_directory_contents_Children})
endmacro()



#
#	Adds a list of subdirectories to the project, pathList should be a list variable
#
macro(ADD_SUBDIRS_LIST pathList)
	foreach(lfe IN LISTS ${pathList})
		add_subdirectory(${lfe})
	endforeach()
endmacro(ADD_SUBDIRS_LIST)

#
#	Includes all subdirectories from the current source path
#
macro(ADD_SUBDIRS_HERE)
	set(dirlist )
	SUBDIRLIST(dirlist ${CMAKE_CURRENT_SOURCE_DIR})
	foreach(lfe IN LISTS dirlist)
		set(lfename )		
		get_filename_component(lfename ${lfe} NAME)
		add_subdirectory(${lfename})
	endforeach()
endmacro()

#
#	Includes all paths listed that contain CMake lists
#
#	@param rootDir Root directory path
#
function(ADD_CMAKE_SUBDIRS_FN rootDir)

	# Get subdirectories
	set(subdirList )
	SUBDIRLIST(subdirList ${rootDir})

	# Include each subdir if it has a cmake lists
	foreach(subd IN LISTS subdirList)
		if (EXISTS "${rootDir}/${subd}/CMakeLists.txt")
			add_subdirectory("${rootDir}/${subd}")
		endif()
	endforeach()

endfunction()

#
#	Includes all subdirectories containing CMake lists from the current source dir
#
macro(ADD_CMAKE_SUBDIRS_HERE)
	ADD_CMAKE_SUBDIRS_FN("${CMAKE_CURRENT_SOURCE_DIR}")
endmacro()


#
#	Adds the sources in the current directory matching a pattern to a target
#
macro(ADD_SOURCES_HERE in_Target in_MatchPattern)

	# Check that a valid target was given
	if(NOT TARGET ${in_Target})
		message(FATAL_ERROR "Cannot add sources to invalid target ${in_Target}")
	endif()

	# Root path to get source files from
	set(__add_sources_here_Path "${CMAKE_CURRENT_SOURCE_DIR}")
	
	# Get directory contents
	set(__add_sources_here_Contents )
	GET_DIRECTORY_CONTENTS(__add_sources_here_Contents "${__add_sources_here_Path}")

	# Get list of paths that pass the given match pattern
	set(__add_sources_here_SourceList )
	foreach(lfe IN LISTS __add_sources_here_Contents)
		if ("${lfe}" MATCHES "${in_MatchPattern}")
			list(APPEND __add_sources_here_SourceList "${lfe}")
		endif()
	endforeach()

	# Add sources to project
	ADD_SOURCES_LIST(${in_Target} __add_sources_here_SourceList)

endmacro()



#
#	Adds the C++ sources in the current directory to a target
#
macro(ADD_CPP_SOURCES_HERE in_Target)

	# Match expression
	set(__add_cpp_sources_here_Pattern "\.[ch]|\.[ch]pp$")
	ADD_SOURCES_HERE(${in_Target} ${__add_cpp_sources_here_Pattern})

endmacro()


#
#	Gets the contents of a directory that match a given filter
#
#	@param out_Result Output variable
#	@param in_RootPath Directory to search in
#   @param in_Pattern Regex pattern used to filter which child paths are returned
#
macro(MATCH_DIRECTORY_CONTENTS out_Result in_RootPath in_Pattern) 
	set(__match_directory_contents_Raw )
	GET_DIRECTORY_CONTENTS(__match_directory_contents_Raw "${in_RootPath}")

	foreach (lfe IN LISTS __match_directory_contents_Raw)
		if ("${lfe}" MATCHES "${in_Pattern}")
			list(APPEND ${out_Result} "${lfe}")
		endif()
	endforeach()
endmacro()
