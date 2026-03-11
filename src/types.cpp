/*
* 先把最基础的数据结构定义出来：
* 资源类型 ResourceKind
* 文件变化类型 ChangeType
* 资源元信息 ResourceMeta
* 文件变化事件 FileChange
 */

#include "hotreload/types.h"

namespace hotreload
{
    std::string to_string(ResourceKind kind)
    {
        switch (kind)
        {
            case ResourceKind::Text: return "Text";
            case ResourceKind::JsonConfig: return "JsonConfig";
            case ResourceKind::YamlConfig: return "YamlConfig";
            default: return "Unknown";
        }
    }

    std::string to_string(ChangeType type)
    {
        switch (type)
        {
            case ChangeType::Added: return "Added";
            case ChangeType::Modified: return "Modified";
            case ChangeType::Removed: return "Removed";
            default: return "Added";
        }
    }

    ResourceKind detect_kind(const fs::path& path)
    {
        const auto ext = path.extension().string();
        if (ext == ".txt") return ResourceKind::Text;
        if (ext == ".json") return ResourceKind::JsonConfig;
        if (ext == ".yaml" || ext == ".yml") return ResourceKind::YamlConfig;
        return ResourceKind::Unknown;
    }

    bool is_supported(const fs::path& path) // 判断是否支持
    {
        return detect_kind(path) != ResourceKind::Unknown;
    }

    std::string normalize_key(const fs::path& path)
    {
        // fs::absolute(path) 将相对路径转换为绝对路径
        // .lexically_normal() 对路径进行词法规范化，去掉多余的.或者合并分隔符
        // .generic_string() 把路径转换为统一格式字符串，\改为/等
        return fs::absolute(path).lexically_normal().generic_string();
    }

} // namespace hotreload
