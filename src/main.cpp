#include <atomic>
#include <thread>
#include "client/http_stress_client.h"

auto execute(const std::string& host, int port, const std::string& query) {
    auto client = std::make_unique <TrainingTask::HttpStressClient> ();
    return client;
}

int main(int argc, char** argv) {
    if (3 != argc) {
        usage();
        return EXIT_FAILURE;
    }

    std::string host{ argv[1] };
    auto times = std::chrono::milliseconds{ std::atoi(argv[2]) };

    //URLを分解
    std::regex pattern(u8"(.*)://(.*)/(.*)$");

    std::smatch match;
    std::string h{"http://www.yahoo.co.jp/"};
                    if (std::regex_search(h, match, pattern)) {
for (auto each : match) {
    std::cout << each << std::endl;
}
                    }


for (;;) {
auto r = TrainingTask::HttpClient <TrainingTask::Socket> {}.doGet(
"www.yahoo.co.jp"
, 80
, "/testtesttest"
);

auto rr =std::move(r);
}
    return 0;


    std::string host{ argv[1] };
    auto times = std::chrono::milliseconds{ std::atoi(argv[2]) };

    std::atomic <bool> stop_{ false };
    std::atomic <int64_t> counter_{ 0 };

    std::vector <std::thread> threads_;
    auto N = std::thread::hardware_concurrency();
    auto subscribe = [&counter_](int count) {
        counter_ = counter_ + count;
    };

    std::generate_n(
        std::back_inserter(threads_),
        N,
        [&stop_, &subscribe] {
            return std::thread{
                [&stop_, &subscribe] {
                    GetClient client;
                    while (!stop_) {
#if 1
                        client.doGet(
                            //"www.yahoo.co.jp",
                            "www.apple.com",
                            80,
                            [&stop_] {
                                return stop_.load();
                            }
                        );
#else
                        subscribe(
                            func(
                                [&stop_] {
                                    return stop_.load();
                                }
                            )
                        );
#endif
                    }
                    subscribe(client.successCount());
                }
            };
        }
    );

    //待機。
    std::this_thread::sleep_for(times);

    //終了
    stop_.store(true);
    std::for_each(
        std::begin(threads_),
        std::end(threads_),
        [](auto&& thr) {
            thr.join();
        }
    );

    std::cout << "Counter:  " << std::to_string(counter_) << std::endl;
    //全てのClientに完了を通知。
    return 0;
}

