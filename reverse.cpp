//
// reverse.cpp
//

#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Process.h"
#include "Poco/NamedEvent.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/NumberParser.h"
#include <iostream>

using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnectionFilter;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::UInt16;
using Poco::Event;
using Poco::NamedEvent;
using Poco::Process;
using Poco::ProcessImpl;
using Poco::Exception;
using Poco::NumberParser;


class ClientConnection: public TCPServerConnection
{
public:
    ClientConnection(const StreamSocket& s): TCPServerConnection(s) {}

    void run()
    {
        StreamSocket& ss = socket();
        try
        {
            char buffer[256];
            char str[256];
            char prompt[2] =">";

            int n = ss.sendBytes(welcomeStr, sizeof(welcomeStr));

            while(n > 0)
            {
               memset(buffer, 0, sizeof(buffer));
               int n = ss.receiveBytes(buffer, sizeof(buffer));

               strcat(str, buffer);

               if(buffer[0] == 0x0D)
               {
                   if(n > 255)
                   {
                       ss.sendBytes(exceedStr, sizeof(exceedStr));
                       ss.sendBytes(enterStr, sizeof(enterStr));
                       memset(str, 0, sizeof(str));
                       memset(buffer, 0, sizeof(buffer));
                       socket().close();
                       break;
                   }
                   else
                   {
                      trimString(str);
                      reverseString(str, 0, strlen(str) - 1);

                      if(strlen(str) != 0)
                        ss.sendBytes(prompt, 1);
                      ss.sendBytes(str, strlen(str));
                      memset(str, 0, sizeof(str));
                   }
               }
            }
        }
        catch (Exception& exc)
        {
            std::cerr << "ClientConnection exception: " << exc.displayText() << std::endl;
        }
    }
private:
    const char welcomeStr[50] = {">\"Welcome to POCO TCP server. Enter you string:\"\n"};
    const char exceedStr[45] = {">\"The line exceed 255 symbols. Job resets!\"\n"};
    const char enterStr[22] = {">\"Enter you string:\"\n"};

    void trimString(char str[])
    {
        char *pstr = str;

        while(true)
        {
            if(*pstr == 13 || *pstr == 10)
            {
                *pstr = 0;
                return;
            }
            pstr++;
        }
    }

    void reverseString(char str[], int idx, int size)
    {
        char a;

        a = str[idx];
        str[idx] = str[size - idx];
        str[size - idx] = a;

        if (idx == size / 2)
        {
            return;
        }
        reverseString(str, idx + 1, size);
    }
};

typedef TCPServerConnectionFactoryImpl<ClientConnection> TCPFactory;
Event terminator;

int main(int argc, char** argv)
{
    std::cout << "Simple POCO TcpServer started on localhost(127.0.0.1), port 28888." << std::endl;

    try
    {
        Poco::UInt16 port = 28888;

        TCPFactory *factory = new TCPFactory();
        TCPServer srv(factory, port);

        srv.start();

        terminator.wait();
    }
    catch (Exception& exc)
    {
        std::cerr << exc.displayText() << std::endl;
        return 1;
    }

    return 0;
}
