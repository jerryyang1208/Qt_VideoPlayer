# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\VideoPlayer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\VideoPlayer_autogen.dir\\ParseCache.txt"
  "VideoPlayer_autogen"
  )
endif()
