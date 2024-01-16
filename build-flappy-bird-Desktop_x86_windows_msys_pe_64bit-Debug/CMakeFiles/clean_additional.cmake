# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\flappy-bird_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\flappy-bird_autogen.dir\\ParseCache.txt"
  "flappy-bird_autogen"
  )
endif()
