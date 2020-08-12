#include "SocketServer.hpp"
#include "TcpComLink.hpp"
#include "Slim.hpp"

#include <stdio.h>


SocketServer::SocketServer() {
	slim_ = std::make_shared<Slim>();
}

SocketServer::~SocketServer() {
	Shutdown();
}

void SocketServer::serve(int socket) {
	int result = 0;
	auto comLink = std::make_shared<TcpComLink>(socket);

	slim_->HandleConnection(comLink);
}

void SocketServer::Run(const std::string& port_number) {
	StartWSA();
	CreateSocket();
	BindToSocket(std::stoi(port_number));
	WaitForConnection();	
	Shutdown();
}

void SocketServer::Shutdown() {
	shutdown(socket_,SD_SEND);
	closesocket(socket_);
	WSACleanup();
}

void SocketServer::WaitForConnection() {
	listen(socket_,1);

	SOCKET TempSock = SOCKET_ERROR;
	while(TempSock==SOCKET_ERROR) {
		TempSock = accept(socket_,nullptr,nullptr);
	}
	
	serve(TempSock);

	closesocket(TempSock);
}

void SocketServer::StartWSA() {
	WSADATA WsaDat;
	if(WSAStartup(MAKEWORD(2,2),&WsaDat) != 0) {
		printf("WSA Initialization failed!");
	}
}

void SocketServer::CreateSocket() {
	socket_ = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(socket_ == INVALID_SOCKET) {
		printf("Socket creationg failed.");
	}
}

void SocketServer::BindToSocket(int port) {
	SOCKADDR_IN serverInf;
	serverInf.sin_family=AF_INET;
	serverInf.sin_addr.s_addr=INADDR_ANY;
	serverInf.sin_port=htons(port);

	if(bind(socket_,(SOCKADDR*)(&serverInf),sizeof(serverInf)) == SOCKET_ERROR){
		printf("Unable to bind socket!");
	}
}
