#pragma once
#include <optional>
#include "bt/core/event_queue.hpp"
#include "bt/data/csv_loader.hpp"

namespace bt::core{
    class OrderManager; //forward declaration 告诉编译器有这么一个东西，细节以后再说
    //order manager的作用：规范化订单，链接Startegy和exchange/simulator
    class IStrategy{   //策略接口：只关注market,fill,timer，下单通过order manager
        public:
            virtual ~IStrategy() = default;
            virtual void on_start(OrderManager& om){}
            virtual void on_market(const bt::data::Event& md, OrderManager& om){}
            virtual void on_fill(const Fill& fill, OrderManager& om) {}
            virtual void on_timer(const Timer& timer, OrderManager& om) {}
            virtual void on_end(OrderManager& om) {}
    };

    class IExchangeSim{ //交易所sim接口，处理订单事件和市场事件，产生fill
        public:
            virtual ~IExchangeSim() = default;
            virtual void on_market(const bt::data::Event & md, EventQueue &q) = 0;
            virtual void on_order_submit(const OrderSubmit& os, EventQueue& q) = 0;
            virtual void on_order_cancel(const OrderCancel& oc, EventQueue& q) = 0;
        };

    class IPortfolio{
        public:
            virtual ~IPortfolio() = default;
            virtual void on_fill(const Fill&fill) = 0;
    };

    class Engine{
        public: 
            struct Config{
                bool stop_on_loader_eof = true; //当行情数据源（loader）读到eof是立即停止还是把剩余事件跑完
                std::size_t max_events = 0;  //0 表示没有limit
            };
            Engine(
                bt::data::CsvLoader & loader,
                IStrategy& strategy,
                IExchangeSim& exchange,
                IPortfolio& portfolio,
                Config cfg
            );
            void run();
            Timestamp now() const noexcept {return now_;}
        private:
            void pump_market_until_queue_nonempty_or_eof();

            bt::data::CsvLoader& loader_;
            IStrategy& strategy_;
            IExchangeSim& exchange_;
            IPortfolio& portfolio_;
            Config cfg_;

            EventQueue q_;
            Timestamp now_{0};
            OrderManager* om_{nullptr};
    };
}