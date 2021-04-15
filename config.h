#ifndef CPP_WEB_SERVER_CONFIG_H
#define CPP_WEB_SERVER_CONFIG_H

#include <string>
#include <stdint.h>

struct ServerConfig {
    const char *address;
    std::uint16_t port;
};

ServerConfig load_config();

#endif //CPP_WEB_SERVER_CONFIG_H
