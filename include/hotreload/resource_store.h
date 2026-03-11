#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

#include "hotreload/types.h"

namespace hotreload
{
    class ResourceStore
    {
    public:
        bool load_one(const fs::path& path);
        void remove_one(const fs::path& path);

        const std::string* get_text(const fs::path& path) const;
        const nlohmann::json* get_json(const fs::path& path) const;
        const YAML::Node* get_yaml(const fs::path& path) const;

        void print_summary() const;

        std::size_t text_count() const { return text_map_.size(); }
        std::size_t json_count() const { return json_map_.size(); }
        std::size_t yaml_count() const { return yaml_map_.size(); }
        std::size_t total_count() const { return metas_.size(); }

    private:
        std::unordered_map<std::string, ResourceMeta> metas_;
        std::unordered_map<std::string, std::string> text_map_;
        std::unordered_map<std::string, nlohmann::json> json_map_;
        std::unordered_map<std::string, YAML::Node> yaml_map_;
    };
} // namespace hotreload