#pragma once
#include <string>
#include <memory>
#include <winsock2.h>

class Slim;

	class SocketServer {
	public:
		SocketServer();
		~SocketServer();

		void Run(const std::string& port);

	protected:
		void StartWSA();
		void BindToSocket(int port);
		void Shutdown();
		void CreateSocket();
		void WaitForConnection();
		void serve(int socket);

	private:
		SOCKET socket_;
		std::shared_ptr<Slim> slim_;
	};
