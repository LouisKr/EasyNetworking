#include "Server.h"

Server::Server(int Port)
    :mPort(Port)
{}

bool Server::Start()
{
    //Server
    // Initialze winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
        return 0;
    }

    // Create a socket
    mListening = socket(AF_INET, SOCK_STREAM, 0);
    if (mListening == INVALID_SOCKET)
    {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        return 0;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(mPort);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

    bind(mListening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening 
    listen(mListening, SOMAXCONN);
    return 1;
}

bool Server::FindClient()
{
    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    Clients.push_back(accept(mListening, (sockaddr*)&client, &clientSize));

    char host[NI_MAXHOST];		// Client's remote name
    char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

    ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        std::cout << host << " connected on port " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " <<
            ntohs(client.sin_port) << std::endl;
    }
    return 1;
}

std::string Server::Reciev(SOCKET Client)
{
    memset(&mBuf, 0, 255);

    // Wait for client to send data;
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set); /* clear the set */
    FD_SET(Client, &set); /* add our file descriptor to the set */
    timeout.tv_sec = 0.1;
    timeout.tv_usec = 0;
    int rv = select(Client, &set, NULL, NULL, &timeout);

    if (rv > 0)
    {
        int bytesReceived = recv(Client, mBuf, 255, 0);

        if (bytesReceived == 0 or bytesReceived == SOCKET_ERROR)
            return "";

        return mBuf;
    }
    return "";
}

bool Server::Send(SOCKET Client, std::string msg)
{
    send(Client, msg.c_str(), msg.size() + 1, 0);
    return 1;
}

bool Server::Close()
{
    for (size_t i = 0; i < Clients.size(); i++)
    {
        closesocket(Clients[i]);
    }
    closesocket(mListening);
    WSACleanup();
    return 1;
}
