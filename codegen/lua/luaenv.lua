--
--  Lua script to add the jclib lua lib directory to the package manager
--

-- CONSTANTS

-- Path to the jclib lua lib dir, relative to this script
local LIBRARY_PATH <const> = "lib/"

-- The search path entry to add to the global package search paths
local LIBRARY_SEARCH_PATH <const> = LIBRARY_PATH .. "?.lua"


--
--  END OF CONSTANTS
--

-- Get cli args
local _args = { ... }

-- Handle args
if #_args < 2 then
    print("ERROR : missing the path to the folder this lua script is in. Use --help for usage")
    return -1
elseif _args[1] == "--help" or _args[2] == "--help" then
    print("usage = luaenv.lua <path/to/this/script's/folder> <path/to/script/to/run.lua> [<args...>]")
    return 0
end

local function add_path_to_env(_luaFolderPath)

    -- Create the full search path
    local ABSOLUTE_LIBRARY_SEARCH_PATH <const> = _luaFolderPath .. "/" .. LIBRARY_SEARCH_PATH

    print("Added \"" .. ABSOLUTE_LIBRARY_SEARCH_PATH .. "\" to search paths")

    -- Add lua lib folder path to package search paths
    _G.package.path = _G.package.path .. ";" .. ABSOLUTE_LIBRARY_SEARCH_PATH

end

add_path_to_env(_args[1])

-- Invoke the script that was asked to be invoked from the cli args
local _fn, _err = loadfile(_args[2], "bt")
if not _fn then
    error(_err)
    return -1
else
    -- Invoke the script
    return _fn(table.unpack(_args, 3))
end
