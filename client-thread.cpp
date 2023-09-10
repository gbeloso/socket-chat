#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

using namespace std;

#define MAX 500
#define port 5200

bool connection_on;

void receiveMessages(int clientSocket) {
    char receiveMessage[MAX];
    while (true) {
        int rMsgSize = recv(clientSocket, receiveMessage, MAX, 0);

        if (rMsgSize < 0) {
            cout << "Packet not received" << endl;
            break;
        } else if (rMsgSize == 0) {
            cout << "Server off" << endl;
            break;
        }

        if (strcmp(receiveMessage, "bye") == 0) {
            cout << "Conn ended" << endl;
            break;
        } else {
            cout << "received server: " << receiveMessage << endl;
        }
    }

    connection_on = false;
}

int main() {
    int clientSocket, serverSocket, receiveMsgSize;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        cout << "Creation of client socket failed" << endl;
        return 0;
    }

    string ip_address;
    cout << "Server ip: ";
    cin >> ip_address;
    cin.ignore();
    cout << endl;

    struct sockaddr_in serverAddr, clientAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip_address.c_str());
    serverAddr.sin_port = htons(port);

    int conn = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (conn < 0) {
        cout << "Conn error" << endl;
        return 0;
    }

    thread recvThread(receiveMessages, clientSocket);

    connection_on = true;
    while (connection_on) {
        string s;
        char input[MAX];
        
        getline(cin, s);
        int n = s.size();
        int i = 0;

        while (i < n) {
            input[i] = s[i];
            i++;
        }
        input[n] = '\0';

        send(clientSocket, input, strlen(input) + 1, 0);
        
        if (strcmp(input, "bye") == 0) {
            cout << "Conn ended" << endl;
            break;
        }
    }

    recvThread.join();
    close(clientSocket);
    return 0;
}
