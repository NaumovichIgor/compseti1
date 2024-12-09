#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

struct Request {
    char message[256];
};

struct Response {
    char message[256];
};

void writeRequest(const Request& request) {
    ofstream outFile("f1.bin", ios::binary | ios::app);
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char*>(&request), sizeof(Request));
        outFile.close();
    }
    else {
        cerr << "Unable to open f1.bin for writing." << endl;
    }
}

void readResponse() {
    Response response;
    ifstream inFile("f2.bin", ios::binary);
    while (true) {
        if (inFile.is_open()) {
            while (inFile.read(reinterpret_cast<char*>(&response), sizeof(Response))) {
                cout << "Response from server: " << response.message << endl;
            }
            inFile.close();
        }
        this_thread::sleep_for(chrono::seconds(1));
        inFile.open("f2.bin", ios::binary);
    }
}

int main() {
    thread responseThread(readResponse);
    Request request;

    while (true) {
        cout << "Enter your request (type 'exit' to quit): ";
        cin.getline(request.message, 256);
        if (string(request.message) == "exit") {
            break;
        }
        writeRequest(request);
    }

    responseThread.detach();
    return 0;
}
