cmake_minimum_required(VERSION 3.13)
project(Sucklab)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Sucklab
        Makefile
        sqserv.h
        SqTCPServ.java
#        squdpclient.c
#        squdpserv.c
        simplecl.c
        simplesv.c)
