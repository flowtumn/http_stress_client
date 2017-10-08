#include "client/http_stress_client.h"

#include <regex>

using namespace TrainingTask;

void HttpStressClient::doStress(const std::string& host, int port, const std::string& query) {
    if (!this->running_.compare_exchange_weak(false, true)) {
        return{};
    }

    this->clear();

    std::regex pattern(u8"^HTTP/1.1 (.*) (.*) (.*)");

    for (;this->running_;) {
        try {
            if (auto s = TrainingTask::HttpClient <TrainingTask::Socket>{}.doGet(host, port, query)) {
                std::smatch match;

                //doGetに成功したので、中身を確認。(ResponseCodeだけが知りたいので、128byteも読めば十分)
                auto buf = s.read(128).data();
                auto httpResponseHeader = std::string{ buf.data() };

                if (std::regex_search(httpResponseHeader, match, pattern)) {
                    auto responseCode = std::atoi(std::string{ *(match.begin() + 1) });
                    if (200 >= responseCode && responseCode < 300) {
                        //200番台は成功。
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
            std::cerr << e << std::endl;
        }
    }

    this->running_.store(false);
}
