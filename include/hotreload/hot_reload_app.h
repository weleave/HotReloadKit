#pragma once

#include <vector>
#include "hotreload/directory_scanner.h"
#include "hotreload/file_watcher.h"
#include "hotreload/resource_store.h"

namespace hotreload
{
    class HotReloadApp
    {
    public:
        explicit HotReloadApp(fs::path data_root);

        bool initialize();
        std::vector<FileChange> tick();

        void print_summary() const;
        void print_demo_values() const;

        const std::string* get_text(const fs::path& path) const
        {
            return store_.get_text(path);
        }

        const nlohmann::json* get_json(const fs::path& path) const
        {
            return store_.get_json(path);
        }

        const YAML::Node* get_yaml(const fs::path& path) const
        {
            return store_.get_yaml(path);
        }

    private:
        fs::path data_root_;
        DirectoryScanner scanner_;
        FileWatcher watcher_;
        ResourceStore store_;
    };
} // namespace hotreload