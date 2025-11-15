cmake_minimum_required(VERSION 3.20)
project(MediSchedule VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(MediSchedule
    src/main.cpp
)

if (MSVC)
  target_compile_options(MediSchedule PRIVATE /W4 /permissive-)
else()
  target_compile_options(MediSchedule PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Output to bin/
set_target_properties(MediSchedule PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
