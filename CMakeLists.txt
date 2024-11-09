cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

set(EXPORT_COMPILE_COMMANDS ON)
project(bug-repro VERSION 1.0.0 LANGUAGES CXX)

find_package(opentelemetry-cpp CONFIG REQUIRED)

add_executable(${PROJECT_NAME} repro.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
target_link_libraries(${PROJECT_NAME} PRIVATE opentelemetry-cpp::ostream_log_record_exporter)
