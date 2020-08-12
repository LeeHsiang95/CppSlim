#include "CppUTest/TestHarness.h"

#include "Slim.hpp"
#include "SlimList.hpp"
#include "TcpComLink.hpp"
#include "SlimConnectionHandler.hpp"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "compatibility.hpp"


class MockComLink : public ComLink {
public:
    
    int Send(const std::string& msg) override {
        lastSendMsg += msg;
        
        int result = msg.size();
        if (sendReturnCodes->GetLength() > 0)
        {
            auto resultAsAString = sendReturnCodes->GetStringAt(0);
            result = std::stoi(resultAsAString);

            sendReturnCodes->PopHead();
        }

        return result;
    }

    int Recv(std::string& buffer, int length) override {
        buffer = recv_str_.substr(0, length);
        recv_str_ = recv_str_.c_str() + length;
        return buffer.size();
    }

    std::string lastSendMsg;
    std::string recv_str_;
    std::shared_ptr<SlimList> sendReturnCodes;
};


  class MockSlim : public ISlim {
  public:
      std::string slimResponse;
      std::string sentSlimMessage;

      std::string HandleMessage(const std::string& message)
      {
          sentSlimMessage = message;
          return slimResponse;
      }
  };

  void AddSendResult(std::shared_ptr<MockComLink> self, int result)
  {
	  char string[22];
	  sprintf(string, "%d", result);
      self->sendReturnCodes->AddString(string);
  }


TEST_GROUP(SlimConnectionHandler)
{
    std::shared_ptr<SlimConnectionHandler> slimConnectionHandler;
    std::shared_ptr<MockComLink> comLink;
    std::shared_ptr<MockSlim> mockMessageHandler;
    void setup()
    {
        comLink = std::make_shared<MockComLink>();

        slimConnectionHandler = std::make_shared<SlimConnectionHandler>(comLink);
        comLink->lastSendMsg = "";
        mockMessageHandler = std::make_shared<MockSlim>();

        comLink->sendReturnCodes = std::make_shared<SlimList>();
        slimConnectionHandler->RegisterSlimMessageHandler(mockMessageHandler);
    }

    void teardown()
    {
    }
};

TEST(SlimConnectionHandler, ShouldSendVersion)
{
    comLink->recv_str_ = "000003:bye";
    slimConnectionHandler->Run();
    CHECK_EQUAL("Slim -- V0.2\n", comLink->lastSendMsg);
}

TEST(SlimConnectionHandler, ShouldReadMessageAndCallSlimHandler)
{
    comLink->recv_str_ = "000006:abcdef000003:bye";

    mockMessageHandler->slimResponse = "ghijklm";
    slimConnectionHandler->Run();

    CHECK_EQUAL("Slim -- V0.2\n000007:ghijklm", comLink->lastSendMsg);
    CHECK_EQUAL("abcdef", mockMessageHandler->sentSlimMessage);
}

TEST(SlimConnectionHandler, CanMockSendResultsAsPartOfTest)
{
	int expected = 1;
	AddSendResult(comLink, expected);
	char message[] = "";

	int actual = comLink->Send(message);
	CHECK_EQUAL(expected, actual);
}

TEST(SlimConnectionHandler, HandlesSendErrorWithoutMemoryLeak)
{
	comLink->recv_str_ = "";
	AddSendResult(comLink, -1);
    slimConnectionHandler->Run();
}
