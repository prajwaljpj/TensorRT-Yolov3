cmake_minimum_required(VERSION 2.8)
project(demo)

set(CMAKE_BUILD_TYPE Release)

FIND_PACKAGE(PythonInterp 3)
FIND_PACKAGE(PythonLibs 3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors -lboost_python-py36")

#add wrapper
set(TRT_WRAPPER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tensorRTWrapper/code)
add_subdirectory(${TRT_WRAPPER_DIR})
include_directories(${TRT_WRAPPER_DIR}/include)


# PYTHON_ADD_MODULE(inference inference.cpp)
# FILE(COPY inference.py DESTINATION .)
# ADD_TEST(NAME inference_trt COMMAND ${PYTHON_EXECUTABLE} inference.py)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#add opencv
FIND_PACKAGE(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARIES_DIRS})

set(${PYTHON_LIBRARIES} "/home/rbccps2080/.virtualenvs/all/lib/python3.6/config-3.6m-x86_64-linux-gnu/libpython3.6m.so")
set(${PYTHON_INCLUDE_DIRS} "/home/rbccps2080/.virtualenvs/all/include/python3.6m")
set(${PYTHON_INCLUDE_DIR} "/home/rbccps2080/.virtualenvs/all/include/python3.6m")
set(${Boost_LIBRARIES} "/home/rbccps2080/software/boost/stage/lib")
set(${Boost_INCLUDE_DIRS} "/home/rbccps2080/software/boost")

message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIR}")
message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")
message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")

#build runYolov3
add_executable(runYolov3 main_infer.cpp eval.cpp dataReader.cpp inference.cpp)
target_link_libraries(runYolov3 TrtNet ${OpenCV_LIBS})

install(TARGETS runYolov3 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/install/)

ENABLE_TESTING()
# INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIR})
INCLUDE_DIRECTORIES(/home/rbccps2080/software/boost ${PYTHON_INCLUDE_DIR})
LINK_LIBRARIES(/home/rbccps2080/software/boost/stage/lib/*.so ${PYTHON_LIBRARIES}) # Deprecated but so convenient!

PYTHON_ADD_MODULE(inference inference.cpp)
FILE(COPY inference.py DESTINATION .)
ADD_TEST(COMMAND ${PYTHON_EXECUTABLE} inference.py)


#cmake_minimum_required(VERSION 2.8)
#project(demo)

#set(CMAKE_BUILD_TYPE Release)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors")

#set(Boost_INCLUDE_DIR ${Boost_INCLUDE_DIR} "/usr/include/boost")
#set(Boost_LIBRARY_DIR ${Boost_LIBRARY_DIR} "/usr/lib/x86_64-linux-gnu/")
#find_package(PythonInterp REQUIRED)
#find_package(PythonLibs REQUIRED)

#include_directories(${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})
#link_directories(${Boost_LIBRARY_DIR})

#set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -O3;
#                -gencode arch=compute_62,code=sm_62;
#                -x cu)
#include_directories(/usr/local/cuda-10.0/targets/x86_64-linux/include/)
#include_directories(/usr/include/boost/)


##add wrapper
#set(TRT_WRAPPER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tensorRTWrapper/code)
#add_subdirectory(${TRT_WRAPPER_DIR})
#include_directories(${TRT_WRAPPER_DIR}/include)


#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

##add opencv
#FIND_PACKAGE(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})
#link_directories(${OpenCV_LIBRARIES_DIRS})
#message("boost include")
#message(${Boost_INCLUDE_DIR})
#message("python include")
#message(${PYTHON_INCLUDE_DIRS})
#message("boost include")
#message(${Boost_LIBRARY_DIR})

##build runYolov3
## add_executable(runYolov3 main.cpp eval.cpp dataReader.cpp)
## add_executable(runYolov3 main_infer.cpp eval.cpp dataReader.cpp inference.cpp)
## target_link_libraries(runYolov3 TrtNet ${OpenCV_LIBS} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

##PYTHON_ADD_MODULE(inferencelib inference.cpp ${OpenCV_LIBRARIES_DIRS} ${CUDA_LIBRARIES} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
#PYTHON_ADD_MODULE(inferencelib inference.cpp ${OpenCV_LIBRARIES_DIRS} ${CUDA_LIBRARIES} ${Boost_LIBRARY_DIR} ${PYTHON_LIBRARIES})
##FILE(COPY world.py DESTINATION .)
##ADD_TEST(NAME inference_trt COMMAND ${PYTHON_EXECUTABLE} world.py)

## install(TARGETS runYolov3 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/install/)

#set_target_properties(inferencelib PROPERTIES SUFFIX .so)
#set_target_properties(inferencelib PROPERTIES PREFIX "")
