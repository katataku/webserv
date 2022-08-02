#include "HTTPReasonPhrase.hpp"

HTTPReasonPhrase::HTTPReasonPhrase() {}

HTTPReasonPhrase::HTTPReasonPhrase(HTTPReasonPhrase const &other) {
    *this = other;
}

HTTPReasonPhrase &HTTPReasonPhrase::operator=(HTTPReasonPhrase const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

HTTPReasonPhrase::~HTTPReasonPhrase() {}

std::string HTTPReasonPhrase::GetReasonPhrase(int status_code) {
    switch (status_code) {
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 302:
            return "Moved Temporarily";
        case 400:
            return "Bad Request";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 413:
            return "Payload Too Large";
        case 414:
            return "URI Too Long";
        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        case 505:
            return "HTTP Version Not Supported";
        default:
            throw std::runtime_error("unsupported status code");
    }
}
