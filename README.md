# EasyNetworking
I always wanted an easy and fast to setup network libary for my Games and outher projects but I could not find anything that fits my requirements, so I coded it myself. 

With this you can implement Socket Networking really fast and easy to your project.

These classes allow you to create a server that accepts as many clients as you wish. 

<br></br>
### EasyClient
```c++
        Client client("127.0.0.1", 190);
        client.Connect();
        while (true)
        {
            std::cout << client.Reciev() << "\n";
            client.Send("Hallo Server");
        }
```
<br></br>
### EasyServer
```c++
        Server server(190);
        std::cout << "Starting...\n";
        while (!server.Start()) {};
        std::cout << "Server Started!\n";
        while (!server.FindClient()) {};
        while (true)
        {
            server.Send(server.Clients[0], "Hallo Client");
            std::cout << server.Reciev(server.Clients[0]) << "\n";
        }   
```
