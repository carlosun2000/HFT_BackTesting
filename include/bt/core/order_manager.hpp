#pragma once 
#include <cstdint>
#include "bt/core/event.hpp"
#include "bt/core/event_queue.hpp"

namespace bt::core{
    class OrderManager{
        public: 
            explicit OrderManager(EventQueue& q): q_(q) {}
            OrderId send(SymbolId sym, Side side, Price px, Qty qty,
               OrderType type = OrderType::Limit,
               TimeInForce tif = TimeInForce::GTC,
               Timestamp ts_ns = 0);
            
            void cancel(OrderId oid, Timestamp ts_ns=0);

        private:
            EventQueue& q_;
            OrderId next_oid_{1};
    };
}