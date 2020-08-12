#include "SlimConnectionHandler.hpp"
#include "SlimList.hpp"
#include "TcpComLink.hpp"
#include "Slim.hpp"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


SlimConnectionHandler::SlimConnectionHandler(std::shared_ptr<ComLink> comLink) {
	com_link_ = comLink;
}

void SlimConnectionHandler::RegisterSlimMessageHandler(std::shared_ptr<ISlim> slim) {
	slim_ = slim;
}

int SlimConnectionHandler::read_size()
{
	std::string size;
	int size_i = 0;
	std::string colon;

	int receiveResult = com_link_->Recv(size, 6);
	if (receiveResult == 6)
	{
		if ((com_link_->Recv(colon, 1)) == 1 && colon == ":")
		{
			size_i = stoi(size);
		}
	} else if (receiveResult == -1) {
		size_i = -1;;
	}
	return size_i;
}


int SlimConnectionHandler::Run()
{
	if (com_link_->Send("Slim -- V0.2\n") == -1)
	{
		return -1;
	}

	while(1)
	{
		int size_i = read_size();

		if (size_i > 0)
		{
			std::string message;
			int numbytes = com_link_->Recv(message, size_i);
			if (numbytes != size_i)
			{
				printf("did not receive right number of bytes.  %d expected but received %d\n", size_i, numbytes);
				break;
			}
			if ("bye" == message)
			{
				break;
			}
			//execute and get response
			auto response_message = slim_->HandleMessage(message);
			char length_buffer[8];
			sprintf(length_buffer, "%06d:", response_message.size());
			int send_result = com_link_->Send(length_buffer);
			send_result = com_link_->Send(response_message);
		} else if (size_i == -1)
		{
			break;
		}
	}
	fflush(stdout);
	return 0;

}
