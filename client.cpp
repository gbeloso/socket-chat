#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
using namespace std;
#define MAX 500
#define port 5200

int main(){
	
	int clientSocket, serverSocket, receiveMsgSize;
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0)
	{
		cout << "Creation of client socket failed" << endl;
		return 0;
	}

	struct sockaddr_in serverAddr, clientAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("192.168.15.11");
	serverAddr.sin_port = htons(port);
	int conn;
	conn = connect(clientSocket, (struct sockaddr *) & serverAddr, sizeof(serverAddr));
	if(conn < 0){
		cout << "Conn error" << endl;
		return 0;
	}
	
	while(true){
		string s;
		char input[MAX];
		cout << "Client: ";
		getline(cin, s);
		int n = s.size();
		int i = 0;
		while(i < n){
			input[i] = s[i];
			i++;
		}
		input[n] = '\0';
		send(clientSocket, input, strlen(input)+1, 0);
		char receiveMessage[MAX];
		int rMsgSize = recv(clientSocket, receiveMessage, MAX, 0);
		if (rMsgSize < 0){
			cout << "Packet not received" << endl;
			return 0;
		}
		else if(rMsgSize == 0){
			cout << "Server off" << endl;
			return 0;
		}

		if(strcmp(receiveMessage, "bye") == 0){
			cout << "Conn ended" << endl;
			break;
		}
		else{
			cout << "received server: " << receiveMessage << endl;
		}
	}
	close(clientSocket);
	return 0;
}
