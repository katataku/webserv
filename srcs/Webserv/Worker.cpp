#include "Worker.hpp"

Worker::Worker() : logging_(Logging(__FUNCTION__)) {}

Worker::Worker(Worker const &other) { *this = other; }

Worker &Worker::operator=(Worker const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

Worker::~Worker() {}

void Worker::Exec(Socket &socket) {
    logging_.Debug("start exec");
    (void)socket;
}
