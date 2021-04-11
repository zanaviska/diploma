cmake_minimum_required(VERSION 3.19)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
 
add_executable(${PROJECT_NAME} main.cpp flowline.cpp functions.cpp terminal.cpp )

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})