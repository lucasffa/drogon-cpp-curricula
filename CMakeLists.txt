cmake_minimum_required(VERSION 3.10)
project(Backend)

set(CMAKE_CXX_STANDARD 17)

find_package(Drogon REQUIRED)
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

include_directories(include)
include_directories(src)
include_directories(/usr/local/include/mongocxx/v_noabi)
include_directories(/usr/local/include/bsoncxx/v_noabi)

add_executable(Backend
    main.cpp
    src/Application.cpp
    src/Routes.cpp
    src/Controllers.cpp
    src/Services.cpp
    src/Database.cpp
    src/Validators.cpp
    src/OpenAIClient.cpp
)

target_link_libraries(Backend
    Drogon::Drogon
    ${MONGOCXX_LIBRARIES}
    ${BSONCXX_LIBRARIES}
)
