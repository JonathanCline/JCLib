#
#	Algorithm configuration support CMake
#

# Only relevent when compiling for C++14/17
option(JCLIB_CONSTEXPR_ALGORIHMS "Enables custom constexpr versions of std algorithms, this may affect performance" ON)



ADD_CONFIG_FILE("algorithm.in.h")
