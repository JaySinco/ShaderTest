#include "utils.h"
#include "shader.h"
#include "mesh.h"
#include "model.h"
#include "texture.h"
#include "camera.h"
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct
{
    struct
    {
        double lastMouseX = 0.0;
        double lastMouseY = 0.0;
        bool leftMousePressed = false;
        bool middleMousePressed = false;
    } trackball;

    const int width = 400;
    const int height = 300;
    gl::Camera camera{float(width) / height};
} g;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam);

int main(int argc, char **argv)
{
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    glfwInit();
    std::shared_ptr<void> glfw_guard(nullptr, [](void *) { glfwTerminate(); });
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(g.width, g.height, "Sample", nullptr, nullptr);
    if (window == nullptr) {
        LOG(ERROR) << "failed to create glfw window";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG(ERROR) << "failed to init glad loader";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glDebugMessageCallback(debug_message_callback, nullptr);

    gl::Shader basicShader;
    if (!basicShader.load(root_DIR L"/shaders/vertex/basic.vert",
                          root_DIR L"/shaders/fragment/basic.frag")) {
        return -1;
    }
    gl::Model model;
    if (!model.load(root_DIR L"/models/backpack/backpack.obj")) {
        return -1;
    }
    gl::Texture diffuse;
    if (!diffuse.load(root_DIR L"/models/backpack/diffuse.jpg", false)) {
        return -1;
    }
    diffuse.use(0);
    basicShader.set("uf_Texture0", 0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        basicShader.set("uf_ModelViewProjectionMatrix",
                        g.camera.getViewProjectionMatrix() * model.getModelMatrix());
        model.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    g.camera.adaptToScreen(width, height);
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    double sensitivity = 0.3;
    g.camera.move(gl::Camera::Forward, yoffset * sensitivity);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        switch (action) {
            case GLFW_PRESS:
                g.trackball.leftMousePressed = true;
                break;
            case GLFW_RELEASE:
                g.trackball.leftMousePressed = false;
                break;
        }
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        switch (action) {
            case GLFW_PRESS:
                g.trackball.middleMousePressed = true;
                break;
            case GLFW_RELEASE:
                g.trackball.middleMousePressed = false;
                break;
        }
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        switch (action) {
            case GLFW_PRESS:
                g.camera.reset();
                break;
        }
    }
}

void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
    float dx = float(xpos - g.trackball.lastMouseX);
    float dy = float(ypos - g.trackball.lastMouseY);
    if (g.trackball.leftMousePressed) {
        g.camera.shake(0.15 * dx);
        g.camera.nod(-0.15 * dy);
    }
    if (g.trackball.middleMousePressed) {
        g.camera.move(gl::Camera::Right, -0.01 * dx);
        g.camera.move(gl::Camera::Up, 0.01 * dy);
    }
    g.trackball.lastMouseX = xpos;
    g.trackball.lastMouseY = ypos;
}

void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam)
{
    LOG(ERROR) << message;
}
