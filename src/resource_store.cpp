/*
* 扫描到文件之后，怎么把它们加载进内存
* 加载 .txt
* 解析 .json
* 解析 .yaml/.yml
* 提供查询接口
 */

#include "hotreload/resource_store.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace hotreload
{
    bool ResourceStore::load_one(const fs::path& path)
    {
        if (!fs::exists(path) || !fs::is_regular_file(path))
        {
            std::cerr << "[load_one] file not found: " << path << "\n";
            return false;
        }

        const auto kind = detect_kind(path);
        if (kind == ResourceKind::Unknown)
        {
            return false;
        }

        const auto key = normalize_key(path);

        try
        {
            if (kind == ResourceKind::Text)
            {
                std::ifstream ifs(path);
                if (!ifs)
                {
                    std::cerr << "[load_one] failed to open text: " << path << "\n";
                    return false;
                }

                std::ostringstream oss;
                oss << ifs.rdbuf();
                text_map_[key] = oss.str();
            }
            else if (kind == ResourceKind::JsonConfig)
            {
                std::ifstream ifs(path);
                if (!ifs)
                {
                    std::cerr << "[load_one] failed to open json: " << path << "\n";
                    return false;
                }

                nlohmann::json j;
                ifs >> j;
                json_map_[key] = std::move(j);
            }
            else if (kind == ResourceKind::YamlConfig)
            {
                YAML::Node node = YAML::LoadFile(path.string());
                yaml_map_[key] = node;
            }
            else
            {
                std::cerr << "[load_one] kind not found: " << path << "\n";
                return false;
            }

            ResourceMeta meta(path, kind, fs::last_write_time(path));
            metas_[key] = std::move(meta);

            std::cout << "[load_one] loaded: " << path << " (" << to_string(kind) << ")\n";
            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "[load_one] failed: " << path << ", reason: " << e.what() << "\n";
            return false;
        }
        return false;
    }

    void ResourceStore::remove_one(const fs::path& path)
    {
        const auto key = normalize_key(path);
        if (metas_.find(key) == metas_.end())
        {
            std::cout << "[remove_one] key not in metas_" << "\n";
        }
        metas_.erase(key);

        if (text_map_.find(key) == text_map_.end())
        {
            std::cout << "[remove_one] key not in text_map_" << "\n";
        }
        text_map_.erase(key);

        if (json_map_.find(key) == json_map_.end())
        {
            std::cout << "[remove_one] key not in json_map_" << "\n";
        }
        json_map_.erase(key);

        if (yaml_map_.find(key) == yaml_map_.end())
        {
            std::cout << "[remove_one] key not in yaml_map_" << "\n";
        }
        yaml_map_.erase(key);

        std::cout << "[remove_one] removed: " << path << "\n";
    }

    const std::string* ResourceStore::get_text(const fs::path& path) const
    {
        const auto key = normalize_key(path);
        auto it = text_map_.find(key);
        return (it == text_map_.end())? nullptr: &it->second;
    }

    const nlohmann::json* ResourceStore::get_json(const fs::path& path) const
    {
        const auto key = normalize_key(path);
        auto it = json_map_.find(key);
        return (it == json_map_.end())? nullptr: &it->second;
    }

    const YAML::Node* ResourceStore::get_yaml(const fs::path& path) const
    {
        const auto key = normalize_key(path);
        auto it = yaml_map_.find(key);
        return (it == yaml_map_.end())? nullptr: &it->second;
    }

    void ResourceStore::print_summary() const
    {
        std::cout << "[summary] text = " << text_count()
                  << ", json = " << json_count()
                  << ", yaml = " << yaml_count()
                  << ", total = " << total_count()
                  << "\n";
    }

} // namespace hotreload
