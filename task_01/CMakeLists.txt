cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Получаем все исходные файлы
file(GLOB_RECURSE source_list "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB test_source_list "${CMAKE_CURRENT_SOURCE_DIR}/src/*test.cpp")

# Убираем тесты из основного списка
list(REMOVE_ITEM source_list ${test_source_list})

include_directories(${PROJECT_NAME} PUBLIC src include)  # <-- ДОБАВИЛ include

add_executable(${PROJECT_NAME} ${source_list})

# Locate GTest
enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

find_library(Utils ../)
target_link_libraries(${PROJECT_NAME} PUBLIC Utils)

# ========== ИСПРАВЛЕНИЕ ДЛЯ ТЕСТОВ ==========
# Добавляем ВСЕ исходные файлы (кроме main.cpp) в тесты
file(GLOB all_cpp_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM all_cpp_files "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")  # Убираем main

# Теперь тесты включают ВСЕ .cpp файлы + тестовые файлы
add_executable(${PROJECT_NAME}_tests 
    ${all_cpp_files}        # <-- ВСЕ исходники
    ${test_source_list}     # <-- Тесты
)

target_link_libraries(
  ${PROJECT_NAME}_tests
  GTest::gtest_main
  Utils
)

target_include_directories(${PROJECT_NAME}_tests 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include  # <-- ВАЖНО!
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)