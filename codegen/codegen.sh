# Helper bash script for invoking the codgen.lua script

# Determine the path to this script
ERROR_TRAP=""
SCRIPT_PATH=$(echo $PWD/$0 | tr \\ / 2>/dev/null)

# Determine path to this script's parent directory
SCRIPT_DIRECTORY_PATH=${SCRIPT_PATH%/*.sh}

# Determine path to luaenv shell script
LUAENV_PATH=$SCRIPT_DIRECTORY_PATH/lua/luaenv.sh

# The arguments passed to the lua env script
LUAENV_ARGS=""
ARGPARSE_REGEX="[^ ]+ [^ ]+"
for arg in "$@"; do
    if [[ $arg =~ $ARGPARSE_REGEX ]]; then
        LUAENV_ARGS="$LUAENV_ARGS \"$arg\""
    else
        LUAENV_ARGS="$LUAENV_ARGS $arg"
    fi
done


# Launch the codegen.lua script using the jclib lua environment
exec $LUAENV_PATH $SCRIPT_DIRECTORY_PATH/codegen.lua $LUAENV_ARGS
