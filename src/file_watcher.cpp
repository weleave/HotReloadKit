//
// 怎么知道文件变了，用last_write_time
// 比较新增/修改/删除
//

#include "hotreload/file_watcher.h"

namespace hotreload
{
    void hotreload::FileWatcher::seed(const std::vector<ResourceMeta>& items)
    {
        known_.clear();
        for (const auto& item : items)
        {
            known_[normalize_key(item.path)] = item.last_write_time;
        }
    }

    std::vector<FileChange> FileWatcher::poll(const fs::path& root)
    {
        std::vector<FileChange> changes;
        std::unordered_map<std::string, fs::file_time_type> current;

        if (fs::exists(root) && fs::is_directory(root))
        {
            for (const auto& entry : fs::recursive_directory_iterator(root))
            {
                if (!entry.is_regular_file()) { continue; }

                const auto& path = entry.path();
                if (!is_supported(path)){ continue; }

                const auto key = normalize_key(path);
                const auto time = fs::last_write_time(path);
                current[key] = time;

                auto it = known_.find(key);
                if (it == known_.end())
                {
                    changes.push_back({path, ChangeType::Added});
                }
                else if (it->second != time)
                {
                    changes.push_back({path, ChangeType::Modified});
                }
            }
        }

        for (const auto& [key, _] : known_)
        {
            if (current.find(key) == current.end())
            {
                changes.push_back({fs::path(key), ChangeType::Removed});
            }
        }

        known_ = std::move(current);
        return changes;
    }

} // namespace hotreload



