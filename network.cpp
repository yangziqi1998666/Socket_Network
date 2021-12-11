//
// Created by Ziqi Yang on 11/21/21.
//

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "network.h"


TCPObject::TCPObject(TCPRole role, const string &ip, int port) {
    this->role = role;
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw SOCKET_CREATE_EXCEPTION;
    }
    if (role == TCP_CLIENT) {
        connectServer(ip, port);
    } else {
        initServer(ip, port);
    }
}

void TCPObject::closeSocket() {
    close(socketFd);
}

void TCPObject::initServer(string ip, int port) {
    struct sockaddr_in *addr = ipFactory(ip, port);

    if (::bind(socketFd, (struct sockaddr *) addr, sizeof(*addr)) == -1) {
        throw TCP_BIND_EXCEPTION;
    }

    if ((listen(socketFd, BACKLOG) == -1)) {
        throw TCP_LISTEN_EXCEPTION;
    }

}

void TCPObject::connectServer(string ip, int port) {
    struct sockaddr_in *addr = ipFactory(ip, port);

    if (connect(socketFd, (struct sockaddr *) addr, sizeof(*addr)) == -1) {
        throw TCP_CONNECT_EXCEPTION;
    }
}

void TCPObject::sendContent(const string &content) {
    if (role == TCP_SERVER) {
        throw BAD_ROLE_EXCEPTION;
    }
    if (send(socketFd, content.c_str(), content.length(), 0) == -1) {
        throw TCP_SEND_EXCEPTION;
    }
}

void TCPObject::sendContentServer(const string &content) const{
    if(role == TCP_CLIENT){
        throw BAD_ROLE_EXCEPTION;
    }
    if(send(newSocketFd, content.c_str(), content.length(),0)<0){
        throw TCP_SEND_EXCEPTION;
    }
}

string TCPObject::receiveContent(){
    if (role != TCP_SERVER) {
        throw BAD_ROLE_EXCEPTION;
    }
    sockaddr_in clientAddr{};

    int recvLen = 0;
    socklen_t connections = sizeof(clientAddr);
    char temp[BUFLEN];
    if ((newSocketFd = accept(socketFd, (sockaddr *) &clientAddr, &connections)) == -1) {
        throw TCP_ACCEPT_EXCEPTION;
    }
    if ((recvLen = (int) recv(newSocketFd, temp, BUFLEN, 0)) < 0) {
        throw TCP_RECV_EXCEPTION;
    }
    temp[recvLen] = '\0';
    string res(temp);
    return res;
}

string TCPObject::receiveContentClient() const{
    if(role != TCP_CLIENT){
        throw BAD_ROLE_EXCEPTION;
    }
    int recvLen = 0;
    char temp[BUFLEN];
    if ((recvLen = (int) recv(socketFd, temp, BUFLEN, 0)) < 0) {
        throw TCP_RECV_EXCEPTION;
    }
    temp[recvLen] = '\0';
    string res(temp);
    return res;
}


UDPObject::UDPObject(const string &ip, int port) {
    if ((socketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        throw SOCKET_CREATE_EXCEPTION;
    }
    sockaddr_in *addr = ipFactory(ip, port);
    if (::bind(socketFd, (sockaddr *) addr, sizeof(*addr)) == -1) {
        throw UDP_BIND_EXCEPTION;
    }
}

void UDPObject::sendContent(const string &content, struct sockaddr_in *dest) {

    if (int sendLen = sendto(socketFd, content.c_str(), content.length(), 0, (struct sockaddr *) dest, sizeof(*dest)) < 0) {
        cerr << "Send Len: "<<sendLen<<endl;
        throw UDP_SEND_EXCEPTION;
    }
}

string UDPObject::receiveContent(struct sockaddr_in *dest) {
    char *temp = new char[BUFLEN]();
    ssize_t recvLen;
    socklen_t connections = sizeof(*dest);
    if ((recvLen = recvfrom(socketFd, temp, BUFLEN, 0, (struct sockaddr *) dest, &connections)) < 0) {
        throw UDP_RECEIVE_EXCEPTION;
    }
    string res(temp);
    delete[]temp;
    return res;
}

void UDPObject::closeSocket() {
    close(socketFd);
}

struct sockaddr_in *ipFactory(const string &ip, int port) {
    auto *addr = new struct sockaddr_in();
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = inet_addr(ip.c_str());
    return addr;
}