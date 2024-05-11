cmake_minimum_required(VERSION 3.0.2)
project(test_socket)

find_package(OpenCV REQUIRED)

include_directories(
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node src/PgmToPng.cpp)

target_link_libraries(${PROJECT_NAME}_node
  ${OpenCV_LIBS}
)

add_executable(yaml src/ReadWriteYaml.cpp)

target_link_libraries(yaml
  ${OpenCV_LIBS}
)

add_executable(filter src/Filter_Avg.cpp)

target_link_libraries(filter
  ${OpenCV_LIBS}
)