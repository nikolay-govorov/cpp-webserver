#ifndef CPP_WEB_SERVER_CORE_H
#define CPP_WEB_SERVER_CORE_H

#include <string>
#include <evhttp.h>

std::string RenderTemplate(std::string const&  tmpl);

void LogRequest(evhttp_request *req);

#endif //CPP_WEB_SERVER_CORE_H
