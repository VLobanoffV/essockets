#pragma once
#define WIN32_LEAN_AND_MEAN 

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") 

namespace web {

    class server {

        WSADATA wsaDta;
        ADDRINFO hints;                                      
        ADDRINFO* addrResult = NULL;                         
        SOCKET ClientSocket = INVALID_SOCKET;                
        SOCKET ListenSocket = INVALID_SOCKET;                

        int result = 0;
        const char* sendBuffer = "Hello from Server";        
        char recvBuffer[512] = "";                           




    public:
        server(ADDRINFO* addrinfo) {
            

            ZeroMemory(&hints, sizeof(hints));              
            hints.ai_family = addrinfo->ai_family;                            
            hints.ai_socktype = addrinfo->ai_socktype;    
            hints.ai_protocol = addrinfo->ai_protocol;  
            hints.ai_flags = addrinfo->ai_flags;        



        }

        ~server() {

            ReleaseResources();
        }

        int ReleaseResources() {

            closesocket(ClientSocket);
            freeaddrinfo(addrResult);
            WSACleanup();

            return 0;
        }

        int SetWSADATA() {

            result = WSAStartup(MAKEWORD(2, 2), &wsaDta);

            if (result != 0)
            {
                std::cout << "WSAStartup failed, result: " << result << std::endl;

                return 1;
            }

            return 0;
        }

        int CheckSocket(PCSTR port) {

            result = getaddrinfo(NULL, port, &hints, &addrResult);
            if (result != 0)
            {
                std::cout << "getaddrinfo() failed, with error: " << result << std::endl;
                WSACleanup();

                return 1;
            }

            return 0;
        }

        int InitConnection() {

            ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

            if (ListenSocket == INVALID_SOCKET)
            {
                std::cout << "Socket creation failed" << std::endl;
                freeaddrinfo(addrResult);
                WSACleanup();

                return 1;
            }

            return 0;
        }

        int SetListening() {

            result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);

            if (result == SOCKET_ERROR)
            {
                std::cout << "Binding socket failed" << std::endl;
                ListenSocket = INVALID_SOCKET;
                ReleaseResources();

                return 1;
            }

            return 0;
        }

        int SetListeningMode() {

            result = listen(ListenSocket, SOMAXCONN);

            if (result == SOCKET_ERROR)
            {
                std::cout << "Listening socket failed" << std::endl;
                ReleaseResources();

                return 1;

            }
            else
            {
                std::cout << "Socket: " << ListenSocket << " has been listening";
            }

            return 0;
        }

        int AcceptConnection() {

            ClientSocket = accept(ListenSocket, NULL, NULL);

            if (ClientSocket == INVALID_SOCKET)
            {
                std::cout << "Accepting socket failed" << std::endl;
                ReleaseResources();

                return 1;

            }

            closesocket(ListenSocket);

            return 0;
        }

        int GetUpdates() {

            do
            {
                ZeroMemory(recvBuffer, 512);

                result = recv(ClientSocket, recvBuffer, 512, 0);

                if (result > 0)
                {
                    std::cout << "Received " << result << " bytes" << std::endl;
                    std::cout << "Received data: " << recvBuffer << std::endl;

                    result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), 0);
                    if (result == SOCKET_ERROR)
                    {
                        std::cout << "Failed to send data back" << std::endl;
                        ReleaseResources();

                        return 1;

                    }
                }
                else if (result == 0)
                {
                    std::cout << "Connection closing..." << std::endl;
                }
                else {
                    std::cout << "Recv failed with error: " << result << std::endl;
                    ReleaseResources();

                    return 1;
                }
            } while (result > 0);

            return 0;
        }

        int CloseForSending() {

            result = shutdown(ClientSocket, SD_SEND);
            if (result == SOCKET_ERROR)
            {
                std::cout << "Failed shutdown" << std::endl;
                ReleaseResources();

                return 1;
            }

            return 0;
        }

    };

    class client {

        WSADATA wsaDta;
        ADDRINFO hints;                                      
        ADDRINFO* addrResult = NULL;                         
        SOCKET ClientSocket = INVALID_SOCKET;                

        int result = 0;
        const char* sendBuffer = "Hello from Server";        
        char recvBuffer[512] = "";                           

    public:
        client(ADDRINFO* addrinfo) {

            ZeroMemory(&hints, sizeof(hints));
            hints.ai_family = addrinfo->ai_family;
            hints.ai_socktype = addrinfo->ai_socktype;
            hints.ai_protocol = addrinfo->ai_protocol;

        }

        ~client() {

            ReleaseResources(ClientSocket);

        }

        int ReleaseResources(SOCKET& sock) {



            closesocket(sock);
            freeaddrinfo(addrResult);
            WSACleanup();
            sock = INVALID_SOCKET;
            return 0;

        }

        int SetWSADATA() {

            int result;
            result = WSAStartup(MAKEWORD(2, 2), &wsaDta);
            
            if (result != 0)
            {
                std::cout << "WSAStartup failed, result: " << result << std::endl;
                return 1;
            }
            return 0;
        }

        int CheckSocket(PCSTR port) {

            result = getaddrinfo("localhost", port, &hints, &addrResult); 
            if (result != 0)
            {
                std::cout << "getaddrinfo() failed, with error: " << result << std::endl;
                WSACleanup(); 
                return 1;
            }
            return 0;
        }

        int InitConnection() {

            ClientSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

            if (ClientSocket == INVALID_SOCKET)
            {
                std::cout << "Socket creation failed" << std::endl;
                freeaddrinfo(addrResult);
                WSACleanup();
                return 1;
            }

            return 0;
        }

        int CheckConnection() {

            result = connect(ClientSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen); 

            if (result == SOCKET_ERROR)
            {
                std::cout << "Connection error!" << std::endl;
                ClientSocket = INVALID_SOCKET;
                ReleaseResources(ClientSocket);
                return 1;
            }
            return 0;
        }

        int Send() {

            result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), 0);

            if (result == SOCKET_ERROR)
            {
                std::cout << "Send failed, error:" << result << std::endl;
                ReleaseResources(ClientSocket);
                return 1;
            }

            std::cout << "Send: " << result << " bytes" << std::endl;
            return 0;
        }

        int CloseSocket() {

            result = shutdown(ClientSocket, SD_SEND);

            if (result == SOCKET_ERROR)
            {
                std::cout << "Shutdown error: " << result << std::endl;
                ReleaseResources(ClientSocket);
                return 1;

            }
            return 0;

        }

        int GetUpdates() {

            do
            {
                ZeroMemory(recvBuffer, 512);

                result = recv(ClientSocket, recvBuffer, 512, 0);

                if (result > 0)
                {
                    std::cout << "Received " << result << " bytes" << std::endl;
                    std::cout << "Received data: " << recvBuffer << std::endl;
                }
                else if (result == 0)
                {
                    std::cout << "Connection closed" << std::endl;
                }
                else {
                    std::cout << "Recv failed with error: " << result << std::endl;
                }
            } while (result > 0);

            return 0;
        }

    };
}
