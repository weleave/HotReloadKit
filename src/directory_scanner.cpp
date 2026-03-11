/*
* 有哪些文件需要管理
 * 给定一个根目录
 * 递归扫描里面所有支持的文件
 * 返回资源列表
 *
 * 得到
 * 哪些文件被识别成 JSON
 * 哪些文件被识别成 YAML
 * 哪些文件被识别成文本资源
 */

#include "hotreload/directory_scanner.h"

#include <iostream>

namespace hotreload
{
    std::vector<ResourceMeta> DirectoryScanner::scan(const fs::path& root) const
    {
        std::vector<ResourceMeta> result;

        if (!fs::exists(root) || !fs::is_directory(root)) { return result; }

        for (const auto& entry : fs::recursive_directory_iterator(root))
        {
            const fs::path& path = entry.path();
            if (!is_supported(path))
            {
                continue;
            }

            ResourceMeta meta(path, detect_kind(path), fs::last_write_time(path));
            result.push_back(std::move(meta));
        }

        return result;
    }

} // namespace hotreload
