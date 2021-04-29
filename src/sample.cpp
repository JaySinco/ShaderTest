#define UNICODE
#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include "prec.h"

PT(Shader) loadShader(std::string vert, std::string frag)
{
    return Shader::load(Shader::SL_GLSL, "shaders/vertex/" + vert + ".vert",
                        "shaders/fragment/" + frag + ".frag");
}

int main(int argc, char **argv)
{
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    load_prc_file("panda3d-prc-file.prc");

    PandaFramework framework;
    framework.open_framework(argc, argv);
    framework.set_window_title("Sample");
    WindowFramework *window = framework.open_window();
    window->setup_trackball();

    NodePath millScene =
        window->load_model(framework.get_models(), "eggs/mill-scene/mill-scene.bam");
    PT(Shader) basicShader = loadShader("basic", "basic");
    millScene.set_shader(basicShader);
    millScene.reparent_to(window->get_render());

    framework.main_loop();
    framework.close_framework();
    return 0;
}
