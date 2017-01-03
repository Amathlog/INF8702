cmake_minimum_required(VERSION 3.5)

project(Project_INF8702)

add_definitions(-std=c++11 -g)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})



file(

        GLOB_RECURSE

        source_files

        src/*

)

file(

        GLOB_RECURSE

        include_files

        include/*

)

include_directories(./include)

add_executable(

        Project_INF8702

        ${source_files}

        ${include_files}

)

target_link_libraries(Project_INF8702 ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_STATIC_LIBRARIES})
