#include "utils.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE) {
        switch (action) {
            case GLFW_PRESS:
                glfwSetWindowShouldClose(window, true);
                break;
            case GLFW_RELEASE:
                break;
        }
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char **argv)
{
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    glfwInit();
    std::shared_ptr<void> glfw_guard(nullptr, [](void *) { glfwTerminate(); });
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(400, 300, "Sample", nullptr, nullptr);
    if (window == nullptr) {
        LOG(ERROR) << "failed to create glfw window";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    // glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetCursorPosCallback(window, cursor_pos_callback);
    // glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG(ERROR) << "failed to init glad";
        return -1;
    }

    gl::Shader basicShader;
    if (!basicShader.load(root_DIR L"/shaders/vertex/basic.vert",
                          root_DIR L"/shaders/fragment/basic.frag")) {
        LOG(ERROR) << "failed to load shader: basic";
        return -1;
    }

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };
    gl::Mesh shape;
    shape.load(sizeof(vertices) / sizeof(float), vertices, sizeof(indices) / sizeof(unsigned),
               indices);

    gl::Texture container, awesomeface;
    if (!container.load(root_DIR L"/images/container.jpg")) {
        LOG(ERROR) << "failed to load texture: container";
        return -1;
    }
    if (!awesomeface.load(root_DIR L"/images/awesomeface.png")) {
        LOG(ERROR) << "failed to load texture: awesomeface";
        return -1;
    }
    container.use(0);
    awesomeface.use(1);

    basicShader.set("u_Texture0", 0);
    basicShader.set("u_Texture1", 1);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        basicShader.use();
        shape.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
