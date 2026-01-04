#pragma once
#include <cstdint>
#include <string_view>
#include <cstdlib>
#include <cmath>
#include <string>
#include "bt/common/types.hpp"
#include "bt/data/schema.hpp"

namespace bt::data{
    inline std::int64_t parse_i64(std::string_view s) {
        return std::strtoll(std::string(s).c_str(),nullptr,10);
    }

    inline std::int64_t parse_i32(std::string_view s) {
        return static_cast<std::int32_t>(std::strtol(std::string(s).c_str(),nullptr,10));
    }

    inline Price parse_price_ticks(std::string_view s, double tick_size =0.01){
        double px = std::strtod(std::string(s).c_str(), nullptr);
        return Price{ static_cast<std::int64_t>(std::llround(px / tick_size)) };
    }
    inline Qty parse_qty_lots(std::string_view s) {
        return Qty{ parse_i64(s) };
    }
    
    inline Aggressor parse_side(std::string_view s) {
        if (s.empty()) return Aggressor::Unknown;
        char c = s[0];
        if (c == 'B') return Aggressor::Buy;
        if (c == 'S') return Aggressor::Sell;
        return Aggressor::Unknown;
    }
}