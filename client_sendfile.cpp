/*  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*
*                   파일 전송 
*
*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS



// library include
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include "header.h"

#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS 

#define PORT 8080
#define BUF_SIZE 1024


void send_file_data(FILE* fp, SOCKET server_sockfd, sockaddr_in addr)
{
    int n;
    char buffer[BUF_SIZE];

    while (fgets(buffer, BUF_SIZE, fp) != NULL)
    {
        std::cout << "\n\n[SENDING] Data: " << buffer << std::endl;

        n = sendto(server_sockfd, buffer, BUF_SIZE, 0,(sockaddr*)&addr, sizeof(addr));

        if (n == SOCKET_ERROR) {
            std::cerr << "[ERROR] reading text file" << std::endl;
            return ;
        }
     
        ZeroMemory(buffer, BUF_SIZE);
    }

    strcpy(buffer, "END");
    sendto(server_sockfd, buffer, BUF_SIZE, 0, (sockaddr*)&addr, sizeof(addr));
    fclose(fp);
    return;
}


int main()
{
    std::cout << "---- UDP SERVER ----!\n";

    const char* ip = "127.0.0.1";



    // WSA 초기화
    UDP_SOCK udpSock;
    udpSock.wsaSockInit();
    std::cout << "WSACleanup_status_cleaned = " << udpSock.WSACleanup_status_cleaned << std::endl;;




    //  socket
    SOCKET server_sockfd;
    server_sockfd = udpSock.sockCreate();
  

    // socket address , client address
    sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);


    FILE* fp;
    const char* filename = "client.txt";
    
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        std::cerr << "[ERROR] reading text file" << std::endl;
        return 0;
    }

    send_file_data(fp, server_sockfd, server_addr);






    std::cout << "[SUCCESS] Data transfer complete" << std::endl;
    std::cout << "[CLOSING] Disconnecting socket" << std::endl;
    closesocket(server_sockfd);
    WSACleanup();

    std::cout << std::endl;
    system("PAUSE");

}



