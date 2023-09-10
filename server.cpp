#include<bits/stdc++.h>
using namespace std;
#include<sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "server.h"

#define MAX 500
#define PORT 5200

int main()
{
	
    int serverSocketHandler = createSocket();

    struct sockaddr_in serverAddr = createServerAddress(PORT);
    struct sockaddr_in clientAddr;

    int bindStatus = bind(serverSocketHandler, (struct sockaddr *) &serverAddr, sizeof(sockaddr));
    //"binda" o socket do servidor para aceitar conexões de qualquer endereço 

    if(bindStatus < 0){
        cout << "Socket binding failed" << endl;
        return 0;
    }

    int listenStatus = listen(serverSocketHandler, 2);
    //faz o socket escutar e coloca uma fila de até duas conexões 
    if(listenStatus < 0){
        cout << "Listen has failed" << endl;
        return 0;
    }
    cout << "Waiting conns" << endl;

    char buffer[MAX];
    socklen_t len = sizeof(clientAddr);
    int conn;
    conn = accept(serverSocketHandler, (struct sockaddr *) &clientAddr, &len);
    //extrai a primeira conexão esperando na fila e direciona para a variável conn

    if(conn > 0){
        cout << "Server accepted the request. \n" << endl;
    }
    else{
        cout << "Conn failed" << endl;
        return 0;
    }

    while (true){
        int rMsgSize;

        rMsgSize = recv(conn, buffer, MAX, 0);
        /*recebe mensagens no socket, unica diferença em relação ao read é a presença de flags, 
        como não estamos passando nenhuma poderia ser mantido o read sem problemas, 
        opatmos por usar ela por ser mais indicada para conexões na rede,*/
        if(rMsgSize > 0){
            cout << "received client: " << buffer << endl;
        }
        if(strcmp(buffer, "bye") == 0){
            cout << "Conn ended" << endl;
            send(conn, buffer, sizeof(buffer)+1, 0);
            break;
        }

        cout << "Server: ";
        char input[MAX];
        string s;
        getline(cin, s);
        int n = s.size();
        for (int i = 0; i < n; i++){
            input[i] = s[i];
        }

        input[n] = '\0';

        send(conn, input, strlen(input)+1, 0);

    }

    close(serverSocketHandler);
    return 0;

}

int createSocket(void){
    int serverSocketHandler = socket(AF_INET , SOCK_STREAM , 0); 
    // cria um fd para a comunicação pela rede

	if(serverSocketHandler < 0)
    {
        cout << "Socket creation has failed.";
        return 0;
    }
    else{
        return serverSocketHandler;
    }
}

struct sockaddr_in  createServerAddress(int port){
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    return serverAddr;
}