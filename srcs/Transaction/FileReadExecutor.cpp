#include "FileReadExecutor.hpp"

FileReadExecutor::FileReadExecutor() : logging_(Logging(__FUNCTION__)) {}

FileReadExecutor::FileReadExecutor(FileReadExecutor const &other) {
    *this = other;
}

FileReadExecutor &FileReadExecutor::operator=(FileReadExecutor const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

FileReadExecutor::~FileReadExecutor() {}

Response *FileReadExecutor::Exec(Request &request, ServerLocation &sl) {
    (void)request;
    (void)sl;
    return NULL;
}
