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

std::string NetworkInterface::Recieve()
{
	return std::string("");
}

std::string NetworkInterface::GetType()
{
    return std::string("Interface");
}
