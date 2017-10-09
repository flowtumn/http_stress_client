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

    std::string url{ argv[1] };
    auto times = std::chrono::milliseconds{ std::atoi(argv[2]) };
    std::string host{};
    std::string query{};
    int port = 80;

    //URLを分解
    std::regex pattern(u8"(.*)://(.*)/(.*)$");

    std::smatch match;
    if (std::regex_search(url, match, pattern)) {
		auto protocol = *(std::begin(match) + 1);
        if ("http" != protocol) {
            std::cerr << "Unsupport protocol " << protocol << std::endl;
            return EXIT_FAILURE;
        }

        host = *(std::begin(match) + 2);
        query ="/" + std::string{ *(std::begin(match) + 3)};
    } else {
        //分解出来なければ、host名のみとし扱い、GETパスは"/"。
        host = url;
        query = "/";
    }


    //論理コア数分のClientを生成。
    std::vector <std::unique_ptr <flowTumn::HttpStressClient>> clients;
    std::generate_n(
        std::back_inserter(clients),
//        std::thread::hardware_concurrency(),
		8,
        [] {
            return std::make_unique <flowTumn::HttpStressClient> ();
        }
    );

    //生成したClientと同数のThreadを生成しexecute
    std::vector <std::thread> threads_;
    for (auto& each : clients) {
        threads_.emplace_back(
            [&each, host, port, query] {
//                execute(*each, host, port, query);
				each->doStress(host, port, query);
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
        for (auto& each : clients) {
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

