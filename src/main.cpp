#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <regex>
#include "client/http_stress_client.h"

template <typename ... Args>
auto execute(flowTumn::HttpStressClient& client, Args&&... args) {
    return client.doStress(std::forward <Args> (args)...);
}

void usage() {
    std::cerr << "http_stress_client [host] [exec time(ms)]" << std::endl;
}

int main(int argc, char** argv) {
    if (3 != argc) {
        usage();
        return EXIT_FAILURE;
    }

    std::string host{ argv[1] };
    auto times = std::chrono::milliseconds{ std::atoi(argv[2]) };
    std::string query{};
    int port = 80;

    //URLを分解
    std::regex pattern(u8"(.*)://(.*)/(.*)$");

    std::smatch match;
    std::string h{"http://www.yahoo.co.jp/"};
    if (std::regex_search(h, match, pattern)) {
        for (auto each : match) {
            std::cout << each << std::endl;
        }
    } else {
        //分解出来なければ、host名のみとして処理。
        query = "/";
    }

    //論理コア数分のClientを生成。
    std::vector <std::unique_ptr <flowTumn::HttpStressClient>> clients;
    std::generate_n(
        std::back_inserter(clients),
        std::thread::hardware_concurrency(),
        [] {
            return std::make_unique_ptr <flowTumn::HttpStressClient> ();
        }
    );

    //生成したClientと同数のThreadを生成しexecute
    std::vector <std::thread> threads_;
    for (auto each : clients) {
        threads_.emplace_back(
            [&each, host, port, query] {
                execute(**each, host, port, query);
            }
        );
    }

    //待機。
    std::this_thread::sleep_for(times);

    //Shutdown.
    std::for_each(
        std::begin(clients),
        std::end(clients),
        [](auto& each) {
            each->stop();
        }
    );

    //Join
    std::for_each(
        std::begin(threads_),
        std::end(threads_),
        [](auto&& thr) {
            thr.join();
        }
    );

    //Report.
    std::cout << "Result: " << host << "\n";
    std::cout << "--------------------------------------\n";
    {
        int64_t total{0};
        int64_t success{0};
        int64_t error{0};
        int64_t fail{0};
        for (auto each : clients) {
            total = total + each->total();
            success = success + each->success();
            error = error + each->error();
            fail = fail + each->failuer();
        }

        std::cout << "Total Execute: " << total << "\n";
        std::cout << "      Success: " << success << "\n";
        std::cout << "        Error: " << error << "\n";
        std::cout << "         Fail: " << fail << "\n";
    }

    return EXIT_SUCCESS;
}
