#include <stdio.h>
#include <netinet/in.h>
#include <iostream>
#include <vector>
#include "network.h"
#include <algorithm>
#include <cmath>
#include <iterator>


using namespace std;

int main() {

    try {
        // initialize TCP and UDP connections
        TCPObject clientA(TCP_SERVER, LOCALHOST_IP, CENTRAL_TCP_A_PORT);
        TCPObject clientB(TCP_SERVER, LOCALHOST_IP, CENTRAL_TCP_B_PORT);
        UDPObject central(LOCALHOST_IP, CENTRAL_UDP_PORT);
        sockaddr_in *serverTUDPAddress = ipFactory(LOCALHOST_IP, T_UDP_PORT);
        sockaddr_in *serverSUDPAddress = ipFactory(LOCALHOST_IP, S_UDP_PORT);
        sockaddr_in *serverPUDPAddress = ipFactory(LOCALHOST_IP, P_UDP_PORT);

        // Boot up message
        cout << "The Central server is up and running." << endl;

        while (true) {
            // Receive the data from two clients
            string username1 = clientA.receiveContent();
            cout << "The Central server received input= " << username1 << " from the client using TCP over port "
                 << CENTRAL_TCP_A_PORT << endl;
            string content = clientB.receiveContent();
            cout << "the Central server received input= " << content << " from the client using TCP over port "
                 << CENTRAL_TCP_B_PORT << endl;

            // judge the clientB send two usernames or one
            // if two, divide them from string into vector
            vector<string> names;
            auto pos = content.find_first_of(' ');
            if (pos == string::npos) {
                names.push_back(content);
            } else {
                names.push_back(content.substr(0, pos));
                names.push_back(content.substr(pos + 1));
            }
            // nameSize is the amount of clientB's input
            int nameSize = names.size();
            // Used to store path
            vector<vector<string>> paths(nameSize);
            vector<string> results(nameSize);

            // Loop according to the amount of clientB's input
            // If clientB send two usernames, central will finish the forwarding process for twice
            for (int nameIndex = 0; nameIndex < nameSize; nameIndex++) {
                string username2 = names[nameIndex];

                // Send usernames to serverT
                // @username1: come from A
                // @username2: come from B
                central.sendContent(username1, serverTUDPAddress);
                central.sendContent(username2, serverTUDPAddress);
                cout << "The Central server sent a request to Backend-Server T." << endl;

                // receive the path from serverT
                // Return is the amount of paths
                string num = central.receiveContent(serverTUDPAddress);
                cout << "The Central server received information from Backend-Server T using UDP over port " << CENTRAL_UDP_PORT << endl;
                int n = stoi(num);
                vector<string> path;
                for (int i = 0; i < n; i++) {
                    string name = central.receiveContent(serverTUDPAddress);
                    path.push_back(name);
                }

                // Send the path to serverS
                paths[nameIndex] = path;
                vector<string> scores(n);
                central.sendContent(num, serverSUDPAddress);
                for (int i = 0; i < n; i++) {
                    central.sendContent(path[i], serverSUDPAddress);
                }
                cout << "The Central server sent a request to Backend-Server S." << endl;

                // Receive the scores from serverS
                for (int i = 0; i < n; i++) {
                    scores[i] = central.receiveContent(serverSUDPAddress);
                }
                cout << "The Central server received information from Backend-Server S using UDP over port " << CENTRAL_UDP_PORT << endl;

                // Send the scores to serverP
                central.sendContent(num, serverPUDPAddress);
                for (int i = 0; i < n; i++) {
                    central.sendContent(scores[i], serverPUDPAddress);
                }
                cout << "The Central server sent a processing request to Backend-Server P." << endl;

                // Receive the matching gap from serverP
                double res = stod(central.receiveContent(serverPUDPAddress));
                cout << "The Central server received the results from backend server P." << endl;

                // To keep the result in two decimals
                int rnd = (int) round(res * 100);
                double dbl = (double) rnd / 100.0;
                char temp[BUFLEN];
                sprintf(temp, "%.2f", dbl);
                string result(temp);

                results[nameIndex] = result;
            }

            // reformat the data of path,matching gap into string and transmit to clients.
            string contentA = to_string(nameSize), contentB = contentA;
            contentB += " " +username1;
            for (int i = 0; i < nameSize; i++) {
                vector<string> &path = paths[i];
                int pathSize = path.size();
                contentA += " " + names[i];
                contentA += " " + to_string(pathSize);
                contentB += " " + to_string(pathSize);
                for (int j=0;j<pathSize;j++) {
                    contentA += " " + path[j];
                    contentB += " " + path[pathSize-1-j];
                }
                contentA += " " + results[i];
                contentB += " " + results[i];
            }
            clientA.sendContentServer(contentA);
            cout << "The Central server sent the results to clientA" << endl;
            clientB.sendContentServer(contentB);
            cout << "The Central server sent the results to clientB" << endl;
        }

    } catch (int e) {
        cout << "Exception code: " << e;
    }


}


