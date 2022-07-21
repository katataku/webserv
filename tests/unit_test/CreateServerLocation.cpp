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

TEST_F(CreateServerLocationTest, LocationContextInServerContext) {
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
