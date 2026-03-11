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

} // namespace hotreload
