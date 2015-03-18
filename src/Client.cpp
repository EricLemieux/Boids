#include "Client.h"

Client::Client()
{
    //ctor
}

Client::~Client()
{
    //dtor
}

void Client::Activate(const char* address)
{
    sockDesc = new RakNet::SocketDescriptor(clientPort, NULL);
    sockDesc->socketFamily = AF_INET;

    interface = RakNet::RakPeerInterface::GetInstance();

    RakNet::StartupResult res = interface->Startup(1, sockDesc, 1);
    assert(res == RakNet::RAKNET_STARTED && "client Count Not Connect.\n");

    RakNet::ConnectionAttemptResult conRes = interface->Connect("localhost", serverPort, password, strlen(password));
    assert(conRes == RakNet::CONNECTION_ATTEMPT_STARTED && "Client could not connect.\n");
}

void Client::Send(std::string message)
{
    interface->Send(message.c_str(), message.length(), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

std::string Client::Recieve()
{
    while ((packet = interface->Receive()) != NULL)
    {
        std::string str = std::string((char*)packet->data).substr(0, packet->length);

        interface->DeallocatePacket(packet);
    }
}

std::string Client::GetType()
{
    return std::string("Client");
}
