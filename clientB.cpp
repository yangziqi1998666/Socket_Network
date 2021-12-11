#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include "network.h"

using namespace std;

void formatPrintResult(vector<string> username,const string &result) {
    stringstream res(result);
    int queries;
    string target;
    res >> queries;
    res >> target;
    for (int i = 0; i < queries; i++) {
        int pathSize;
        res >> pathSize;
        if (pathSize == 1){
            cout << "Found no compatibility for " << username[i] << " and " << target << endl;
            string name,score;
            res >> name;
            res >> score;
        } else {
            string name, score;
            cout << "Found compatibility for " << username[i] << " and " << target << ":" <<endl;
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
    try {
        // Judge whether the input is correct
        if (argc != 2 && argc != 3) {
            cout << "Please input in the following format:" << endl << "./clientB <username> [<username>]" << endl;
            exit(EXIT_FAILURE);
        }

        // Store the inputs and format them
        vector<string> username;
        username.push_back(string(argv[1]));
        string content(argv[1]);
        // Judge if the clientB inputs two usernames
        if (argc == 3) {
            username.push_back(string(argv[2]));
            content += " " + string(argv[2]);
        }

        // Init TCP connection
        TCPObject central(TCP_CLIENT, LOCALHOST_IP, CENTRAL_TCP_B_PORT);
        cout << "The client is up and running." << endl;

        // Send the input to central server
        central.sendContent(content);
        cout << "The client sent " << content << " to the Central server." << endl;

        // Receive the results from central server
        string result = central.receiveContentClient();
        formatPrintResult(username,result);
    } catch (int e) {
        cout << "ERROR in ClientB, Code: " << e << endl;
    }

}