#include <gtest/gtest.h>
#include <unistd.h>

#include "DefaultValues.hpp"
#include "LocationContext.hpp"
#include "ServerContext.hpp"
#include "ServerLocation.hpp"
#include "ServerLocationKey.hpp"
#include "WebservConfig.hpp"

class CreateServerLocationTest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(CreateServerLocationTest, single_ServerLocation) {
    /* 想定したconfigファイル
        server {
            listen 8080;
        }
    */
    WebservConfig conf;
    ServerContext serv_contx;

    serv_contx.set_port(8080);
    conf.PushServerContext(serv_contx);

    std::map<ServerLocationKey, ServerLocation> sl_container =
        conf.CreateServerLocations();

    ASSERT_EQ(sl_container.size(), 1);

    std::map<ServerLocationKey, ServerLocation>::iterator itr =
        sl_container.begin();

    ASSERT_EQ(itr->second.port(), 8080);
}

TEST_F(CreateServerLocationTest, two_ServerLocations) {
    /* 想定したconfigファイル
        server {
            listen 8080;
            location / {
                alias /var/www/html;
            }
        }
    */
    WebservConfig conf;
    ServerContext serv_contx;
    LocationContext locate_contx;

    locate_contx.set_path("/");
    locate_contx.set_alias("/var/www/html");

    serv_contx.set_port(8080);
    serv_contx.PushLocationContext(locate_contx);

    conf.PushServerContext(serv_contx);

    std::map<ServerLocationKey, ServerLocation> sl_container =
        conf.CreateServerLocations();

    ASSERT_EQ(sl_container.size(), 2);

    // serverコンテキストのServerLocationをチェック
    ServerLocationKey serv_key("8080", "", "");
    ServerLocation serv_sl = sl_container[serv_key];

    ASSERT_EQ(serv_sl.port(), 8080);

    // locationコンテキストのServerLocationをチェック
    ServerLocationKey locate_key("8080", "", "/");
    ServerLocation locate_sl = sl_container[locate_key];

    ASSERT_EQ(locate_sl.port(), 8080);
    ASSERT_EQ(locate_sl.path(), "/");
    ASSERT_EQ(locate_sl.alias(), "/var/www/html");
}

TEST_F(CreateServerLocationTest, two_server_contexts) {
    /* 想定したconfigファイル
        server {
            listen 8080;
        }
        server {
            listen 8081;
        }
    */
    WebservConfig conf;
    ServerContext serv_contx1;
    ServerContext serv_contx2;

    serv_contx1.set_port(8080);
    serv_contx2.set_port(8081);

    conf.PushServerContext(serv_contx1);
    conf.PushServerContext(serv_contx2);

    std::map<ServerLocationKey, ServerLocation> sl_container =
        conf.CreateServerLocations();

    ASSERT_EQ(sl_container.size(), 2);

    // serverコンテキストのServerLocationをチェック
    ServerLocationKey serv_key("8080", "", "");
    ServerLocation serv_sl = sl_container[serv_key];

    ASSERT_EQ(serv_sl.port(), 8080);

    // locationコンテキストのServerLocationをチェック
    ServerLocationKey locate_key("8081", "", "");
    ServerLocation locate_sl = sl_container[locate_key];

    ASSERT_EQ(locate_sl.port(), 8081);
}

TEST_F(CreateServerLocationTest, two_location_contexts) {
    /* 想定したconfigファイル
        server {
            listen 8080;
            location / {
                alias /var/www/html;
            }
            location /hoge {
                alias /var/www/hoge;
            }
        }
    */
    WebservConfig conf;
    ServerContext serv_contx;
    LocationContext locate_contx1;
    LocationContext locate_contx2;

    locate_contx1.set_path("/");
    locate_contx1.set_alias("/var/www/html");
    locate_contx2.set_path("/hoge");
    locate_contx2.set_alias("/var/www/hoge");

    serv_contx.set_port(8080);
    serv_contx.PushLocationContext(locate_contx1);
    serv_contx.PushLocationContext(locate_contx2);

    conf.PushServerContext(serv_contx);

    std::map<ServerLocationKey, ServerLocation> sl_container =
        conf.CreateServerLocations();

    ASSERT_EQ(sl_container.size(), 3);

    // serverコンテキストのServerLocationをチェック
    ServerLocationKey serv_key("8080", "", "");
    ServerLocation serv_sl = sl_container[serv_key];

    ASSERT_EQ(serv_sl.port(), 8080);

    // locationコンテキストのServerLocationをチェック
    ServerLocationKey locate_key1("8080", "", "/");
    ServerLocation locate_sl1 = sl_container[locate_key1];

    ASSERT_EQ(locate_sl1.port(), 8080);
    ASSERT_EQ(locate_sl1.path(), "/");
    ASSERT_EQ(locate_sl1.alias(), "/var/www/html");

    ServerLocationKey locate_key2("8080", "", "/hoge");
    ServerLocation locate_sl2 = sl_container[locate_key2];

    ASSERT_EQ(locate_sl2.port(), 8080);
    ASSERT_EQ(locate_sl2.path(), "/hoge");
    ASSERT_EQ(locate_sl2.alias(), "/var/www/hoge");
}

/*
    autoindexをboolで持っているため、autoindex
   offが設定されているか、設定されていないかが判断できない。だから浅いコンテキストの値で上書きされてしまう
*/
// TEST_F(CreateServerLocationTest, complex_test) {
//     /* 想定したconfigファイル
//         autoindex on;
//         server {
//             listen 8080;
//             location / {
//                 alias /var/www/html;
//             }
//             location /hoge {
//                 alias /var/www/hoge;
//             }
//         }
//         server {
//             listen 81;
//             autoindex off;
//         }
//     */
//     WebservConfig conf;
//     ServerContext serv_contx1;
//     ServerContext serv_contx2;
//     LocationContext locate_contx1;
//     LocationContext locate_contx2;

//     locate_contx1.set_path("/");
//     locate_contx1.set_alias("/var/www/html");
//     locate_contx2.set_path("/hoge");
//     locate_contx2.set_alias("/var/www/hoge");

//     serv_contx1.set_port(8080);
//     serv_contx1.PushLocationContext(locate_contx1);
//     serv_contx1.PushLocationContext(locate_contx2);
//     serv_contx2.set_port(81);
//     serv_contx2.set_auto_index(false);

//     conf.PushServerContext(serv_contx1);
//     conf.PushServerContext(serv_contx2);
//     conf.set_auto_index(true);

//     std::map<ServerLocationKey, ServerLocation> sl_container =
//         conf.CreateServerLocations();

//     ASSERT_EQ(sl_container.size(), 4);

//     // serverコンテキストのServerLocationをチェック
//     ServerLocationKey serv_key1("8080", "", "");
//     ServerLocation serv_sl1 = sl_container[serv_key1];

//     ASSERT_EQ(serv_sl1.port(), 8080);
//     ASSERT_EQ(serv_sl1.auto_index(), true);

//     ServerLocationKey serv_key2("81", "", "");
//     ServerLocation serv_sl2 = sl_container[serv_key2];

//     ASSERT_EQ(serv_sl2.port(), 81);
//     ASSERT_EQ(serv_sl2.auto_index(), false);

//     // locationコンテキストのServerLocationをチェック
//     ServerLocationKey locate_key1("8080", "", "/");
//     ServerLocation locate_sl1 = sl_container[locate_key1];

//     ASSERT_EQ(locate_sl1.port(), 8080);
//     ASSERT_EQ(locate_sl1.auto_index(), true);
//     ASSERT_EQ(locate_sl1.path(), "/");
//     ASSERT_EQ(locate_sl1.alias(), "/var/www/html");

//     ServerLocationKey locate_key2("8080", "", "/hoge");
//     ServerLocation locate_sl2 = sl_container[locate_key2];

//     ASSERT_EQ(locate_sl2.port(), 8080);
//     ASSERT_EQ(locate_sl2.auto_index(), true);
//     ASSERT_EQ(locate_sl2.path(), "/hoge");
//     ASSERT_EQ(locate_sl2.alias(), "/var/www/hoge");
// }
