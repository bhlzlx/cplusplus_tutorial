#include <chrono>

namespace cpp {

    class Timer {
        using TimerPoint = decltype( std::chrono::steady_clock::now());
    private:
        TimerPoint  _start;
        uint64_t    _duratoion;
    public:
        Timer() {
            _start = std::chrono::steady_clock::now();
        }
        /**
         * @brief 返回纳秒         * 
         * @return uint64_t 
         */
        uint64_t duration() const {
            auto dur = (std::chrono::steady_clock::now() - _start);
            return dur.count();
        }
    };

}