#include "bt/data/symbol_table.hpp"
#include <stdexcept>

namespace bt::data{
    SymbolId SymbolTable::get_or_add(std::string_view sym){
        auto it = map_.find(std::string(sym));
        if (it != map_.end()) return it->second;   //second是pair对象的第二个元素，即map的值

        SymbolId id = static_cast<SymbolId>(rev_.size());   //static cast:转换类型
        rev_.push_back(std::string(sym) );
        map_.emplace(rev_.back(),id);
        return id;
    }

    std::string_view SymbolTable::to_string(SymbolId id) const{
        if (id >= rev_.size()) throw std::out_of_range("SymbolId out of range");
        return rev_[id];
    }
    
}