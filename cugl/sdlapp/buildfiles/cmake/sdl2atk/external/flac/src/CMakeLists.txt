cmake_minimum_required(VERSION 3.11)

option(ENABLE_64_BIT_WORDS "Set FLAC__BYTES_PER_WORD to 8, for 64-bit machines. For 32-bit machines, turning this off might give a tiny speed improvement" ON)
option(BUILD_UTILS "Build utils" OFF)

add_subdirectory("libFLAC")
add_subdirectory("share/replaygain_analysis")
add_subdirectory("share/replaygain_synthesis")
add_subdirectory("share/getopt")
add_subdirectory("share/utf8")
add_subdirectory("share/grabbag")