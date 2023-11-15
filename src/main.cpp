#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <iostream>
#include <cassert>

#define GL_CHECK(api_call) api_call; assert(glGetError() == GL_NO_ERROR);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    int width = 0;
    int height = 0;
    glfwGetWindowSize(window, &width, &height);

    std::vector<GLubyte> data;
    data.resize(width * height * 3);

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        // data[i] = static_cast<int>(i);
        data[i] = static_cast<GLubyte>(i % 256);
    }

    GLuint tex;
    GL_CHECK(glGenTextures(1, &tex));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, tex));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data()));

    GL_CHECK(GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER));
    static const GLchar* vertex_shader_source = "\
    #version 400\n\
    \
    layout(location = 0) out vec2 vertex_texture_coordinate;\
    \
    void main()\
    {\
        const vec2 vertices[3]=vec2[3](vec2(-1,-1), vec2(3,-1), vec2(-1, 3));\
        \
        gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);\
        \
        vertex_texture_coordinate = 0.5 * gl_Position.xy + vec2(0.5);\
    }\
    \
    ";
    GL_CHECK(glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr));
    GL_CHECK(glCompileShader(vertex_shader));
    {
        GLint info_log_length;
        GL_CHECK(glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &info_log_length));
        std::vector<GLchar> info_log(info_log_length);
        GL_CHECK(glGetShaderInfoLog(vertex_shader, info_log_length, nullptr, info_log.data()));
        for (auto c: info_log)
            std::cout << c;        
        GLint is_compiled;
        GL_CHECK(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled));
        assert(is_compiled == GL_TRUE);
    }

    GL_CHECK(GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER));
    static const GLchar* fragment_shader_source = "\
    #version 400\n\
    \
    uniform sampler2D bitmap;\
    \
    layout(location = 0) in vec2 fragment_texture_coordinate;\
    \
    layout(location = 0) out vec4 framebuffer_color;\
    \
    void main()\
    {\
        framebuffer_color = vec4(texture(bitmap, fragment_texture_coordinate).rgb, 1.0);\
    }\
    \
    ";
    GL_CHECK(glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr));
    GL_CHECK(glCompileShader(fragment_shader));
    {
        GLint info_log_length;
        GL_CHECK(glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_log_length));
        std::vector<GLchar> info_log(info_log_length);
        GL_CHECK(glGetShaderInfoLog(fragment_shader, info_log_length, nullptr, info_log.data()));
        for (auto c: info_log)
            std::cout << c;        
        GLint is_compiled;
        GL_CHECK(glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled));
        assert(is_compiled == GL_TRUE);
    }

    GL_CHECK(GLuint program = glCreateProgram());
    GL_CHECK(glAttachShader(program, vertex_shader));
    GL_CHECK(glAttachShader(program, fragment_shader));
    GL_CHECK(glLinkProgram(program));
    {
        GLint info_log_length;
        GL_CHECK(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length));
        std::vector<GLchar> info_log(info_log_length);
        GL_CHECK(glGetProgramInfoLog(program, info_log_length, nullptr, info_log.data()));
        for (auto c: info_log)
            std::cout << c; 
        GLint is_linked;
        GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, &is_linked));
        assert(is_linked == GL_TRUE);
    }
    GL_CHECK(glUseProgram(program));

    GLuint vao;
    GL_CHECK(glGenVertexArrays(1, &vao));
    GL_CHECK(glBindVertexArray(vao));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

        GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 3));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
