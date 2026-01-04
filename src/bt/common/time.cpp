#include "bt/common/time.hpp"
#include <chrono>

namespace bt{
    Timestamp wall_time_ns(){
        using clock = std::chrono::high_resolution_clock;
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
           clock::now().time_since_epoch()
         ).count();
    }
    
    Timestamp to_ns(double sec) {
        return static_cast<Timestamp>(sec * 1e9);
        }
    
}