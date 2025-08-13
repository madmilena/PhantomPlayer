# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/phantom_player_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/phantom_player_autogen.dir/ParseCache.txt"
  "phantom_player_autogen"
  )
endif()
