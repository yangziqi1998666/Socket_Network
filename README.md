# Project Discription

## **1.FULL NAME:** 

## **2.Student ID:** 

## **3.What I have done**

Social networks are nowadays in every moment of our lives. The information built upon interactions among people has led to different types of applications. Crowdsourced apps such as Uber, Lyft, Waze use information for navigation purposes. Other apps such as dating apps provide matching algorithms to connect users who share similar behaviours and increase their compatibility chances for future success. In this project we shall implement a simplified version of a matching app that'll help us understand how matching systems work in the real world.

This project implement a model of a social matching service where two clients issue a request for finding their compatibility. The project contains 6 parts: two clients to issue a request for finding their compability, one central server and three backend servers. 

Besides the requirement, I also finish the optional part in pahse 4 : client B provides two usernames and the system compares both of their matching gaps with the client A username.

## **4.Code files and functions**

The project is built on Oriented Object Programming.

**network.h & network.cpp**: it functions as utils in this project. Since I find there are a lot of duplicated codes in servers and clients, I conclude two Objects in network.h: TCPObject and UDPObject which defines the abstract function structure. Also, I defines all the related ports number in network.h.

I conclude all possible exceptions into serial number, witch is useful to locate the bugs and figure them out.

```c++
#define SOCKET_CREATE_EXCEPTION -1
#define TCP_BIND_EXCEPTION -2
#define TCP_CONNECT_EXCEPTION -3
#define TCP_SEND_EXCEPTION -4
#define TCP_RECV_EXCEPTION -5
#define BAD_ROLE_EXCEPTION -6
#define TCP_LISTEN_EXCEPTION -7
#define TCP_ACCEPT_EXCEPTION -8
#define UDP_BIND_EXCEPTION -9
#define UDP_SEND_EXCEPTION -10
#define UDP_RECEIVE_EXCEPTION -11
```

Then, I finish the requiring functions in network.cpp: init, connect,  send, receive and ipFactory.

This file can improve the code reuse.

**clientA.cpp & clientB.cpp**: clients are roles of differents users. They want to find the compatibility between different users so they will send request to the central server.

**central.cpp**: It function as a central forwarding system. It will not do the complicated work. The main work of central server is to receive the data from clients and transimit required information between backend servers.

**serverT.cpp**: It will receive two usernames which clients input. Its goal is to figure out the shortest path in topology. I use the dijikstra algorithm in the server.

**serverS.cpp**: It will receive the path from central server, which is figured out by the serverT. Its goal is to seach for the scores map and transmit the scores of users in path to central server.

**serverP.cpp**: It will receive the scores from central server which searched by serverS. These scores will be transmitted in a certain order as the path. Therefore for central server there is no need to transmit path to serverP.

**After all,** in order to simplify the process, each server will send a number at first, which mean the times of messages it will be received.

## **5.The format of all the messages exchanged**

Backend-Server T

|                      Event                      |                      On Screen Messages                      |
| :---------------------------------------------: | :----------------------------------------------------------: |
|                   Booting Up                    | “The ServerT is up and running using UDP on port <port number>.” |
| Upon Receiving the request from Central server: | “The ServerT received a request from Central to get the topology.” |
| After sending the results to the Central server |   “The ServerT finished sending the topology to Central.”    |

Backend-Server S

|                      Event                      |                      On Screen Messages                      |
| :---------------------------------------------: | :----------------------------------------------------------: |
|                   Booting Up                    | “The ServerS is up and running using UDP on port <port number>.” |
| Upon Receiving the request from Central server: | “The ServerS received a request from Central to get the scores.” |
| After sending the results to the Central server |    “The ServerS finished sending the scores to Central.”     |

Backend-Server P

|                      Event                      |                      On Screen Messages                      |
| :---------------------------------------------: | :----------------------------------------------------------: |
|                   Booting Up                    | “The ServerP is up and running using UDP on port <port number>.” |
|        Upon Receiving the input string:         |  “The ServerP received the topology and score information.”  |
| After sending the results to the Central server |  “The ServerP finished sending the results to the Central.”  |

Client A

|                     Event                      |                      On Screen Messages                      |
| :--------------------------------------------: | :----------------------------------------------------------: |
|                   Booting Up                   |               “The client is up and running.”                |
|             Upon sending the input             |      "The client sent <INPUT1> to the Central server.”       |
| After receiving the result from Central server | “Found compatibility for <INPUT1> and <INPUT2>: <br /><INPUT1> --- <USERY> ---<USERX> --- <INPUT2> <br />Matching Gap : <VALUE>” |
|        If no compatibility between both        |      “Found no compatibility for <INPUT1> and <INPUT2>”      |

Client B

|                     Event                      |                      On Screen Messages                      |
| :--------------------------------------------: | :----------------------------------------------------------: |
|                   Booting Up                   |               “The client is up and running.”                |
|             Upon sending the input             |      "The client sent <INPUT2> to the Central server.”       |
| After receiving the result from Central server | “Found compatibility for <INPUT2> and <INPUT1>: <br /><INPUT2> --- <USERY> ---<USERX> --- <INPUT1> <br />Matching Gap : <VALUE>” |
|        If no compatibility between both        |      “Found no compatibility for <INPUT2> and <INPUT1>”      |

Central server

|                      Event                       |                      On Screen Messages                      |
| :----------------------------------------------: | :----------------------------------------------------------: |
|                    Booting Up                    |           “The Central server is up and running.”            |
|  Upon Receiving the username from the client A:  | “The Central server received input=<INPUT1> from the client using TCP over port <port number>.” |
|  Upon Receiving the username from the client B:  | “The Central server received input=<INPUT2> from the client using TCP over port <port number>.” |
|       After querying each Backend-Servers        | “The Central server sent a request to Backend-Server T”.<br/>“The Central server sent a request to Backend-Server S”<br /> “The Central server sent a processing request to Backend-Server P.” |
|  After receiving result from backend server i)   | “The Central server received information from Backend-Server <i> using UDP over port <port number>.” |
|   After receiving result from backend server P   | “The Central server received the results from backend server P.” |
| After sending the final result to the client j): |     “The Central server sent the results to client <j>.”     |

## **6.Idiosyncrasy**

I have try my best to consider diferent situations in this project. Also, I finish the optional part which clientB input two usernames. However, at first I deceide to fulfill a more complicated goal: **Both clientA and clientB can send as many usernames as they want.** I think it is not diffcult since I have conclude all the required function of TCP and UDP into a network.cpp file. However, the real situation is not easy as what I think. It will increase a lot of workload. I have another project in other course, so the time is limited. I wish I could finish it in future.

## **7.Rused Code**

All of the project is finished by myself. I reference some codes on the internet but I have modify them into my own coding style.