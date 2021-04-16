#include <cstdlib>

#include "config.h"

const char* getenv(const char* name, const char* def) {
    auto value = std::getenv(name);

    return value ? value : def;
}

ServerConfig load_config() {
    using namespace std;

    uint16_t port = stoi(getenv("PORT", "5000"));
    uint16_t threads = stoi(getenv("THREADS", "4"));
    const char* address = getenv("ADDRESS", "localhost");

    return { address, port, threads };
}
