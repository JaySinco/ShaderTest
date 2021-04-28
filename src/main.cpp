#define UNICODE
#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include "pandaFramework.h"
#include "pandaSystem.h"

int main(int argc, char **argv)
{
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    PandaFramework framework;
    framework.open_framework(argc, argv);
    framework.set_window_title("Sample");
    WindowFramework *window = framework.open_window();
    window->setup_trackball();
    NodePath scene = window->load_model(framework.get_models(), "models/environment");
    scene.reparent_to(window->get_render());
    scene.set_scale(0.25f, 0.25f, 0.25f);
    scene.set_pos(-8, 42, 0);
    framework.main_loop();
    framework.close_framework();
    return 0;
}
