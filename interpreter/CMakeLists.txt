cmake_minimum_required(VERSION 3.19)
project(nepl)

set(CMAKE_CXX_STANDARD 20)

link_libraries(gmp gmpxx boost_program_options)

add_executable(nepl main.cpp common.cpp common.h Token.cpp Token.h Lexer.cpp Lexer.h AST.cpp AST.h)