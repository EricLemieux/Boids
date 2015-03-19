#ifndef CLIENT_H
#define CLIENT_H

#include "NetworkInterface.h"

class Client : public NetworkInterface
{
    public:
        Client();
        virtual ~Client();

        virtual void Activate(const char* address);
        virtual void Send(std::string message);
        virtual void Recieve(Boid* flock[100]);

        virtual std::string GetType();
    protected:
    private:
};

#endif // CLIENT_H
