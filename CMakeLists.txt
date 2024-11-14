#设置最低版本的 cmake
cmake_minimum_required(VERSION 3.10)
#设置项目名称
project(ray_tracing_rander)

#设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#设置编译选项
add_compile_options(-Wall -O3) 

#查找所有源文件
file(GLOB SOURCES "src/*.cpp")

include_directories(src)

#添加可执行文件
add_executable(demo ${SOURCES})

#链接外部库
# target_link_libraries(${PROJECT_NAME} pthread)

