#include <atomic>
#include <iostream>
#include <type_traits>
#include <iterator>
#include <regex>
#include <thread>
#include "socket/socket.h"

class GetClient {
public:
    bool doGet(
        std::string host,
        int port,
        std::function <bool()> exiter
    ) {
        TrainingTask::Socket s;
        if (s.initialize()) {
            std::regex pattern(u8"^HTTP\/1\.1 (.*) (.*) (.*)");
            std::smatch match;
            if (s.connect(host, port)) {
                //while (!exiter())
                {
                    auto req = toString(
                        "GET", " / ", "HTTP/1.1",
                        "\r\nHost: ", host,
                        "\r\n\r\n"
                    );

                    auto ss = s.write(toByteBuf(req));
                    auto pp = s.read(10000);
                    auto httpResponseHeader = std::string{ pp.data() };

                    if (std::regex_search(httpResponseHeader, match, pattern)) {
                        auto responseCode = std::string{ *(match.begin() + 1) };
                        ++this->successCounter_;
                    }
                }
            }
        }
        return false;
    }

    constexpr auto successCount() const {
        return this->successCounter_;
    }

private:
    //TrainingTask::Socket socket_;
    int64_t successCounter_{ 0 };
};


constexpr auto isSuccess(int responseCode) {
    return (200 <= responseCode && 300 > responseCode);
}

void usage() {
    std::cerr << "tcp [host] [exec time(ms)]" << std::endl;
}

void fs() {
}

void observer(int responseCode) {
    std::cout << "responseCode: " << responseCode << std::endl;
}

int main(int argc, char** argv) {
    if (3 != argc) {
        usage();
        return EXIT_FAILURE;
    }

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
    
    
    
    //URLを分解
    std::regex pattern(u8"(.*):\/\/(.*)\/(.*)$");
}
