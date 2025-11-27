#ifndef BMBZ_GAME_TIME_TIMEMANAGER_HPP
#define BMBZ_GAME_TIME_TIMEMANAGER_HPP

#include <list>
#include "TimerItem.hpp"

namespace Bmbz
{
namespace Game
{
    class TimeManager
    {
    public:
        TimeManager(const boost::asio::strand<boost::asio::any_io_executor>& strand)
        : _strand(strand)
        {}

        int AsyncWait(const std::chrono::milliseconds& time, std::function<void()> handler)
        {
            auto it = std::find_if(_timers.begin(), _timers.end(), [](const TimerItem& timer){ return !timer.Busy(); });
            if(it != _timers.end())
            {
                it->Run(time, std::move(handler));
                return it->Id();
            }
            else
            {
                _timers.emplace_back(TimerItem(_strand, _nextTimerId)).Run(time, std::move(handler));
                return _nextTimerId++;
            }
        }

        void ResetAsyncWait(int timerId)
        {
            for(auto& timer : _timers)
                if(timer.Id() == timerId)
                {
                    timer.Reset();
                    return;
                }
        }

    private:
        const boost::asio::strand<boost::asio::any_io_executor>& _strand;
        std::list<TimerItem> _timers;
        int _nextTimerId = 1;
    };
}
}
#endif