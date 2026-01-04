#pragma once
#include <cstdint>
#include <string_view>
#include "bt/common/types.hpp"

namespace bt::data{
    enum class EventType : std::uint8_t { Quote, Trade };

    struct Quote{
        Timestamp ts_ns{};
        SymbolId  sym{};
        Price     bid_px{};
        Price     ask_px{};
        Qty       bid_sz{};
        Qty       ask_sz{};
    };

    enum class Aggressor : std::uint8_t { Unknown, Buy, Sell };

    struct Trade {
        Timestamp ts_ns{};
        SymbolId  sym{};
        Price     trade_px{};
        Qty       trade_sz{};
        Aggressor side{Aggressor::Unknown};
        };

    struct Event {
        EventType type{};
        union {
            Quote q;
            Trade t;
        };  //q 和 t不会同时存在
        Event() : type(EventType::Quote), q{} {}
    };
}