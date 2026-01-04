#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include "bt/common/types.hpp"

namespace bt::data{
    class SymbolTable {
        public:
            SymbolId get_or_add(std::string_view sym);
            std::string_view to_string(SymbolId id) const;

        private:
            std::unordered_map<std::string, SymbolId> map_;
            std::vector<std::string> rev_;
        };

}
