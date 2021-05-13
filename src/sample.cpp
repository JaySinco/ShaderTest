#include "utils.h"
#include "shader.h"
#include "model.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "material.h"
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
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
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
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG(ERROR) << "failed to init glad loader";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glDebugMessageCallback(debug_message_callback, nullptr);

    gl::Shader phong;
    if (!phong.load(root_DIR L"/shaders/vertex/phong.vert",
                    root_DIR L"/shaders/fragment/phong.frag")) {
        return -1;
    }

    gl::Model model;
    // if (!model.load(root_DIR L"/models/cyborg/cyborg.obj")) {
    //     return -1;
    // }
    // model.zoom(0.55, 0.55, 0.55);
    // model.move(0.0, -1.0, 0.0);
    if (!model.load(root_DIR L"/models/planet/planet.obj")) {
        return -1;
    }
    model.zoom(0.25, 0.25, 0.25);

    gl::Material material;
    material.color = glm::vec4(0.53, 0.8, 0.92, 1.0);
    // material.diffuse = std::make_shared<gl::Texture>();
    // if (!material.diffuse->load(root_DIR L"/models/cyborg/cyborg_diffuse.png", true)) {
    //     return -1;
    // }
    // material.specular = std::make_shared<gl::Texture>();
    // if (!material.specular->load(root_DIR L"/models/cyborg/cyborg_specular.png", true)) {
    //     return -1;
    // }
    material.shininess = 0.25 * 128;

    gl::Light light0;
    light0.type = gl::Light::Ambient;
    light0.color = glm::vec4(0);

    gl::Light light1;
    light1.type = gl::Light::Direct;
    light1.color = glm::vec4(glm::vec3(0.8), 1.0);
    light1.direction = glm::vec3(-1, -1, 0);

    gl::Light light2;
    light2.type = gl::Light::Point;
    light2.color = glm::vec4(glm::vec3(0.8), 1.0);
    light2.position = glm::vec3(0, 5, 3);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        material.use(phong);
        light0.use(phong, g.camera, 0);
        light1.use(phong, g.camera, 1);
        model.draw(phong, g.camera);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam)
{
    LOG(ERROR) << message;
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
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            switch (action) {
                case GLFW_PRESS:
                    g.trackball.leftMousePressed = true;
                    break;
                case GLFW_RELEASE:
                    g.trackball.leftMousePressed = false;
                    break;
            }
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            switch (action) {
                case GLFW_PRESS:
                    g.trackball.middleMousePressed = true;
                    break;
                case GLFW_RELEASE:
                    g.trackball.middleMousePressed = false;
                    break;
            }
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            switch (action) {
                case GLFW_PRESS:
                    g.camera.reset();
                    break;
            }
            break;
    }
}

void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
    float dx = float(xpos - g.trackball.lastMouseX);
    float dy = float(ypos - g.trackball.lastMouseY);
    if (g.trackball.middleMousePressed) {
        g.camera.shake(0.15 * dx);
        g.camera.nod(-0.15 * dy);
    }
    if (g.trackball.leftMousePressed) {
        g.camera.move(gl::Camera::Right, -0.01 * dx);
        g.camera.move(gl::Camera::Up, 0.01 * dy);
    }
    g.trackball.lastMouseX = xpos;
    g.trackball.lastMouseY = ypos;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    static bool wireframe = false;
    switch (key) {
        case GLFW_KEY_L:
            switch (action) {
                case GLFW_PRESS:
                    wireframe = !wireframe;
                    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
                    break;
            }
            break;
    }
}
