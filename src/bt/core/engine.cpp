#include "bt/core/engine.hpp"
#include "bt/core/order_manager.hpp"

#include <stdexcept>

namespace bt::core{
    Engine::Engine(bt::data::CsvLoader& loader,
               IStrategy& strategy,
               IExchangeSim& exchange,
               IPortfolio& portfolio,
               Config cfg)
        : loader_(loader),
            strategy_(strategy),
            exchange_(exchange),
            portfolio_(portfolio),
            cfg_(cfg) {}

    void Engine::pump_market_until_queue_nonempty_or_eof(){
        while (q_.empty()){
            auto md = loader_.next();
            if (!md) return;

            Event ev;
            ev.ts_ns = (md->type ==bt::data::EventType::Quote) ? md->q.ts_ns : md->t.ts_ns;
            ev.type = EventType::Market;
            ev.payload = *md;
            q_.push(std::move(ev));
            return ;
        }
    }

    void Engine::run(){
        OrderManager om(q_);
        om_ = &om;
        strategy_.on_start(*om_);
        std::size_t processed = 0;

        while (true){
            if (q_.empty()){
                pump_market_until_queue_nonempty_or_eof();
                if (q_.empty()){
                    if (cfg_.stop_on_loader_eof) break;
                }
            }
            if (q_.empty()){
                break;
            }
            Event ev = q_.pop();
            now_ = ev.ts_ns;

            switch (ev.type){
                case EventType::Market:{
                    const auto & md = std::get<bt::data::Event>(ev.payload);
                    //先整合更新市场状态
                    exchange_.on_market(md,q_);
                    //再让策略读取行情并决定是否下单
                    strategy_.on_market(md,*om_);
                } break; 

                case EventType::OrderSubmit:{
                    const auto& os = std::get<OrderSubmit>(ev.payload);
                    exchange_.on_order_submit(os,q_);
                } break; 

                case EventType::OrderCancel:{
                    const auto& oc = std::get<OrderCancel>(ev.payload);
                    exchange_.on_order_cancel(oc,q_);
                } break; 
                
                case EventType::Fill: {
                    const auto& fill = std::get<Fill>(ev.payload);
                    portfolio_.on_fill(fill);
                    strategy_.on_fill(fill, *om_);
                } break;

                case EventType::Timer:{
                    const auto& timer = std::get<Timer>(ev.payload);
                    strategy_.on_timer(timer, *om_);
                } break;

            }
            if (cfg_.max_events != 0 && ++processed >= cfg_.max_events) break;
  
        }
        strategy_.on_end(*om_);
    }
    
}

