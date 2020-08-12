#pragma once
#include <string>

#include <winsock2.h>

class ComLink {
public:
    virtual int Send(const std::string& msg) = 0;
    virtual int Recv(std::string& buffer, int length) = 0;
};

class TcpComLink : public ComLink {
public:
    TcpComLink(int socket);

    int Send(const std::string& msg) override;
    int Recv(std::string& buffer, int length) override;

    SOCKET socket_;

private:
    //int pointer_ = 0;
    //std::string debug_string = "000905:[000010:000081:[000004:000015:scriptTable_0_0:000004:make:000016:scriptTableActor:000005:Count:]:000081:[000004:000015:scriptTable_0_1:000004:call:000016:scriptTableActor:000005:count:]:000081:[000004:000015:scriptTable_0_2:000004:call:000016:scriptTableActor:000005:count:]:000083:[000004:000015:scriptTable_0_3:000004:call:000016:scriptTableActor:000007:counter:]:000081:[000004:000015:scriptTable_0_4:000004:call:000016:scriptTableActor:000005:count:]:000081:[000004:000015:scriptTable_0_5:000004:call:000016:scriptTableActor:000005:count:]:000083:[000004:000015:scriptTable_0_6:000004:call:000016:scriptTableActor:000007:counter:]:000081:[000004:000015:scriptTable_0_7:000004:call:000016:scriptTableActor:000005:count:]:000081:[000004:000015:scriptTable_0_8:000004:call:000016:scriptTableActor:000005:count:]:000083:[000004:000015:scriptTable_0_9:000004:call:000016:scriptTableActor:000007:counter:]:]";
};
