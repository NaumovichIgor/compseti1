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

void processRequest(const Request& request, Response& response) {
    snprintf(response.message, sizeof(response.message), "Processed: %s", request.message);
}

void checkRequests() {
    Request request;
    Response response;
    streampos lastSize = 0;

    while (true) {
        ifstream inFile("f1.bin", ios::binary | ios::ate);
        streampos currentSize = inFile.tellg();
        inFile.close();

        if (currentSize > lastSize) {
            inFile.open("f1.bin", ios::binary);
            inFile.seekg(lastSize);
            while (inFile.read(reinterpret_cast<char*>(&request), sizeof(Request))) {
                processRequest(request, response);
                ofstream outFile("f2.bin", ios::binary | ios::app);
                outFile.write(reinterpret_cast<const char*>(&response), sizeof(Response));
                outFile.close();
            }
            lastSize = currentSize;
            inFile.close();
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main() {
    checkRequests();
    return 0;
}
