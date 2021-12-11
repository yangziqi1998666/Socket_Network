#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "network.h"

using namespace std;

// Format and print the information according the result from central server
void formatPrintResult(string username,const string &result) {
    stringstream res(result);
    int queries;
    res >> queries;
    for (int i = 0; i < queries; i++) {
        string target;
        res >> target;
        int pathSize;
        res >> pathSize;

        if (pathSize == 1){
            cout << "Found no compatibility for " << username << " and " << target << endl;
            string name,score;
            res >> name;
            res >> score;
        } else {
            string name, score;
            cout << "Found compatibility for " << username << " and " << target << ":" <<endl;
            res >> name;
            cout << name;
            for (int j = 1; j < pathSize; j++) {
                res >> name;
                cout << " --- " << name;
            }
            cout << endl;
            res >> score;
            cout << "Matching Gap : "<< score <<endl;
        }
    }
}

int main(int argc, const char *argv[]) {
    try{
        // Judge whether the input is correct
        if (argc != 2) {
            cout << "Please input in the following format:" << endl << "./clientA <username>" << endl;
            exit(EXIT_FAILURE);
        }

        // Init TCP connection
        string username(argv[1]);
        TCPObject central(TCP_CLIENT, LOCALHOST_IP, CENTRAL_TCP_A_PORT);
        cout << "The client is up and running." << endl;

        // send the input to central server
        central.sendContent(username);
        cout << "The client sent " << username << " to the Central server." << endl;

        // Receive the results from central server
        string result = central.receiveContentClient();
        formatPrintResult(username,result);
    }catch (int e){
        cout<<"ERROR in ClientA, Code: "<<e<<endl;
    }

}