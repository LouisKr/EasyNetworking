#include "Client.h"

Client::Client(std::string IP, int Port)
    :mIP(IP),
    mPort(Port)
{}

bool Client::Connect()
{
    // Initialize WinSock
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
        return 0;
    }

    // Create socket
    mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocket == INVALID_SOCKET)
    {
        std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }

    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(mPort);
    inet_pton(AF_INET, mIP.c_str(), &hint.sin_addr);

    // Connect to server
    int connResult = connect(mSocket, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
        closesocket(mSocket);
        WSACleanup();
        return 0;
    }
    return 1;
}

bool Client::Send(std::string msg)
{
    int sendResult = send(mSocket, msg.c_str(), msg.size() + 1, 0);
    if (sendResult == SOCKET_ERROR)
        return 0;
    return 1;
}

bool Client::Clean()
{
    closesocket(mSocket);
    WSACleanup();
    return 1;
}

std::string Client::Reciev()
{
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set); /* clear the set */
    FD_SET(mSocket, &set); /* add our file descriptor to the set */
    timeout.tv_sec = 0.1;
    timeout.tv_usec = 0;
    int rv = select(mSocket, &set, NULL, NULL, &timeout);

    if (rv > 0)
    {
        memset(&buf, 0, 255);
        int bytesReceived = recv(mSocket, buf, 255, 0);
        if (bytesReceived > 0)
        {
            return buf;
        }
    }
    return "";
}