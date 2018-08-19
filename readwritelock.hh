#ifndef __READ_WRITE_LOCK_HH__
#define __READ_WRITE_LOCK_HH__

#include <mutex>
#include <condition_variable>

// A write-preference Read-Write lock
class ReadWriteLock {
    public:
        ReadWriteLock() {
            _nread = _nread_waiters = 0;
            _nwrite = _nwrite_waiters = 0;
        }

        void read_lock() {
            std::unique_lock<std::mutex> lck(_mtx);
            if (_nwrite || _nwrite_waiters) {
                _nread_waiters++;
                while (_nwrite || _nwrite_waiters)
                    _rcond.wait(lck); // calls lck.unlock() inherently, lck.lock() is called after notified.
                _nread_waiters--;
            }
            _nread++;
        }

        void read_unlock() {
            std::unique_lock<std::mutex> lck(_mtx);
            _nread--;
            if (_nwrite_waiters)
                _wcond.notify_one();
        }

        void write_lock() {
            std::unique_lock<std::mutex> lck(_mtx);
            if (_nread || _nwrite) {
                _nwrite_waiters++;
                while (_nread || _nwrite)
                    _wcond.wait(lck);
                _nwrite_waiters--;
            }
            _nwrite++;
        }

        void write_unlock() {
            std::unique_lock<std::mutex> lck(_mtx);
            _nwrite--;
            if (_nwrite_waiters) // write-preference
                _wcond.notify_one();
            else if (_nread_waiters)
                _rcond.notify_all();
        }

    private:
        std::mutex _mtx;
        std::condition_variable _rcond;
        std::condition_variable _wcond;
        uint32_t _nread, _nread_waiters;
        uint32_t _nwrite, _nwrite_waiters;
};

#endif //__READ_WRITE_LOCK_HH__
