#include "bt/core/event_queue.hpp"

namespace bt::core{

    void EventQueue::push(Event ev){
        pq_.push(Node{std::move(ev),seq_++});
    }
    bool EventQueue::empty() const noexcept {
        return pq_.empty();
    }
    std::size_t EventQueue::size() const noexcept {
        return pq_.size();
    }
    Event EventQueue::pop() {
        Node n = std::move(const_cast<Node&>(pq_.top()));
        pq_.pop();
        return std::move(n.ev);
    }
}
