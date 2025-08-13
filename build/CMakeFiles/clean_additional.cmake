# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/spotify_clone_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/spotify_clone_autogen.dir/ParseCache.txt"
  "spotify_clone_autogen"
  )
endif()
