#pragma once
#include <cstdint>
#include <queue>
#include <vector>
#include "bt/core/event.hpp"

namespace bt::core{

    class EventQueue{
        public: 
            EventQueue() = default;

            void push(Event ev);
            bool empty() const noexcept;
            std::size_t size() const noexcept;
            Event pop();

        private:
            struct Node{
                Event ev;
                std::uint64_t seq;
            };
            struct Cmp{
                bool operator() (const Node &a, const Node &b){
                    if (a.ev.ts_ns !=b.ev.ts_ns) return a.ev.ts_ns > b.ev.ts_ns;
                    return a.seq > b.seq;
                }
            };
            std::priority_queue<Node, std::vector<Node>, Cmp> pq_;
            std::uint64_t seq_ = 0;

    };

}