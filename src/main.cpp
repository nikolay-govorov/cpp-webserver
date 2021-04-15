#include <memory>
#include <fstream>
#include <iostream>
#include <cstring>
#include <ctime>

#include <evhttp.h>

#include "config.h"

void LogRequest(evhttp_request *req) {
    auto uri = evhttp_request_get_uri(req);

    time_t now = time(nullptr);
    auto time = strtok(ctime(&now), "\n");

    std::cout << "[" << time << "]: " << uri << std::endl;
}

void OnReq(evhttp_request *req, void *) {
    using namespace std;

    auto *OutBuf = evhttp_request_get_output_buffer(req);

    if (!OutBuf) {
        return;
    }

    ifstream ifs("static/index.html");
    string content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());

    evbuffer_add_printf(OutBuf, "%s", content.c_str());
    evhttp_send_reply(req, HTTP_OK, "", OutBuf);

    LogRequest(req);
};

int main() {
    if (!event_init()) {
        std::cerr << "Failed to init libevent." << std::endl;
        return -1;
    }

    auto config = load_config();

    std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(config.address, config.port), &evhttp_free);

    if (Server) {
        std::cout << "Server listening " << config.address << ":" << config.port << std::endl;
    } else {
        std::cerr << "Failed to init http server." << std::endl;
        return -1;
    }

    evhttp_set_gencb(Server.get(), OnReq, nullptr);

    if (event_dispatch() == -1) {
        std::cerr << "Failed to run messahe loop." << std::endl;
        return -1;
    }

    return 0;
}
