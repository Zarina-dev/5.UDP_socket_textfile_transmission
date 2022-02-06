#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>

class UDP_SOCK
{
public:
	int wsaSockInit();
	SOCKET sockCreate();
	int sockBind();

public:
	bool WSACleanup_status_cleaned ;

	void displaySockAddInfo( sockaddr_in* addr);


private:
	SOCKET SockPriv;
	sockaddr_in sockPrivAddr;
};



int UDP_SOCK::wsaSockInit()
{
	WSADATA wsaData;
	int iRetWsa;
	iRetWsa = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRetWsa != 0)
	{
		std::cout << "[ERROR] wsa 초기화 부분 오류 " << std::endl;
		WSACleanup();
		return 0;
	}
	else
	{
		std::cout << "[INFO] wsa 초기화 완료 " << std::endl;
		WSACleanup_status_cleaned = false;
		return 1;
	}
}


SOCKET UDP_SOCK::sockCreate()
{
	
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cout << "[ERROR] socket 선언 부분 오류 " << std::endl;
		WSACleanup();
		WSACleanup_status_cleaned = true;
		return 0;
	}
	else
	{
		std::cout << "[INFO] socket 선언 완료 " << std::endl;
		SockPriv = sock;
		return sock;
	}
		
}

void UDP_SOCK::displaySockAddInfo( sockaddr_in* addr)
{
	char addrIP[256];
	inet_ntop(AF_INET, &(addr->sin_addr), addrIP, 256);

	std::cout << "\n --------------------------------------- SOCKET ADDRESS INFO --------------- " << std::endl;
	std::cout << "                                     AF_Family : " << addr->sin_family << std::endl;
	std::cout << "                                   socket   IP : " << addrIP << std::endl;
	std::cout << "socket PORT(randomly give by listening server) : " << addr->sin_port << std::endl;
	std::cout << "   ----------------------------------------------------------------------------- \n" << std::endl;

	sockPrivAddr = *addr;
}

int UDP_SOCK::sockBind() {
	int bindRet;
	bindRet = bind(SockPriv, (sockaddr*) & sockPrivAddr, sizeof(sockPrivAddr));
	if (bindRet == SOCKET_ERROR)
	{
		std::cout << "[ERROR] socket bind 부분 오류 " << std::endl;
		WSACleanup();
		WSACleanup_status_cleaned = true;
		return -1;
	}
	else {
		std::cout << "[INFO] socket bind 완료 " << std::endl;
		return bindRet; // ==0
	}
}