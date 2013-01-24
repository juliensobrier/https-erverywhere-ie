#include "common.h"

#include "MTServer.h"
#include "debug.h"

#include <sstream>

using namespace std;

static SOCKET server = NULL;

int Server::Start(int portStart, int portEnd)
{
	WSADATA wsaData;
	sockaddr_in local;

	if (portEnd < portStart)
	{
		Debug::Error("port %i < %i !", portEnd, portStart);
		return 0;
	}

	int port = portStart;
	
	int wsaret = WSAStartup(0x101,&wsaData);
	if(wsaret!=0)
	{
		Debug::Error("WSAStartup failed");
		return 0;
	}

	while (port <= portEnd)
	{
		local.sin_family=AF_INET;
		//local.sin_addr.s_addr=INADDR_ANY;
		local.sin_addr.s_addr = inet_addr("127.0.0.1");
		local.sin_port=htons((u_short)port);
		server=socket(AF_INET,SOCK_STREAM,0);
		if(server==INVALID_SOCKET)
		{
			Debug::Warn("Could not create socket on port %i", port);
			port += 1;
			continue;
		}
		if(bind(server,(sockaddr*)&local,sizeof(local)) !=0 )
		{
			Debug::Warn("Could not bind to port %i", port);
			//return 0;

			//some kind of cleanup needed?
			port += 1;
			continue;
		}
		if(listen(server,20 )!=0 )
		{
			Debug::Error("Could not listen to port %i", port);
			return 0;
		}

		AfxBeginThread(Server::MTServerThread,(LPVOID)server);

		return port;
	}

	return 0;
}

void Server::Stop()
{
	if (server != NULL)
	{
		closesocket(server);
		WSACleanup();
	}
}


UINT Server::MTServerThread(LPVOID pParam)
{		
	SOCKET client;
	sockaddr_in from;
	int fromlen=sizeof(from);
	SOCKET server = (SOCKET)pParam;

	while(true)
	{
		client=accept(server,
			(struct sockaddr*)&from,&fromlen);
		AfxBeginThread(Server::ClientThread,(LPVOID)client);	
	}	

	return 0;
}

UINT Server::ClientThread(LPVOID pParam)
{
	char buffer[512];
	std::string data("");
	CString cmd;
	CString params;
	int n;
	SOCKET client=(SOCKET)pParam;

	try
	{
		do
		{
			n = recv(client,buffer,512,0);
			if(n == SOCKET_ERROR )
				break;
			buffer[n]=0;

			data.append(buffer);

			if (data.find("\r\n\r\n") != std::string::npos)
			{
				//Debug::Log("%s", data.c_str());

				// End of client data
				int start = data.find("X-Redirect: ");
				if (start > 0) {
					start += strlen("X-Redirect: ");
					int end = data.find_first_of("\r\n", start + 1);
					std::string redirect = data.substr(start, end - start);
					//Debug::Log("X-Redirect value length: %i", redirect.size());

					//std::string redirection("HTTP/1.1 302 Found\r\n");
					std::string redirection("HTTP/1.0 302 Found\r\n");
					//redirection.append("Content-length: 0\r\n");
					//redirection.append("Connection: close\r\n");
					//redirection.append("Content-Type: image/gif\r\n");
					//redirection.append("Host: www.google.com\r\n");
					redirection.append("Location: ");
					redirection.append(redirect);
					//redirection.append("\r\n");
					//redirection.append("Content-Type: text/html; charset=UTF-8\r\n");
					redirection.append("\r\n\r\n");

					//Debug::Log("%s", data.c_str());

					send(client,redirection.c_str(),strlen(redirection.c_str()),0);
					Debug::Log("%s", redirection.c_str());
				}
				else {
					Debug::Error("X-Redirect not found");
					std::string redirection("HTTP/1.0 500 Internal Error\r\n");
					redirection.append("Content-length: 0\r\n");
					redirection.append("\r\n\r\n");

					send(client,redirection.c_str(),strlen(redirection.c_str()),0);
				}

				break;
			}
		}		
		while (n > 0);
	}
	catch(std::exception &e)
	{
		Debug::Error(e.what());
	}
	catch(...)
	{
		Debug::Error("Server::ClientThread SEH error");
	}

	closesocket(client);
	return 0;
}
