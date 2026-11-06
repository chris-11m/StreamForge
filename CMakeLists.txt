cmake_minimum_required(VERSION 3.20)
project(StreamForge VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_library(streamforge INTERFACE)
target_include_directories(streamforge INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(streamforge_demo src/main.cpp)
target_link_libraries(streamforge_demo PRIVATE streamforge)
enable_testing()
add_executable(event_tests tests/event_tests.cpp)
target_link_libraries(event_tests PRIVATE streamforge)
add_test(NAME event_tests COMMAND event_tests)

# Concurrent sensor processing example
add_executable(sensor_pipeline
    src/sensor_pipeline.cpp
    src/SensorProcessor.cpp
    src/SensorSimulator.cpp
    src/AnomalyDetector.cpp
)

target_include_directories(sensor_pipeline PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(sensor_pipeline PRIVATE streamforge)

add_executable(test_bounded_queue
    tests/test_bounded_queue.cpp
)

target_include_directories(test_bounded_queue PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(test_bounded_queue PRIVATE streamforge)

add_test(
    NAME bounded_queue
    COMMAND test_bounded_queue
)

add_executable(test_sensor_simulator
    tests/test_sensor_simulator.cpp
    src/SensorSimulator.cpp
)

target_link_libraries(test_sensor_simulator
    PRIVATE streamforge
)

add_test(
    NAME sensor_simulator
    COMMAND test_sensor_simulator
)


add_executable(test_anomaly_detector
    tests/test_anomaly_detector.cpp
    src/AnomalyDetector.cpp
)

target_link_libraries(test_anomaly_detector
    PRIVATE streamforge
)

add_test(
    NAME anomaly_detector
    COMMAND test_anomaly_detector
)

add_executable(test_event_json
    tests/test_event_json.cpp
    src/AnomalyDetector.cpp
)

target_link_libraries(test_event_json PRIVATE streamforge)

add_test(
    NAME event_json
    COMMAND test_event_json
)