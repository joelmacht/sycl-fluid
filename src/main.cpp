#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <iostream>
#include <cassert>

#define GL_CHECK(api_call) { api_call; assert(glGetError() == GL_NO_ERROR);}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GL_CHECK({});
    static const GLchar* inline_vertex_shader = "\
    #version 460\
    \
    void main()\
    {\
    }\
    \
    ";
    GL_CHECK(glShaderSource(vertex_shader, 1, &inline_vertex_shader, nullptr));
    GL_CHECK(glCompileShader(vertex_shader));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
