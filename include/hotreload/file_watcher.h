#pragma once

#include <unordered_map>
#include <vector>
#include "hotreload/types.h"

namespace hotreload
{
    class FileWatcher
    {
    public:
        void seed(const std::vector<ResourceMeta>& items);
        std::vector<FileChange> poll(const fs::path& root);

    private:
        std::unordered_map<std::string, fs::file_time_type> known_;
    };
}