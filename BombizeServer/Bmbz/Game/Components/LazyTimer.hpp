#ifndef BMBZ_GAME_COMPONENT_LAZYTIMER_HPP
#define BMBZ_GAME_COMPONENT_LAZYTIMER_HPP

#include <chrono>

namespace Bmbz
{
namespace Game
{
namespace Component
{
    class LazyTimer
    {
    public:
        LazyTimer(const std::chrono::milliseconds& waitTime, bool isRunning = false)
        : _waitTime(waitTime)
        {
            if(isRunning) Run();
        }

        void Run() noexcept
        {
            _startTime = std::chrono::steady_clock::now();
            _isRunning = true;
        }
        void Stop() noexcept { _isRunning = false; }
        void Continue() noexcept
        {
            if(!_isRunning && _passed > std::chrono::milliseconds(0))
            {
                _isRunning = true;
            }
        }

        bool IsRunning() const noexcept { return _isRunning; }

        void Reset(bool isRunning = false) noexcept
        {
            _passed = std::chrono::milliseconds(0);
            if(isRunning) Run();
        }

        void WaitTime(const std::chrono::milliseconds& newTime)
        {
            _waitTime = newTime;
        }

        void DoTick()
        {
            if(_isRunning)
            {
                auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - _startTime
                );
                _passed += dt;
                _startTime = std::chrono::steady_clock::now();
            }
        }

        bool Completed()
        {
            DoTick();
            return _passed >= _waitTime;
        }

        const std::chrono::milliseconds& Passed() const noexcept { return _passed; }
        const std::chrono::milliseconds Remains() const noexcept { return _waitTime - _passed; }

    private:
        std::chrono::milliseconds _waitTime;
        std::chrono::milliseconds _passed{0};
        std::chrono::time_point<std::chrono::steady_clock> _startTime;
        bool _isRunning = false;
    };
}
}
}
#endif