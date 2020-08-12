#include "TcpComLink.hpp"
#include <string>
#include <array>
#include <iostream>


inline int util_min(int no1, int no2) {
	return no1 < no2 ? no1:no2;
}


TcpComLink::TcpComLink(int socket) {
	socket_ = socket;
}

int TcpComLink::Send(const std::string& msg)
{
	int total = 0;        // how many bytes we've sent
	int bytesleft = msg.size(); // how many we have left to send
	int n;

	while (total < msg.size()) {
		n = send(socket_, msg.c_str() + total, bytesleft, 0);
		if (n == -1) { break; }
		total += n;
		bytesleft -= n;
	}

	std::cout << "Send: " << msg << std::endl;
	return total;
}

/*int TcpComLink::Send(const std::string& msg) {
	return msg.size();
}*/

int TcpComLink::Recv(std::string& buffer, int length)
{
	std::array<char, 512> buf;
	int bytes_received = 0;
	while (bytes_received < length) {
		const auto received = recv(socket_, buf.data(), util_min(length - bytes_received, 512), 0);
		bytes_received += received;
		buffer += std::string(buf.data(), received);
	}

	std::cout <<buffer << std::flush;
	return bytes_received;
}

/*int TcpComLink::Recv(std::string& buffer, int length)
{
	buffer = debug_string.substr(pointer_, util_min(length, debug_string.size() - pointer_));
	pointer_ += buffer.size();
	return buffer.size();
}*/
