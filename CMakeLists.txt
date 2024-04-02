cmake_minimum_required(VERSION 3.27)
project(kongroo C)

set(CMAKE_C_STANDARD 11)

add_executable(kongroo main.c
        bytecode.h
        stack.h
        stack.c
        virtual-machine.c
        virtual-machine.h
        codesource.c
        codesource.h
        compiler.c
        compiler.h
        vector.c
        vector.h
        buffer.c
        buffer.h
        ast.c
        ast.h
        debug.h
        globals.h
        debug.c)
