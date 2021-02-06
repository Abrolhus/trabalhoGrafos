cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_executable(teste
    Node.cpp
    Graph.cpp
    Edge.cpp
    main.cpp)
