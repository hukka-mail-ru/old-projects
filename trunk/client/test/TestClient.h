#ifndef TestClient_H_
#define TestClient_H_

#include <cppunit/extensions/HelperMacros.h>
#include "Client.h"
#include "Exception.h"

/*====================================================================================================
//  _____          _       ___ _ _            _   
// /__   \___  ___| |_    / __\ (_) ___ _ __ | |_ 
//   / /\/ _ \/ __| __|  / /  | | |/ _ \ '_ \| __|
//  / / |  __/\__ \ |_  / /___| | |  __/ | | | |_ 
//  \/   \___||___/\__| \____/|_|_|\___|_| |_|\__|
====================================================================================================*/

#define RIGHT_SERVER_HOSTNAME   "192.168.56.101"
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

        CPPUNIT_ASSERT_EQUAL(126, (int)getCRC(arr));
    }
    
    void testConnectOK()
    { 
        SHOW_FUNCTION_NAME;
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        CPPUNIT_ASSERT_NO_THROW(client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT));
        CPPUNIT_ASSERT_EQUAL(CLI_ONLINE, client.status());

        CPPUNIT_ASSERT_NO_THROW(client.disconnectFromHost());
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
    }


    void testConnectWrongProxy()
    {
        SHOW_FUNCTION_NAME;
        proxy.setHostName("wrong_server_name");

        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        CPPUNIT_ASSERT_THROW(client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT), Exception);
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
    }

    void testConnectWrongAddress()
    {
        SHOW_FUNCTION_NAME;
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        CPPUNIT_ASSERT_THROW(client.connectToHost(proxy, "wrong_server_name", RIGHT_SERVER_PORT), Exception);
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
    }

    void testConnectWrongPort()
    {
        SHOW_FUNCTION_NAME;
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        CPPUNIT_ASSERT_THROW(client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, 0), Exception);
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
    }



    void testLoginOK()
    {
        SHOW_FUNCTION_NAME;
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        CPPUNIT_ASSERT_NO_THROW(client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT));
        CPPUNIT_ASSERT_EQUAL(CLI_ONLINE, client.status());

        CPPUNIT_ASSERT_NO_THROW(client.login(RIGTH_USER_NAME, RIGTH_USER_PASSWD));

        CPPUNIT_ASSERT_NO_THROW(client.disconnectFromHost());
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
    }

   void testLoginWrongUser()
   {
        SHOW_FUNCTION_NAME;

        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        CPPUNIT_ASSERT_NO_THROW(client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT));
        CPPUNIT_ASSERT_EQUAL(CLI_ONLINE, client.status());

        CPPUNIT_ASSERT_THROW(client.login("wrong_server_name", RIGTH_USER_PASSWD), Exception);

        CPPUNIT_ASSERT_NO_THROW(client.disconnectFromHost());
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
   }

   void testLoginWrongPasswd()
   {
        SHOW_FUNCTION_NAME;

        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        CPPUNIT_ASSERT_NO_THROW(client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT));
        CPPUNIT_ASSERT_EQUAL(CLI_ONLINE, client.status());

        CPPUNIT_ASSERT_THROW(client.login(RIGTH_USER_NAME, "wrong_server_name"), Exception);

        CPPUNIT_ASSERT_NO_THROW(client.disconnectFromHost());
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
   }

   void testLoginAlreadyOnLine()
   {
        SHOW_FUNCTION_NAME;

        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        CPPUNIT_ASSERT_NO_THROW(client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT));
        CPPUNIT_ASSERT_EQUAL(CLI_ONLINE, client.status());

        CPPUNIT_ASSERT_NO_THROW(client.login(RIGTH_USER_NAME, RIGTH_USER_PASSWD));
        CPPUNIT_ASSERT_NO_THROW(client.login(RIGTH_USER_NAME, RIGTH_USER_PASSWD));

        CPPUNIT_ASSERT_NO_THROW(client.disconnectFromHost());
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
   }


   void testLoginAlreadyDisconnected()
   {
        SHOW_FUNCTION_NAME;

        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());
        CPPUNIT_ASSERT_NO_THROW(client.connectToHost(proxy, RIGHT_SERVER_HOSTNAME, RIGHT_SERVER_PORT));
        CPPUNIT_ASSERT_EQUAL(CLI_ONLINE, client.status());

        CPPUNIT_ASSERT_NO_THROW(client.disconnectFromHost());
        CPPUNIT_ASSERT_EQUAL(CLI_OFFLINE, client.status());

        CPPUNIT_ASSERT_THROW(client.login(RIGTH_USER_NAME, RIGTH_USER_PASSWD), Exception);
   }
};


CPPUNIT_TEST_SUITE_REGISTRATION(TestClient); 
   
#endif /*TestClient_H_*/
