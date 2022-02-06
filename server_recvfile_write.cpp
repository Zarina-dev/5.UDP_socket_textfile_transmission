/*  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*
*                   파일 수신 -> 새로운 파일에 저장
* 
*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
// library include
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <stdio.h>
#include "header.h"

#pragma comment(lib, "ws2_32.lib")
#define PORT 8080
#define BUF_SIZE 1024

void write_file(SOCKET sockfd)
{
    sockaddr_in addr;
    int addr_size = sizeof(addr);
    FILE* fp;
    const char* filename = "server.txt";  // server.txt is new file
    char buffer[BUF_SIZE];
    int n;

    fp =  fopen(filename, "w");
    while (1)
    {
        n = recvfrom(sockfd, (char*)&buffer, BUF_SIZE, 0, (sockaddr*)&addr, &addr_size);

        if (strcmp(buffer, "END") == 0)
        {
            closesocket(sockfd);
            WSACleanup();
            break;
            return;
        }

        std::cout << "[RECEIVING ] Data: "  << buffer << std::endl;;

        fprintf(fp, "%s", buffer);
        ZeroMemory(buffer, BUF_SIZE);

    }


    std::cout << "[DATA SAVED]  DATA HAS WRITTEN IN server.txt FILE: " << buffer << std::endl;;

    fclose(fp);
    return;

}

int main()
{
    std::cout << "---- UDP SERVER ----!\n";

    const char* ip = "127.0.0.1";
   


    // WSA 초기화 - you can use normal mmethod
    UDP_SOCK udpSock;
    udpSock.wsaSockInit();
    std::cout << "WSACleanup_status_cleaned = " << udpSock.WSACleanup_status_cleaned << std::endl;;




    //  socket
    SOCKET server_sockfd;
    server_sockfd = udpSock.sockCreate();

  
    // socket address , client address
    sockaddr_in server_addr;
    char buffer[BUF_SIZE];



    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    


    // bind
    if (bind(server_sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR )
    {
        std::cout <<"[ERROR] binding error" << std::endl;
        WSACleanup();
    }

    std::cout << "[STARTING ] UDP file server started \n\n" << std::endl;

    write_file(server_sockfd);


    std::cout << "[SUCCESS] Data transfer complete" << std::endl;
    std::cout << "[CLOSING] Disconnecting socket" << std::endl;


    closesocket(server_sockfd);
    WSACleanup();

    std::cout << std::endl;
    system("PAUSE");

}
