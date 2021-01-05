#pragma once
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

class Client
{
public:
    Client(std::string ServerIP, int ServerPort);

    bool Connect();
    bool Send(std::string msg);
    bool Clean();
    std::string Reciev();
private:
    std::string mIP;
    int mPort;

    char buf[255];

    SOCKET mSocket;
};
