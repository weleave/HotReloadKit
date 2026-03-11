//
// 总控类把前面内容拼接起来
// 初次扫描
// 初次价值
// 轮询变化
// 重新加载
// 打印日志
//

#include "hotreload/hot_reload_app.h"
#include <iostream>

namespace hotreload
{
    HotReloadApp::HotReloadApp(fs::path data_root)
    {
        this->data_root_ = data_root;
    }

    bool HotReloadApp::initialize()
    {
        if (!fs::exists(data_root_) || !fs::is_directory(data_root_))
        {
            std::cerr << "[initialize] data root not found: " << data_root_ << "\n";
            return false;
        }

        auto items = scanner_.scan(data_root_);
        watcher_.seed(items);

        std::cout << "[initialize] found " << items.size() << " supported files\n";
        for (const auto& item : items)
        {
            store_.load_one(item.path);
        }
        return true;
    }

    std::vector<FileChange> HotReloadApp::tick()
    {
        auto changes = watcher_.poll(data_root_);

        for (const auto& change : changes)
        {
            std::cout << "[tick] " << to_string(change.type) << ": " << change.path << "\n";

            if (change.type == ChangeType::Removed)
            {
                store_.remove_one(change.path);
            }
            else
            {
                store_.load_one(change.path);
            }
        }
        return changes;
    }

    void HotReloadApp::print_demo_values() const
    {
        const auto text_path = data_root_ / "resources" / "welcome.txt";
        const auto json_path = data_root_ / "configs" / "app.json";
        const auto yaml_path = data_root_ / "configs" / "render.yaml";

        if (const auto* text = store_.get_text(text_path))
        {
            std::cout << "[demo] welcome.txt => " << *text << "\n";
        }

        if (const auto* json = store_.get_json(text_path))
        {
            if (json->contains("window_title"))
            {
                std::cout << "[demo] app.json.window_title => " <<
                    (*json)["window_title"].get<std::string>() << "\n";
            }
        }

        if (const auto* yaml = store_.get_yaml(text_path))
        {
            if ((*yaml)["vsync"])
            {
                std::cout << "[demo] render.yaml.vsync => " <<
                    (*yaml)["vsync"].as<bool>() << "\n";
            }
        }
    }

    void HotReloadApp::print_summary() const
    {
        store_.print_summary();
    }

} // namespace hotreload
