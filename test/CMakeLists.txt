# Подключаем googletest через FetchContent
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

# Создаем тестовый исполняемый файл
add_executable(ip_filter_tests
    test_ip_filter.cpp
)

# Связываем тесты с нашей библиотекой и gtest
target_link_libraries(ip_filter_tests
    ip_filter_lib
    gtest_main
)

# Включаем директории с заголовками
target_include_directories(ip_filter_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

# Добавляем тесты в CTest
include(GoogleTest)
gtest_discover_tests(ip_filter_tests)