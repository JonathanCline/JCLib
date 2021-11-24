local format = require("format")

local _args = { ... }

local function make_feature_test_code(_name, _macro, _value)
    local _jclibFeatureValueMacroName = "JCLIB_FEATURE_VALUE_" .. _name
    local _jclibFeatureMacroName = "JCLIB_FEATURE_" .. _name
    return format([[

/*
    Test for {2}
*/

#define {4} {3}
#if JCLIB_CPP >= {4} || __cpp_lib_constexpr_algorithms >= {4}
    #define {5}
#else
    #ifdef {5}
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef {5}
    #define {5}_V true
#else
    #define {5}_V false
#endif

]], _name, _macro, _value, _jclibFeatureValueMacroName, _jclibFeatureMacroName)
end


-- Handle arguements
if #_args == 0 then
    print("no arguements provided! use --help to get usage")
elseif _args[0] == "--help" then
    print([[usage
<path/to/feature_list.lua> <path/to/output_file.h>
]])
    return
end

-- Set input and output file path values
local _featureListFilePath = _args[1]
local _featureHeaderOutputPath = _args[2] or "feature.h"

-- Open output file
local _outputFile = io.open(_featureHeaderOutputPath, "w+")
_outputFile:write([[// generated using codegen/feature_gen.lua
#pragma once
#ifndef JCLIB_FEATURE_H
#define JCLIB_FEATURE_H

/*
	Copyright 2021 Jonathan Cline
	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
	(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	JCLib feature c++ testing encapsulation header. This exists partially because certain feature c++ testing macros
	are imply different features available depending on their value.
*/

#define _JCLIB_FEATURE_

#define JCLIB_CPP __cplusplus

]])

-- Adds a feature checking block to output
function new(_name, _cppFeatureTestMacro, _requiredValue)
    _outputFile:write(make_feature_test_code(_name, _cppFeatureTestMacro, _requiredValue))
end

-- Get the feature list table from the script
local _featureList = dofile(_featureListFilePath)

-- Add footer to output file
_outputFile:write([[
    
#endif
]])
_outputFile:flush()