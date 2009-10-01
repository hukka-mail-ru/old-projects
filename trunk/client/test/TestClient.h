#ifndef TestClient_H_
#define TestClient_H_


#include <cppunit/extensions/HelperMacros.h>

#include "Client.h"

#define RIGHT_SERVER_HOSTNAME   "wapserver3.wapportal.ru"
#define RIGHT_SERVER_PORT       1234

#define RIGHT_PROXY_HOSTNAME    "proxy.t-systems.ru"
#define RIGHT_PROXY_PORT        3128
#define RIGHT_PROXY_TYPE        QNetworkProxy::HttpProxy // supports only outgoing TCP connections

#define RIGTH_USER_NAME         "test"
#define RIGTH_USER_PASSWD       "test"

#define SHOW_FUNCTION_NAME      qDebug() << "================" << __FUNCTION__ << "================";

class TestClient: public CppUnit::TestFixture 
{
   CPPUNIT_TEST_SUITE(TestClient);
   CPPUNIT_TEST(testGetCRC);
   CPPUNIT_TEST(testConnectOK);
 //  CPPUNIT_TEST(testConnectWrongProxy);
   CPPUNIT_TEST(testConnectWrongAddress);
   CPPUNIT_TEST(testConnectWrongPort);

   CPPUNIT_TEST(testLoginOK);
/*
   CPPUNIT_TEST(testLoginWrongUser);
   CPPUNIT_TEST(testLoginWrongPasswd);
   CPPUNIT_TEST(testLoginAlreadyOnLine);
   CPPUNIT_TEST(testLoginAlreadyDisconnected);
*/
   CPPUNIT_TEST_SUITE_END();

    Client client;
    QNetworkProxy proxy;

         
public:   
    ~TestClient() {}
              
    void setUp() 
    {
        proxy=QNetworkProxy(RIGHT_PROXY_TYPE, RIGHT_PROXY_HOSTNAME, RIGHT_PROXY_PORT);
    }

    void tearDown() {}

    void testGetCRC()
    {
        SHOW_FUNCTION_NAME;

        QByteArray arr;
        arr.push_back(2);
        arr.push_back(1);
        arr.push_back((char)0);
        arr.push_back(127);
        arr.push_back(2);

        CPPUNIT_ASSERT_EQUAL(126, (int)client.getCRC(arr));
    }
    
    void testConnectOK()
    {
        SHOW_FUNCTION_NAME;
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT);
        CPPUNIT_ASSERT_EQUAL(CLI_ONLINE, client.status());

        CPPUNIT_ASSERT_EQUAL(true, client.disconnectFromHost());
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
    }


    void testConnectWrongProxy()
    {
        SHOW_FUNCTION_NAME;
        proxy.setHostName("wrong");

        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT);
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
    }

    void testConnectWrongAddress()
    {
        SHOW_FUNCTION_NAME;
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        client.connectToHost(proxy, "wrong", RIGHT_SERVER_PORT);
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
    }

    void testConnectWrongPort()
    {
        SHOW_FUNCTION_NAME;
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, 0);
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
    }



    void testLoginOK()
    {
        SHOW_FUNCTION_NAME;
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT);
        CPPUNIT_ASSERT_EQUAL(CLI_ONLINE, client.status());

        CPPUNIT_ASSERT_EQUAL(NOERR, client.login(RIGTH_USER_NAME, RIGTH_USER_PASSWD));

        client.disconnectFromHost();
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
    }

   void testLoginWrongUser()
   {
        SHOW_FUNCTION_NAME;

        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT);
        CPPUNIT_ASSERT_EQUAL(CLI_ONLINE, client.status());

        CPPUNIT_ASSERT_EQUAL(ERRBADLOGIN, client.login("wrong", RIGTH_USER_PASSWD));

        client.disconnectFromHost();
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
   }

   void testLoginWrongPasswd()
   {
        SHOW_FUNCTION_NAME;

        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT);
        CPPUNIT_ASSERT_EQUAL(CLI_ONLINE, client.status());

        CPPUNIT_ASSERT_EQUAL(ERRBADPASS, client.login(RIGTH_USER_NAME, "wrong"));

        client.disconnectFromHost();
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
   }

   void testLoginAlreadyOnLine()
   {
        SHOW_FUNCTION_NAME;

        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        CPPUNIT_ASSERT_EQUAL(true, client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT));
        CPPUNIT_ASSERT_EQUAL(CLI_ONLINE, client.status());

        CPPUNIT_ASSERT_EQUAL(NOERR, client.login(RIGTH_USER_NAME, RIGTH_USER_PASSWD));

        CPPUNIT_ASSERT_EQUAL(ERRUSERONLINE, client.login(RIGTH_USER_NAME, RIGTH_USER_PASSWD));

        CPPUNIT_ASSERT_EQUAL(true, client.disconnectFromHost());
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
   }


   void testLoginAlreadyDisconnected()
   {
        SHOW_FUNCTION_NAME;

        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        CPPUNIT_ASSERT_EQUAL(true, client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT));
        CPPUNIT_ASSERT_EQUAL(CLI_ONLINE, client.status());

        CPPUNIT_ASSERT_EQUAL(true, client.disconnectFromHost());
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());

        CPPUNIT_ASSERT_EQUAL(ERRUNDEF, client.login(RIGTH_USER_NAME, RIGTH_USER_PASSWD));
   }
};


CPPUNIT_TEST_SUITE_REGISTRATION(TestClient); 
   
#endif /*TestClient_H_*/