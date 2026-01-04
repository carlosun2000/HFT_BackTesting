#pragma once
#include <cstdint>

namespace bt{
    using SymbolId  = std::uint32_t;
    using OrderId   = std::uint64_t;
    using Timestamp = std::int64_t; 

enum class Side : std::uint8_t { Buy, Sell };

struct Price { std::int64_t ticks; };  // ticks * tick_size
struct Qty   { std::int64_t lots;  };  // lots * lot_size
}