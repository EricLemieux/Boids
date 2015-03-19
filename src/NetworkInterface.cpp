#include "NetworkInterface.h"

NetworkInterface::NetworkInterface()
{

}

NetworkInterface::~NetworkInterface()
{
    //dtor
}

void NetworkInterface::Activate(const char*)
{

}

void NetworkInterface::Send(std::string)
{

}

void NetworkInterface::Recieve(Boid* flock[100])
{
}

std::string NetworkInterface::GetType()
{
    return std::string("Interface");
}
