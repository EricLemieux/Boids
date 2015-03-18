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

	netInterface = RakNet::RakPeerInterface::GetInstance();

	RakNet::StartupResult res = netInterface->Startup(1, sockDesc, 1);
    assert(res == RakNet::RAKNET_STARTED && "client Count Not Connect.\n");

	RakNet::ConnectionAttemptResult conRes = netInterface->Connect("localhost", serverPort, password, strlen(password));
    assert(conRes == RakNet::CONNECTION_ATTEMPT_STARTED && "Client could not connect.\n");
}

void Client::Send(std::string message)
{
	netInterface->Send(message.c_str(), message.length(), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

std::string Client::Recieve()
{
	while ((packet = netInterface->Receive()) != NULL)
    {
        std::string str = std::string((char*)packet->data).substr(0, packet->length);

		netInterface->DeallocatePacket(packet);
    }

	return std::string("");
}

std::string Client::GetType()
{
    return std::string("Client");
}