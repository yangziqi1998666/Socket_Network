#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

#define FILENAME "scores.txt"

#include "network.h"

class ServerS : public UDPObject {
public:

    // Init UDP Socket
    // This section will start when the class Server is created
    ServerS(const string &filename, const string &ip, int port) : UDPObject(ip, port) {
        loadDataFromFile(filename);
        centralUDPAddress = ipFactory(LOCALHOST_IP, CENTRAL_UDP_PORT);
    }

    // Read the data from "scores.txt"
    void loadDataFromFile(const string &filename) {
        fstream newfile;
        string name, score;
        newfile.open(filename, ios::in); //open a file to perform read operation using file object
        while (newfile >> name >> score) {
            data[name] = score;
        }
        newfile.close(); //close the file object.

    }

    // Receive the shortest path given from central server
    vector<string> receiveData() {
        string num = receiveContent(centralUDPAddress);
        int n = stoi(num);
        vector<string> scores(n);
        for(int i=0;i<n;i++){
            string name = receiveContent(centralUDPAddress);
            cout << "Received " << name << " from central" << endl;
            scores[i] = data[name];
        }
        return scores;
    }

    // Send the score of each user in path to central server
    void sendData(const vector<string> &scores){
        auto n = scores.size();
        for(auto i=0;i<n;i++){
            sendContent(scores[i], centralUDPAddress);
            cout << "Sent score " << scores[i] << " to central" << endl;
        }
    }

private:
    unordered_map<string, string> data;
    struct sockaddr_in *centralUDPAddress;

};

int main() {
    ServerS serverS(FILENAME, LOCALHOST_IP, S_UDP_PORT);
    cout << "The ServerS is up and running using UDP on port " << S_UDP_PORT << "." << endl;

    while (true) {
        vector<string> scores = serverS.receiveData();
        cout << "The ServerS received a request from Central to get the scores." << endl;
        serverS.sendData(scores);
        cout << "The ServerS finished sending the scores to Central." << endl;
    }
}