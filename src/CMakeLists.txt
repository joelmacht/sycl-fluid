add_executable(viewer main.cpp ${GLFW_SOURCE_DIR}/deps/glad_gl.c)

find_package(OpenGL REQUIRED)

target_link_libraries(viewer PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(viewer PRIVATE glfw)

target_include_directories(viewer PRIVATE ${GLFW_SOURCE_DIR}/deps)
