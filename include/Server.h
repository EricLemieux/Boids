#ifndef SERVER_H
#define SERVER_H

#include "NetworkInterface.h"

class Server : public NetworkInterface
{
    public:
        Server();
        virtual ~Server();

        virtual void Activate(const char* address);
        virtual void Send(std::string message);
        virtual std::string Recieve();

        virtual std::string GetType();
    protected:
    private:
};

#endif // SERVER_H
