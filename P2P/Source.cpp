#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

//A man who thinks he can and a man who thinks he cannot are both right!!

int main() {

	int res = 0;
	
	WSADATA wsaData = { 0 };

	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != NO_ERROR) {

		std::cout << "WSAStartup Failed!! with error - "<<WSAGetLastError();
		return 0;
	}

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) {

		std::cout << "Socket Creation Failed with error - " << WSAGetLastError();
		return 0;
	}

	sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_port = htons(4596);
#pragma warning(suppress : 4996)
	address.sin_addr.s_addr = INADDR_ANY;

	res = bind(sock, (SOCKADDR *)&address, sizeof(address));
	if (res == SOCKET_ERROR) {

		std::cout << "Binding Failed with error - " << WSAGetLastError();
		closesocket(sock);
		WSACleanup();
		return 0;
	}

	char* target_ip = (char*)calloc(17, sizeof(char));
	short choice;

	std::cout << "Initialization Successfull\n";
	std::cout << "Enter the ip address of the target (IPv4): ";
	std::cin >> target_ip;

	sockaddr_in target_addr;
	target_addr.sin_family = AF_INET;
	target_addr.sin_port = htons(4596);
#pragma warning(suppress : 4996)
	target_addr.sin_addr.s_addr = inet_addr(target_ip);

	free(target_ip);

	char* sendBuffer = (char*)calloc(20, sizeof(char));
	sprintf_s(sendBuffer, 19, "Hello There!");

	std::cout << "Starting Hole Punching...";

	for (int i = 0; i < 5; i++) {

		res = sendto(sock, sendBuffer, strlen(sendBuffer), 0, (SOCKADDR*)&target_addr, sizeof(target_addr));
		if (res == SOCKET_ERROR) {

			std::cout << "WSAGetLastError - " << WSAGetLastError();
		}
	}

	free(sendBuffer);
	sendBuffer = (char*)calloc(20, sizeof(char));

	std::cout << "Done!\n\n";

	std::cout << "Send 0 or Receive 1?";
	std::cin >> choice;

	if (choice == 1) {

		int addrSize = sizeof(target_addr);
		res = recvfrom(sock, sendBuffer, 19, 0, (SOCKADDR*)&target_addr, &addrSize);
		if (res == SOCKET_ERROR) {

			std::cout << "WSAGetLastError - " << WSAGetLastError();
		}

		std::cout << "Received Data - " << sendBuffer;
	}
	else {

		sprintf_s(sendBuffer, 19, "Fuck Yeah!!!!");

		res = sendto(sock, sendBuffer, strlen(sendBuffer), 0, (SOCKADDR*)&target_addr, sizeof(target_addr));
		if (res == SOCKET_ERROR) {

			std::cout << "WSAGetLastError - " << WSAGetLastError();
		}
	}

	closesocket(sock);
	WSACleanup();

	return 0;

}