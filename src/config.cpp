#include <cstdlib>

#include "config.h"

const char* getenv(const char* name, const char* def) {
    auto value = std::getenv(name);

    if (!value) {
        return def;
    }

    return value;
}

ServerConfig load_config() {
    using namespace std;

    uint16_t port = stoi(getenv("PORT", "5000"));
    const char* address = getenv("ADDRESS", "localhost");

    return { address, port };
}
