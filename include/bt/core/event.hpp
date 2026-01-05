#pragma once
#include <cstdint>
#include <variant>

#include "bt/common/types.hpp"
#include "bt/data/schema.hpp"

namespace bt::core{
    enum class OrderType : std::uint8_t {Limit, Market};
    enum class TimeInForce: std::uint8_t {GTC, IOC};

    struct OrderSubmit {
        Timestamp ts_ns{};
        OrderId oid{};
        SymbolId sym{};
        Side side{};
        OrderType type{OrderType::Limit};
        Price px{};
        Qty qty{};
        TimeInForce tif{TimeInForce::GTC};
    };

    struct OrderCancel {
        Timestamp ts_ns{};
        OrderId oid{};
    };

    struct Fill{
        Timestamp ts_ns{};
        OrderId oid{};
        SymbolId sym{};
        Side side{};
        Price px{};
        Qty qty{};
    };

    struct Timer {           //引擎内部定时器事件，例如每一秒触发一次
        Timestamp ts_ns{};
        std::uint32_t timer_id{};
    };

    enum class EventType : std::uint8_t{
        Market,
        OrderSubmit,
        OrderCancel,
        Fill,
        Timer
    };

    using Payload = std::variant<
    bt::data::Event,
    OrderSubmit,
    OrderCancel,
    Fill,
    Timer
    >;

    struct Event {
        Timestamp ts_ns{};
        EventType type{};
        Payload payload{};
    };
}