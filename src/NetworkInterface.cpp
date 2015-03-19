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

void NetworkInterface::Recieve(Boid* flock[BOID_COUNT])
{
}

std::string NetworkInterface::GetType()
{
    return std::string("Interface");
}
