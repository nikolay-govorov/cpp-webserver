#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <ctime>

#include <evhttp.h>

#include "core.h"

std::string RenderTemplate(std::string const&  tmpl) {
    std::string path = "static/" + tmpl + ".html";

    std::ifstream ifs(path);
    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    return content;
}

void LogRequest(evhttp_request *req) {
    auto uri = evhttp_request_get_uri(req);

    time_t now = time(nullptr);
    auto time = strtok(ctime(&now), "\n");

    std::cout << "[" << time << "]: " << uri << std::endl;
}
