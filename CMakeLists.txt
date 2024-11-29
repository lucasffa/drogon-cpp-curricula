cmake_minimum_required(VERSION 3.10)
project(Backend)

set(CMAKE_CXX_STANDARD 17)

# Encontrar pacotes necessários
find_package(Drogon REQUIRED)
find_package(bsoncxx REQUIRED CONFIG HINTS /usr/local/lib/cmake/bsoncxx-4.0.0)
find_package(mongocxx REQUIRED CONFIG HINTS /usr/local/lib/cmake/mongocxx-4.0.0)


# Adicionar diretórios de inclusão
include_directories(include)
include_directories(src)
include_directories(/usr/local/include/bsoncxx/v_noabi)
include_directories(/usr/local/include/mongocxx/v_noabi)


# Criar o executável
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

# Vincular bibliotecas
target_link_libraries(Backend
    PRIVATE
    Drogon::Drogon
    /usr/local/lib/libbsoncxx.so
    /usr/local/lib/libmongocxx.so
)
