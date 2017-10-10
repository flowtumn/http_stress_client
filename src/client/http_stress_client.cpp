#include "client/http_stress_client.h"

#include <iostream>
#include <regex>
#include "client/http_client.h"

using namespace flowTumn;

void HttpStressClient::stop() {
    auto flg{true};
    if (!this->running_.compare_exchange_weak(flg, false)) {
        return;
    }
}

void HttpStressClient::doStress(const std::string& host, int port, const std::string& query) {
    auto flg{false};
    if (!this->running_.compare_exchange_weak(flg, true)) {
        return;
    }

    this->clear();

    std::regex pattern(u8"^HTTP/1.1 ([0-9]{3})");

    for (;this->running_;) {
        try {
            ++this->total_;
            if (auto s = flowTumn::HttpClient <flowTumn::Socket>{}.doGet(host, port, query)) {
                std::smatch match;

                //doGetに成功したので、中身を確認。(ResponseCodeだけが知りたいので、12byteも読めば十分)
                auto buf = s->read(12);
                auto httpResponseHeader = std::string{ buf.data(), buf.size() };

                if (std::regex_search(httpResponseHeader, match, pattern)) {
                    auto responseCode = std::atoi(std::string{ *(match.begin() + 1) }.c_str());
                    if (200 <= responseCode && responseCode < 400) {
                        //200番、300番台は成功。
                        ++this->success_;
                        continue;
                    }
                }

                //それ以外は失敗とする。
                // (失敗の要因は、readの失敗、Http Responseでは無かった、200番台以外の戻り値だったの3パターンがある)
                ++this->error_;
            } else {
                ++this->failuer_;
            }
        } catch (const std::exception& e){
            //処理自体が失敗。
            ++this->failuer_;
            std::cerr << e.what() << std::endl;
        }
    }
}

