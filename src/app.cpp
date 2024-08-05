#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "util/util.h"
#include "callback/callback.h"
#include "util/Shader.h"
#include "image-reader/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <string>
#include <cmath>
#include <thread>
#include <chrono>
int main(void)
{
    /**
     * Seting up for the window
     * Use the Glew for dynamically link the opengl
     */
    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(800, 800, "OpenGl", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK)
    {
        std:: cerr << "Error with GLEW\n";
    }
    glViewport(0,0,800,800);
    glfwSetFramebufferSizeCallback(window,&framebuffer_size_callback);
    /**
     * Hardware's specifications
     * Setting stb library to flip the image 
     */
    std:: cout << "Version: "<<glGetString(GL_VERSION)<<'\n';
    int n;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
    std::cout << "Maximum nr of vertex attributes supported: " << n << '\n';
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&n);
    std::cout <<"Number of texture uints: " << n << '\n';
    stbi_set_flip_vertically_on_load(true); 

    float vertices[]
    {
        0.5f , 0.5f , 0.0f , 1.0f , 0.0f, 0.0f, 1.0f, 1.0f, 
        0.5f ,-0.5f , 0.0f , 0.0f , 1.0f, 0.0f, 1.0f, 0.0f, 
       -0.5f ,-0.5f , 0.0f , 0.0f , 0.0f, 1.0f, 0.0f, 0.0f, 
       -0.5f , 0.5f , 0.0f , 0.0f , 1.0f, 1.0f, 0.0f, 1.0f 
    };


    unsigned int indices[]
    {
        0, 1, 2,
        0, 2, 3
    };


    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (void*)vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), (void*)indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);


    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w, h, numberColourChannels;
    unsigned char* data{stbi_load("../textures/t.png", &w, &h, &numberColourChannels, 0)};
    std:: cout << w << ' ' << h << ' ' << numberColourChannels << '\n';
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    util::Shader s{"../src/glsl/vertex_shader.glsl","../src/glsl/fragment_shader.glsl"};



    /**
     * The loop for rendering
     */
    while (!glfwWindowShouldClose(window))
    {
        util::processInput(window);
        /* Render here */
        glClearColor(0.5f,0.5f,0.5f,0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        s.use();
        s.setInt("myTexture", 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);

    glDeleteTextures(1, &texture);

    stbi_image_free(&data);

    glfwTerminate();
    return 0;
}