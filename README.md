# EasyNetworking
I always wanted an easy and fast to setup network libary for my Games and outher projects but I could not find anything that fits my requirements, so I coded it myself. 

With this you can implement Socket Networking really fast and easy to your project.

These classes allow you to create a server that accepts as many clients as you wish. 

## Setup
Just Include the Headers and cpp files and you are good to go.

## Examples

### EasyClient
```c++
        Client client("127.0.0.1", 69);
        client.Connect();
        while (true)
        {
            std::cout << client.Reciev() << "\n";
            client.Send("Hello Server");
        }
```
### EasyServer (that accepts one client)
```c++
        Server server(69);
        std::cout << "Starting...\n";
        while (!server.Start()) {};
        std::cout << "Server Started!\n";
        while (!server.FindClient()) {};
        while (true)
        {
            server.Send(server.Clients[0], "Hello Client");
            std::cout << server.Reciev(server.Clients[0]) << "\n";
        }   
```
### EasyServer (that accepts many clients)
```c++
void find_new_client(Server &server)
{
    while (true)
    {
        server.FindClient();
    }
}

void SendToAll(Server &server, std::string msg)
{
    for (size_t i = 0; i < server.Clients.size(); i++)
    {
        server.Send(server.Clients[i], msg);
    }
}

int main()
{
    Server server(69);

    std::cout << "Starting...\n";
    while (!server.Start()) {};
    std::cout << "Server Started!\n";

    std::thread thr(find_new_client, std::ref(server));
    thr.detach();

    while (true)
    {
        for (size_t i = 0; i < server.Clients.size(); i++)
        {
            std::string msg = server.Reciev(server.Clients[i]);
            if (msg.size() > 0)
            {
                if (msg.size() > 0)
                {
                    std::cout << "Client: " << server.Clients[i] << ", " << msg << "\n";
                    SendToAll(server, msg);
                }
            }
        }
    }
}
```
