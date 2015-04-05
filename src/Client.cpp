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

	RakNet::ConnectionAttemptResult conRes = netInterface->Connect(address, serverPort, password, strlen(password));
    assert(conRes == RakNet::CONNECTION_ATTEMPT_STARTED && "Client could not connect.\n");
}

void Client::Send(std::string message)
{
	netInterface->Send(message.c_str(), message.length(), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::Recieve(Boid* flock[BOID_COUNT])
{
	while ((packet = netInterface->Receive()) != NULL)
    {
        std::string str = std::string((char*)packet->data).substr(0, packet->length);

        int id;
        glm::vec3 pos, forwardVec;
        sscanf(str.c_str(),"%i %f %f %f - %f %f %f",&id,&pos.x,&pos.y,&pos.z,&forwardVec.x, &forwardVec.y, &forwardVec.z);
		if (id < BOID_COUNT && id >= 0)
		{
			flock[id]->SetRemote(pos, forwardVec);
		}

		netInterface->DeallocatePacket(packet);
    }
}

std::string Client::GetType()
{
    return std::string("Client");
}
