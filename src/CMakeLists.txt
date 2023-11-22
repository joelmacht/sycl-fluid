add_executable(viewer main.cpp)

find_package(OpenGL REQUIRED)

target_link_libraries(viewer PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(viewer PRIVATE glad)
target_link_libraries(viewer PRIVATE glfw)