#
#	Provides handling of configuration files for individual headers within jclib
#
#	Note: the regular "jclib/config.h" file is not handled by the part of the code
#

# Path to the config input root file directory
set(config_input_root_path "${CMAKE_CURRENT_SOURCE_DIR}/in/")

# Path to the config output file directory
set(config_output_root_path "${CMAKE_CURRENT_SOURCE_DIR}/include/jclib/config/")




# Config input file extension, used to generate some regex patterns for config search
set(config_input_file_extension ".cmake")

# Config output file extension, used to preform the auto output file name handling
set(config_output_file_extension ".h")




# Common pattern used for various config file path regexes, this is never used directly
set(base_input_file_pattern )

# Build the input file pattern regex
set(__subsections )
string(REGEX MATCHALL "[^\.]+" __subsections ${config_input_file_extension} )
foreach (sse IN LISTS __subsections)
	string(APPEND base_input_file_pattern "\\." ${sse})
endforeach()


# Config input file regex match pattern
set(input_file_pattern "${base_input_file_pattern}")
string(APPEND input_file_pattern "$")
string(PREPEND input_file_pattern ".*")




#
#	Finds the configuration input files
#
#	@param out_ConfigPaths	Paths output variable, this will be a list of the paths found
#	@param in_RootDir		Root directory to search from
#
macro(FIND_CONFIG_INPUT_FILES out_ConfigPaths)

	# Get all files from the config header directory
	set(__configFiles )
	file(GLOB_RECURSE __configFiles "${config_input_root_path}/*")
	
	# Find configuration input files "*.in.h" 
	foreach(lfe IN LISTS __configFiles)
	
		# Check for valid input file path
		if ("${lfe}" MATCHES "${input_file_pattern}")

			# Add path to output variable list
			list(APPEND ${out_ConfigPaths} "${lfe}")
			
		endif()

	endforeach()

endmacro()



#
#	Gets the appropriate output path for a config input file
#
#	@param out_path Result out variable
#	@param in_inputPath The config input path
#
macro(CONFIG_OUTPUT_PATH out_path in_inputPath)

	# Compute the relative path to the input from the root path
	set(__config_output_path_Relative )
	file(RELATIVE_PATH __config_output_path_Relative "${config_input_root_path}" "${in_inputPath}")

	# Get just the file path without the extension
	set(__retval)
	string(REGEX MATCH "[a-zA-Z\\/]+" __retval "${__config_output_path_Relative}")
	
	# Add the new extension and prepend the output root pth
	string(APPEND __retval "${config_output_file_extension}")
	string(PREPEND __retval "${config_output_root_path}/")
	
	# Return value
	set(${out_path} ${__retval})
endmacro(CONFIG_OUTPUT_PATH)

#
#	Adds a new configuration file
#
#	@param configPath ABSOLUTE path to the config input file
#
function(ADD_CONFIG_FILE_FN configPath)
	set(output_path )
	CONFIG_OUTPUT_PATH(output_path ${configPath})
	configure_file("${configPath}" "${output_path}")
endfunction()


#
#	Adds a new configuration file
#
#	@param configPath path to the config input file
#
macro(ADD_CONFIG_FILE configPath)
	ADD_CONFIG_FILE_FN("${CMAKE_CURRENT_LIST_DIR}/${configPath}")
endmacro()




set(_config_files )
FIND_CONFIG_INPUT_FILES(_config_files)
foreach (lfe IN LISTS _config_files)
	include("${lfe}")
endforeach()
