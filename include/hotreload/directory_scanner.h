#pragma once

#include <vector>
#include "hotreload/types.h"

namespace hotreload
{
    class DirectoryScanner
    {
    public:
        std::vector<ResourceMeta> scan(const fs::path& root) const;
    };
} // namespace hotreload