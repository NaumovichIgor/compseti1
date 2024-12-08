#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

void writeRequest(const string& request) {
    ofstream outFile("f1.txt", ios::app);
    if (outFile.is_open()) {
        outFile << request << endl;
        outFile.close();
    } else {
        cerr << "Unable to open f1.txt for writing." << endl;
    }
}

void readResponse() {
    ifstream inFile("f2.txt");
    string response;
    while (true) {
        if (inFile.is_open()) {
            while (getline(inFile, response)) {
                cout << "Response from server: " << response << endl;
            }
            inFile.close();
        }
        this_thread::sleep_for(chrono::seconds(1)); // Check every second
        inFile.open("f2.txt");
    }
}

int main() {
    thread responseThread(readResponse);
    string request;

    while (true) {
        cout << "Enter your request (type 'exit' to quit): ";
        getline(cin, request);
        if (request == "exit") {
            break;
        }
        writeRequest(request);
    }

    responseThread.detach();
    return 0;
}
