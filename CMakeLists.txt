cmake_minimum_required(VERSION 3.9)
project(Tensorflow)

set(CMAKE_CXX_STANDARD 11)

add_executable(Tensorflow main.cpp Client.cpp Client.h GraphNode.h StaticParams.h Master.cpp Master.h CalculateWorker.cpp CalculateWorker.h Psworker.cpp Psworker.h)