# Enables ADD_GIT_DEPENDENCY functionality
option(ENABLE_GIT_DEPENDENCIES "Enables automatic cloning of dependencies that do not already exit" OFF)


find_package(Git QUIET)




#
#	Adds a new dependency and automatically clones it if the target does not already exist.
#
#	@param depPath Where to clone the repo into, must be an absolute path!
#	@param depTarget Name of the target, this is used to check if the dependency already exists
#	@param depRepo Path or URL to clone the repo from
#	@param branchName? Name of the branch to clone, defaults to HEAD
#
function(ADD_GIT_DEPENDENCY_FN depPath depTarget depRepo)

	# Ignore if target already exists
	if (NOT TARGET ${depTarget})
	 
		# Only preform branch if git dependencies are allowed
		if (ENABLE_GIT_DEPENDENCIES)
		
			set(gitResult )

			# Use branch optional parameter if it was provided
			if (ARGC GREATER 3)
				execute_process(COMMAND
					${GIT_EXECUTABLE} clone -b "${ARGV3}" ${depRepo} ${depPath}
					RESULTS_VARIABLE gitResult
					COMMAND_ERROR_IS_FATAL ANY)
			else()
				execute_process(COMMAND
					${GIT_EXECUTABLE} clone ${depRepo} ${depPath}
					RESULTS_VARIABLE gitResult
					COMMAND_ERROR_IS_FATAL ANY)
			endif()

			# Check result
			

			# Add the cloned repo as a subdir if it has CMake support
			if (EXISTS "${depPath}/CMakeLists.txt")
				add_subdirectory("${depPath}")
			
				# Check that dependency target is now defined
				if (NOT TARGET ${depTarget})
					message(FATAL "Cloned dependency has a CMakeLists but the dependency target was not defined!")
				endif()
				
			endif()

		endif()

	endif()
endfunction()

#
#	Adds a new dependency and automatically clones it if the target does not already exist.
#
#	@param depPath Relative path to clone the repo into
#	@param depTarget Name of the target, this is used to check if the dependency already exists
#	@param depRepo Path or URL to clone the repo from
#	@param branchName? Name of the branch to clone, defaults to HEAD
#
macro(ADD_GIT_DEPENDENCY depPath depTarget depRepo)

	# Make file path absolute
	set(__addgitdepedency_realpath )
	file(REAL_PATH ${depPath} __addgitdepedency_realpath)
	
	# Determine invocation syntax
	if (${ARGC} GREATER 3)
		# Invoke with branchName parameter
		ADD_GIT_DEPENDENCY_FN(${__addgitdepedency_realpath} ${depTarget} ${depRepo} ${ARGV3})
	else()
		# Invoke without branchName parameter
		ADD_GIT_DEPENDENCY_FN(${__addgitdepedency_realpath} ${depTarget} ${depRepo})
	endif()
endmacro()


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
	set(__get_directory_contents_Path )
	file(REAL_PATH ${in_DirectoryPath} __get_directory_contents_Path)

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