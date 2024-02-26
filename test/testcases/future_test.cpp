#include <future>
#include <thread>

#include "gtest/gtest.h"

class Watcher {
public:
    Watcher() {
        begin_timestamp_us_ = std::chrono::steady_clock::now().time_since_epoch().count() / 1000;
    }

    int64_t duration_us() const {
        return std::chrono::steady_clock::now().time_since_epoch().count() / 1000 - begin_timestamp_us_;
    }

private:
    int64_t begin_timestamp_us_ = 0;
};

TEST(future, test_async) {
    Watcher w;
    std::future<int> f = std::async(std::launch::async, []()-> int {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return 3;
    });
    int v = f.get();
    ASSERT_GE(w.duration_us(), 1000 * 1000);
    ASSERT_EQ(v, 3);
}

TEST(future, test_promise) {
    std::promise<int> p;
    std::future<int> f = p.get_future();

    Watcher w;
    std::thread th([&p]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        p.set_value(3);
    });

    int v = f.get();
    if (th.joinable()) {
        th.join();
    }

    ASSERT_GE(w.duration_us(), 1000 * 1000);
    ASSERT_EQ(v, 3);
}