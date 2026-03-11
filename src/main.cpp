#include <chrono>
#include <iostream>
#include <thread>

#include "hotreload/hot_reload_app.h"

namespace fs = std::filesystem;

static fs::path resolve_data_root()
{
    if (fs::exists("data")) { return "data"; }
    if (fs::exists("../data")) { return "../data"; }
    return "data";
}

int main()
{
    fs::path data_root = resolve_data_root();

    hotreload::HotReloadApp app(data_root);
    if (!app.initialize())
    {
        return 1;
    }

    app.print_summary();
    app.print_demo_values();

    std::cout << "\n=== 热加载演示开始 ===\n";
    std::cout << "文件所在位置：\n";
    std::cout << "1. data/resources/welcome.txt\n";
    std::cout << "2. data/configs/app.json\n";
    std::cout << "3. data/configs/render.yaml\n\n";

    using namespace std::chrono_literals;
    for (int i = 0; i < 30; ++i)
    {
        auto changes = app.tick();
        if (!changes.empty())
        {
            app.print_summary();
            app.print_demo_values();
        }

        std::this_thread::sleep_for(1s);
    }

    std::cout << "\n=== 演示结束 ===\n";
    return 0;
}