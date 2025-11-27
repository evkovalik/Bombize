#ifndef BMBZ_GAME_TIME_TIMERITEM_HPP
#define BMBZ_GAME_TIME_TIMERITEM_HPP

#include <boost/asio.hpp>
#include <chrono>
#include <functional>

namespace Bmbz
{
namespace Game
{
    class TimerItem
    {
    public:
        TimerItem(const boost::asio::strand<boost::asio::any_io_executor>& strand, int id)
        : _timer(strand)
        , _id(id)
        {}

        inline bool Busy() const noexcept { return _busy; }
        inline int Id() const noexcept { return _id; }

        void Run(const std::chrono::milliseconds& time, std::function<void()> handler)
        {
            if(_busy) return;

            _busy = true;
            _timer.expires_after(time);
            _handler = std::move(handler);
            _timer.async_wait([this](const boost::system::error_code& e){ OnTimeout(e); });
        }

        void Reset()
        {
            _timer.cancel();
            _busy = false;
        }

    private:
        int _id;
        boost::asio::steady_timer _timer;
        std::function<void()> _handler = nullptr;
        bool _busy = false;

        void OnTimeout(const boost::system::error_code& error)
        {
            if(!error)
            {
                _handler();
            }
            _busy = false;
        }
    };
}
}
#endif