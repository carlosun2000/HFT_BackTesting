#include "bt/core/order_manager.hpp"

namespace bt::core{

    OrderId OrderManager::send(SymbolId sym, Side side, Price px, Qty qty,
                           OrderType type, TimeInForce tif, Timestamp ts_ns) {
        OrderId oid = next_oid_++;
        OrderSubmit os;
        os.ts_ns = ts_ns;
        os.sym = sym;
        os.oid = oid;
        os.side = side;
        os.px = px;
        os.qty = qty;
        os.tif = tif;

        Event ev;
        ev.ts_ns = ts_ns;
        ev.type = EventType::OrderSubmit;
        ev.payload = os;

        q_.push(std::move(ev));
        return oid;
        }
        
    void OrderManager::cancel(OrderId oid, Timestamp ts_ns) {
        OrderCancel oc;
        oc.ts_ns = ts_ns;
        oc.oid = oid;

        Event ev;
        ev.ts_ns = ts_ns;
        ev.type = EventType::OrderCancel;
        ev.payload = oc;

        q_.push(std::move(ev));
        }
}