#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <climits>

#include "network.h"

using namespace std;

#define FILENAME "edgelist.txt"


class ServerT : public UDPObject {
public:
    // Init UDP Socket
    // This section will start when the class Server is created
    explicit ServerT(const string &filename, const string &ip, int port) : UDPObject(ip, port) {
        loadDataFromFile(filename);
        centralUDPAddress = ipFactory(LOCALHOST_IP, CENTRAL_UDP_PORT);
    }

    // read the data from "edgelist.txt"
    void loadDataFromFile(const string &filename) {
        fstream newfile;
        newfile.open(filename, ios::in); //open a file to perform read operation using file object
        if (newfile.is_open()) {
            string name1, name2;
            while (newfile >> name1 >> name2) {
                adj[name1].insert(name2);
                adj[name2].insert(name1);
            }
            newfile.close(); //close the file object.
        }
    }

   // receive the username from central server
    vector<string> recvFromCentral() {
        string usernameA = receiveContent(centralUDPAddress);
        string usernameB = receiveContent(centralUDPAddress);
        cout << "The ServerT received a request from Central to get the topology." << endl;
        return {usernameA, usernameB};
    }

    // Use the Dijkstra algorithm to find the shortest path between two users
    vector<string> searchPath(const string &from, const string &to) {
        unordered_map<string, string> records;
        unordered_set<string> founds;
        unordered_map<string, int> dists;
        std::for_each(adj.begin(), adj.end(), [&](auto it) { dists[it.first] = it.first == from ? 0 : INT_MAX; });

        while (!founds.count(to)) {
            string minName;
            int minVal = INT_MAX;

            for (auto &dist: dists) {
                if (founds.count(dist.first)) continue;
                if (dist.second < minVal) {
                    minVal = dist.second;
                    minName = dist.first;
                }
            }

            if (adj.count(minName)) {
                auto &neighbors = adj[minName];
                for (const string &neighbor: neighbors) {
                    if (founds.count(neighbor)) continue;
                    int dist = minVal + 1;
                    if (dist < dists[neighbor]) {
                        dists[neighbor] = dist;
                        records[neighbor] = minName;
                    }
                }
                founds.insert(minName);
            } else {
                break;
            }
        }

        vector<string> path;
        string p = to;
        while (!p.empty()) {
            path.push_back(p);
            p = records[p];
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    // Function which prints all the paths
    // from start to end
    // Used for testing
    static void printPaths(const vector<string> &path) {
        for (auto &i: path) {
            cout << i << " ";
        }
        cout << endl;
    }

    // Send the shortest path to central server
    void sendToCentral(const vector<string> &path) {
        auto n = path.size();
        sendContent(to_string(n), centralUDPAddress);
        for (auto i = 0; i < n; i++) {
            sendContent(path[i], centralUDPAddress);
        }
    }

private:
    unordered_map<string, set<string>> adj;
    sockaddr_in *centralUDPAddress;
};


int main() {

    try {
        ServerT serverT(FILENAME, LOCALHOST_IP, T_UDP_PORT);
        cout << "The ServerT is up and running using UDP on port " << T_UDP_PORT << "." << endl;
        serverT.loadDataFromFile(FILENAME);
        while (true) {
            vector<string> names = serverT.recvFromCentral();
            cout << "The ServerT received a request from Central to get the topology." << endl;
            vector<string> path = serverT.searchPath(names[0], names[1]);
            //ServerT::printPaths(path);
            serverT.sendToCentral(path);
            cout << "The ServerT finished sending the topology to Central. " << endl;
        }
    } catch (int e) {
        cerr << "ERROR in serverT, code: " << e << endl;
    }


}