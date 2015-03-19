#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <raknet/RakPeer.h>
#include <raknet/MessageIdentifiers.h>
#include <iostream>

#include "Boid.h"

class NetworkInterface
{
    public:
        NetworkInterface();
        virtual ~NetworkInterface();

        virtual void Activate(const char* address);
        virtual void Send(std::string message);
        virtual void Recieve(Boid* flock[BOID_COUNT]);

        virtual std::string GetType();
    protected:
        static const int clientPort = 8081;
        static const int serverPort = 8080;
        const char* password = "cats";

        RakNet::RakPeerInterface* netInterface;
        RakNet::SocketDescriptor* sockDesc;

        RakNet::Packet* packet;
        char packetID;

        const int maxConnections = 2;
    private:
};

#endif // NETWORKINTERFACE_H
