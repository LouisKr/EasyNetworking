#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

class Server
{
public:
    Server(int Port);
    
    bool Start();
    bool FindClient();

    std::string Reciev(SOCKET Client);
    bool Send(SOCKET Client, std::string msg);

    bool Close();

    std::vector<SOCKET> Clients;
private:
    int mPort;

    SOCKET mListening;
    char mBuf[255];
};
