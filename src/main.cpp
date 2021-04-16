#include <memory>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <vector>

#include <evhttp.h>

#include "config.h"
#include "core.h"

void OnReq(evhttp_request *req, void *) {
    using namespace std;

    auto *OutBuf = evhttp_request_get_output_buffer(req);

    if (!OutBuf) {
        return;
    }

    auto tmpl = RenderTemplate("index");

    evbuffer_add_printf(OutBuf, "%s", tmpl.c_str());
    evhttp_send_reply(req, HTTP_OK, "", OutBuf);

    LogRequest(req);
};

int main() {
    auto config = load_config();

    try {
        std::exception_ptr InitExcept;

        bool volatile IsRun = true;
        evutil_socket_t Socket = -1;

        auto ThreadFunc = [&] {
            try {
                std::unique_ptr<event_base, decltype(&event_base_free)> EventBase(event_base_new(), &event_base_free);
                if (!EventBase)
                    throw std::runtime_error("Failed to create new base_event");

                std::unique_ptr<evhttp, decltype(&evhttp_free)> EvHttp(evhttp_new(EventBase.get()), &evhttp_free);
                if (!EvHttp)
                    throw std::runtime_error("Failed to create new evhttp.");

                evhttp_set_gencb(EvHttp.get(), OnReq, nullptr);

                if (Socket == -1) {
                    auto *BoundSock = evhttp_bind_socket_with_handle(EvHttp.get(), config.address, config.port);
                    if (!BoundSock)
                        throw std::runtime_error("Failed to bind server socket.");

                    if ((Socket = evhttp_bound_socket_get_fd(BoundSock)) == -1) {
                        throw std::runtime_error("Failed to get server socket for next instance.");
                    }
                } else if (evhttp_accept_socket(EvHttp.get(), Socket) == -1) {
                    throw std::runtime_error("Failed to bind server socket for new instance.");
                }

                while (IsRun) {
                    event_base_loop(EventBase.get(), EVLOOP_NONBLOCK);
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            } catch (...) {
                InitExcept = std::current_exception();
            }
        };

        auto ThreadDeleter = [&] (std::thread *t) {
            IsRun = false;
            t->join();
            delete t;
        };

        typedef std::unique_ptr<std::thread, decltype(ThreadDeleter)> ThreadPtr;
        std::vector<ThreadPtr> Threads;

        for (unsigned i = 0; i < config.threads; ++i) {
            ThreadPtr Thread(new std::thread(ThreadFunc), ThreadDeleter);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (InitExcept != std::exception_ptr()) {
                IsRun = false;
                std::rethrow_exception(InitExcept);
            }

            Threads.push_back(std::move(Thread));
        }

        std::cout << "Server listening " << config.address << ":" << config.port << std::endl;

        // The application can exit by SIGKILL
        while (IsRun)
            std::cin.get();
    } catch (std::exception &e) {
        std::cerr << "[Error]: " << e.what() << std::endl;
    }

    return 0;
}
