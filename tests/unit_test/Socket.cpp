#include "Socket.hpp"

#include <gtest/gtest.h>

class SocketTest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

static int MakeClient(int port) {
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        return -1;
    }

    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr,
                             sizeof(serv_addr))) < 0) {
        return -1;
    }
    return sock;
}

static void SendFromClient(int fd, const std::string& msg) {
    send(fd, msg.c_str(), msg.size(), 0);
}

TEST_F(SocketTest, receive) {
    Socket* serv_sock = Socket::OpenListeningSocket("8081");

    pid_t pid = fork();
    if (pid == -1) {
        return;
    }
    if (pid == 0) {
        int fd = MakeClient(8081);
        SendFromClient(fd, "foo");
        close(fd);
        exit(0);
    }
    int st = 0;
    int sock_fd = serv_sock->Accept();

    char buf[1028];
    bzero(buf, 1028);
    read(sock_fd, buf, 1028);

    ASSERT_EQ(std::string(buf), "foo");

    serv_sock->Close();
    delete serv_sock;
}

class DISABLED_SocketTest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(DISABLED_SocketTest, send) {
    pid_t pid = fork();
    if (pid == -1) {
        return;
    }
    if (pid == 0) {
        Socket* serv_sock = Socket::OpenListeningSocket("8082");
        int sock_fd = serv_sock->Accept();
        Socket sock_to_client(sock_fd, false, "8082");
        sock_to_client.Send("bar");
        sock_to_client.Close();
        serv_sock->Close();
        delete serv_sock;
        exit(0);
    }
    int fd = MakeClient(8082);

    char buf[1028];
    bzero(buf, 1028);
    ssize_t byte = read(fd, buf, 1028);
    buf[byte] = '\0';

    ASSERT_EQ(std::string(buf), "bar");

    close(fd);
}
