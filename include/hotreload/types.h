//
// Created by Administrator on 2026/3/11.
//

#pragma once

#include <filesystem>
#include <string>

namespace hotreload
{
    namespace fs = std::filesystem;

    enum class ResourceKind // 资源类型
    {
        Text,
        JsonConfig,
        YamlConfig,
        Unknown
    };

    enum class ChangeType   // 文件变化类型
    {
        Added,
        Modified,
        Removed
    };

    struct  ResourceMeta // 资源元信息
    {
        fs::path path;
        ResourceKind kind{ResourceKind::Unknown};
        fs::file_time_type last_write_time{};
        ResourceMeta(fs::path path_, ResourceKind kind_, fs::file_time_type last_write_time_) :
            path(path_), kind(kind_), last_write_time(last_write_time_) { }
    };

    struct FileChange // 文件变化事件
    {
        fs::path path;
        ChangeType type{ChangeType::Modified};
    };

    std::string to_string(ResourceKind kind);
    std::string to_string(ChangeType type);

    ResourceKind detect_kind(const fs::path& path);
    bool is_supported(const fs::path& path);
    std::string normalize_key(const fs::path& path);

} // namespace hotreload




