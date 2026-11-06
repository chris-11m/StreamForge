#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <chrono>

namespace streamforge {

template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(std::size_t capacity)
        : capacity_(capacity) {

        if (capacity_ == 0) {
            throw std::invalid_argument(
                "Queue capacity must be greater than zero"
            );
        }
    }

    
    bool push(const T& item) {
        std::unique_lock<std::mutex> lock(mutex_);

        // Measure waiting only when the queue is full.
        if (queue_.size() >= capacity_ && !closed_) {
            ++producer_wait_count_;

            auto wait_start = std::chrono::steady_clock::now();

            not_full_cv_.wait(lock, [&] {
                return queue_.size() < capacity_ || closed_;
            });

            auto wait_end = std::chrono::steady_clock::now();

            producer_wait_time_ += wait_end - wait_start;
        }

        if (closed_) {
            return false;
        }

        queue_.push(item);

        // Record the highest queue size observed.
        if (queue_.size() > peak_queue_size_) {
            peak_queue_size_ = queue_.size();
        }

        lock.unlock();
        not_empty_cv_.notify_one();

        return true;
    }


    
    bool pop(T& item) {
        std::unique_lock<std::mutex> lock(mutex_);

        not_empty_cv_.wait(lock, [&] {
            return !queue_.empty() || closed_;
        });

        if (queue_.empty()) {
            return false;
        }

        item = std::move(queue_.front());
        queue_.pop();

        lock.unlock();
        not_full_cv_.notify_one();

        return true;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }

        not_full_cv_.notify_all();
        not_empty_cv_.notify_all();
    }

        
    std::size_t peakQueueSize() {
        std::lock_guard<std::mutex> lock(mutex_);
        return peak_queue_size_;
    }

    std::size_t producerWaitCount() {
        std::lock_guard<std::mutex> lock(mutex_);
        return producer_wait_count_;
    }

    std::chrono::nanoseconds producerWaitTime() {
        std::lock_guard<std::mutex> lock(mutex_);
        return producer_wait_time_;
    }

   



private:
    std::queue<T> queue_;

    std::size_t capacity_;

    std::mutex mutex_;

    std::condition_variable not_empty_cv_;
    std::condition_variable not_full_cv_;

    bool closed_ = false;

    std::size_t peak_queue_size_ = 0;
    std::size_t producer_wait_count_ = 0;
    std::chrono::nanoseconds producer_wait_time_{0};
};

} // namespace streamforge