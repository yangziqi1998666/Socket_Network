#include <cstdlib>
#include <netinet/in.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "network.h"

using namespace std;

class ServerP: public UDPObject{
public:

    // Init UDP Socket
    // This section will start when the class Server is created
    ServerP(const string &ip, int port): UDPObject(ip, port){
        centralUDPAddress = ipFactory(LOCALHOST_IP, CENTRAL_UDP_PORT);
    }

    // Receive the data from Central server
    vector<int> recvFromCentral(){
        string num = receiveContent(centralUDPAddress);
        int n = stoi(num);
        vector<int> scores(n);
        for(int i=0;i<n;i++){
            string score = receiveContent(centralUDPAddress);
            cout << "Received " << score << " from central" << endl;
            scores[i] = stoi(score);
        }
        return scores;
    }

    // Calculate the scores to figure out the matching gap between users
    static double process(vector<int> scores){
        double result;
        for ( int i =0; i< scores.size()-1; i++){
            double gap = (double)(abs(scores[i]-scores[i+1]))/(double)(scores[i]+scores[i+1]);
            result = result + gap;
        }
        return result;
    }

    // Send the results to Central server
    void sendToCentral(double result){
        sendContent(to_string(result), centralUDPAddress);
    }

private:
    struct sockaddr_in *centralUDPAddress;
};

int main(){
    ServerP serverP(LOCALHOST_IP, P_UDP_PORT);
    cout << "The ServerP is up and running using UDP on port " << P_UDP_PORT << "." << endl;


    while(true){
        vector<int> scores = serverP.recvFromCentral();
        cout << "The ServerP received the topology and score information." <<endl;
        double result = serverP.process(scores);
        serverP.sendToCentral(result);
        cout << "The ServerP finished sending the results to the Central." << endl;
    }
}