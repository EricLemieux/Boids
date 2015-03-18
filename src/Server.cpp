#include "Server.h"

Server::Server()
{
    //ctor
}

Server::~Server()
{
    //dtor
}


void Server::Activate(const char*)
{
    sockDesc = new RakNet::SocketDescriptor(serverPort, NULL);
	sockDesc->socketFamily = AF_INET;

	interface = RakNet::RakPeerInterface::GetInstance();
	RakNet::StartupResult res = interface->Startup(maxConnections, sockDesc, 1);

	interface->SetMaximumIncomingConnections(maxConnections);

	assert(res == RakNet::RAKNET_STARTED && "Server Could Not Start.\n");

	std::cout << "Server Started.\n";

	interface->SetIncomingPassword(password, strlen(password));
}

void Server::Send(std::string message)
{
    interface->Send(message.c_str(), message.length(), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

std::string Server::Recieve()
{
    while ((packet = interface->Receive()) != NULL)
	{
		if (packet->data[0] == ID_NEW_INCOMING_CONNECTION)
		{
			std::cout << "A remote system has connected.\n";
		}
		else if (packet->data[0] == ID_DISCONNECTION_NOTIFICATION)
		{
			std::cout << "A remote system has been disconnected.\n";
		}
		else if (packet->data[0] == ID_CONNECTION_LOST)
		{
			std::cout << "A remote system has lost it's connected.\n";
		}
		else
		{
            std::string str = std::string((char*)packet->data).substr(0, packet->length);

		}
		interface->DeallocatePacket(packet);
    }
}

std::string Server::GetType()
{
    return std::string("Server");
}
