//
// Created by Ziqi Yang on 11/21/21.
//

#ifndef INC_2021SOCKET_NETWORK_H
#define INC_2021SOCKET_NETWORK_H

#include <string>
#include <netinet/in.h>

// IP and Ports
#define LOCALHOST_IP "127.0.0.1"

#define T_UDP_PORT 21948
#define S_UDP_PORT 22948
#define P_UDP_PORT 23948
#define CENTRAL_UDP_PORT 24948
#define CENTRAL_TCP_A_PORT 25948 //
#define CENTRAL_TCP_B_PORT 26948 //

typedef bool TCPRole;
#define TCP_SERVER 1
#define TCP_CLIENT 0

#define BACKLOG 20
#define BUFLEN 100

// Exceptions
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



using namespace std;

class TCPObject{
public:
    TCPObject(TCPRole role, const string& ip, int port);
    void sendContent(const string &content);
    string receiveContent();
    string receiveContentClient() const;
    void sendContentServer(const string &content) const;
    void closeSocket();

private:
    void initServer(string ip, int port);
    void connectServer(string ip, int port);

    int socketFd;
    int newSocketFd;
    TCPRole role;
};

class UDPObject{
public:
    UDPObject(const string &ip, int port);
    void sendContent(const string &content, struct sockaddr_in *dest);
    string receiveContent(struct sockaddr_in *dest);

    void closeSocket();

    int socketFd;
};


struct sockaddr_in * ipFactory(const string& ip, int port);


#endif //INC_2021SOCKET_NETWORK_H
