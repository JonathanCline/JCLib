#!

#
#   This is a helper script that uses the system "lua.exe" to run a script
#   but ensures that the libraries defined for jclib codegen are available
#   to be "require"(ed).
#

# The name of the lua redirect script
LUAENV_SCRIPT=luaenv.lua

# Determine the path to this script
SCRIPT_PATH=$0

# Determine the path to this script's parent directory
SCRIPT_DIRECTORY_PATH=${SCRIPT_PATH%/*.sh}

# The arguments passed to the lua env script
LUA_SCRIPT_ARGS=""
ARGPARSE_REGEX="[^ ]+ [^ ]+"
for arg in "$@"; do
    if [[ $arg =~ $ARGPARSE_REGEX ]]; then
        LUA_SCRIPT_ARGS="$LUA_SCRIPT_ARGS \"$arg\""
    else
        LUA_SCRIPT_ARGS="$LUA_SCRIPT_ARGS $arg"
    fi
done

# Invoke lua script to create environment 
lua $SCRIPT_DIRECTORY_PATH/$LUAENV_SCRIPT $SCRIPT_DIRECTORY_PATH $LUA_SCRIPT_ARGS
