#include "utils.h"
#include "shader.h"
#include "mesh.h"
#include "model.h"
#include "texture.h"
#include "camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const int g_Width = 400;
const int g_Height = 300;
const float g_InitCameraZ = 3.0f;

double g_LastMouseX = 0.0;
double g_LastMouseY = 0.0;
bool g_LMousePressed = false;

gl::Model g_Model;
gl::Camera g_Camera(static_cast<float>(g_Width) / g_Height);

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    double sensitivity = 0.3;
    g_Camera.move(gl::Camera::Forward, yoffset * sensitivity);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        switch (action) {
            case GLFW_PRESS:
                g_LMousePressed = true;
                break;
            case GLFW_RELEASE:
                g_LMousePressed = false;
                break;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        switch (action) {
            case GLFW_PRESS:
                g_Model.reset();
                g_Camera.moveTo(0, 0, g_InitCameraZ);
                break;
        }
    }
}

void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (g_LMousePressed) {
        float dx = float(xpos - g_LastMouseX);
        float dy = float(ypos - g_LastMouseY);
        float dz = std::hypot(dx, dy);
        g_Model.spin(float(0.15 * dz), dy, dx, 0);
    }
    g_LastMouseX = xpos;
    g_LastMouseY = ypos;
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

    GLFWwindow *window = glfwCreateWindow(g_Width, g_Height, "Sample", nullptr, nullptr);
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
        LOG(ERROR) << "failed to init glad";
        return -1;
    }

    gl::Shader basicShader;
    if (!basicShader.load(root_DIR L"/shaders/vertex/basic.vert",
                          root_DIR L"/shaders/fragment/basic.frag")) {
        LOG(ERROR) << "failed to load shader: basic";
        return -1;
    }
    if (!g_Model.load(root_DIR L"/models/backpack/backpack.obj")) {
        return -1;
    }
    gl::Texture diffuse;
    if (!diffuse.load(root_DIR L"/models/backpack/diffuse.jpg")) {
        return -1;
    }
    diffuse.use(0);
    basicShader.set("uf_Texture0", 0);
    g_Camera.moveTo(0, 0, g_InitCameraZ);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        basicShader.set("uf_ModelViewProjectionMatrix",
                        g_Camera.getViewProjectionMatrix() * g_Model.getModelMatrix());
        g_Model.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
