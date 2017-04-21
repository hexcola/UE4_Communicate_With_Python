# Unreal Engine 4 Communcate with Python

> WIP & Sorry for my bad English. :)

Since UE4 can't have multiple sessions on one **GSI(Game Server Instance)**, we need to use multiple GSIs to meet players' requirements.

Then we face a problem, how we gonna manange these game server instances? 

The ideal way is make a master server in charge of :
* Spawn GSI.
* Communcate with GSI.
* Supervise on GSIs status, such as CPU, memory, bandwidth usage, how many players.
* Shutdown GSI.
* And etc.

But, how to build this master server? 

Use UE4 to create one? Maybe. How about use python. Assume we created a **Python Master Server** and all the GSIs are its **Node**s, and first step is we need to figure out how can we make UE4 communicate with Python?

There're some links here and there, some of them obsoleted, and some of them even totally doesn't work, that's why this simple echo example come out.

## Requirements

* Unreal Engine 4.14.3
* Python 2.7.13

## What's in the project?

* UE4 TCP Server <-> Python Simple Client
* UE4 TCP Client <-> Python Simple Server

## How to test it?

1. Clone or just download project zip file.
```bash
git clone https://github.com/hexcola/UE4_Communicate_With_Python.git
```

2. Go to `UE4_Network_Socket` folder, which is the root of UE4 project, find `UE4_Network_Socket.uproject` and right click choose `Generate Visual Studio project files`.

3. Open `UE4_Network_Socket.sln` solution, and build `UE4_Network_Socket`, after that, open this project with UE4 editor.
> You should only care about 5 files: `ActorTCPClient.h`, `ActorTCPClient.cpp`, `ActorTCPServer.h`, `ActorTCPServer.cpp` and the `UE4_Network_Socket.Build.cs` which add `Sockets` and `Networking` dependencies.

4. There're two maps, `TCP_Server`, `TCP_Client`, simple enought since both of them just have one actor.
    * If you treate UE4 instance as tcp server, then open `TCP_Server` map, and hit the Play button, then use `Python simple_tcp_client.py` to connect
    * If you treate UE4 instance as tcp client, then fisrt use `Python simple_tcp_server.py` to launch the tcp server, then open `TCP_Client` map, hit the play button to connect.


## Reference

* [TCP Socket Listener, Receive Binary Data From an IP/Port Into UE4, (Full Code Sample)](https://wiki.unrealengine.com/TCP_Socket_Listener,_Receive_Binary_Data_From_an_IP/Port_Into_UE4,_%28Full_Code_Sample)) - if can't open, try copy and open this url: https://wiki.unrealengine.com/TCP_Socket_Listener,_Receive_Binary_Data_From_an_IP/Port_Into_UE4,_%28Full_Code_Sample)
* [Communication Between UE4 and a Python UDP Server](https://forums.unrealengine.com/showthread.php?54343-Communication-Between-UE4-and-a-Python-UDP-Server)
* [Sending Data to Python From UE4 via TCP or UDP, partial success requesting help](https://answers.unrealengine.com/questions/71715/sending-data-to-python-from-ue4-via-tcp-or-udp-par.html)
