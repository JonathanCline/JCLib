-------------------------------------------------------------------------------

This directory is for jclib internal code generation tools.
These tools are NOT meant for general use and will not be maintained
and tested to the same extent as the rest of jclib.

Lua 5.4 is required to run the codegen lua scripts. This can be
built from source using the code in the github repository at
"https://github.com/lua/lua/tree/master" or similar source repo.

If you wish to use CMake to build the lua source code, I have a fork
set up at "https://github.com/JonathanCline/lua" that provides a CMake
script to build lua from source.

-------------------------------------------------------------------------------

The lua directory contains any lua utility files that are used
in jclib codegen lua scripts. If you are running the code gen,
make sure your lua environment can access these.

Alternatively, you may run the "lua/luaenv.sh" bash script.
This script has the following usage:

    bash luaenv.sh <path/to/your/script.lua>

Lua scripts invokes from this shell script are able to "require()" the
lua files defined in "lua/lib" as that directory is added to the package
library's search path.

-------------------------------------------------------------------------------
