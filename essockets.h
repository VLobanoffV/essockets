#pragma once
// ������� ����������� � WINDOWS 
#define WIN32_LEAN_AND_MEAN 

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

// ����������� DLL ���������� WinSock2 
#pragma comment(lib, "Ws2_32.lib") 

namespace web {

    class server {

        WSADATA wsaDta;
        ADDRINFO hints;                                      // ��������� ������
        ADDRINFO* addrResult = NULL;                         // ��������� ��� ������ getaddrinfo()
        SOCKET ClientSocket = INVALID_SOCKET;                // �������� ������ � �������������,         INVALID_SOCKET - �������� ����� 
        SOCKET ListenSocket = INVALID_SOCKET;                // ��������� �����

        int result = 0;
        const char* sendBuffer = "Hello from Server";        // ����� ��� �������� ��������� �� ������
        char recvBuffer[512] = "";                           // ����� ��� ��Ȩ�� ��������� � �������




    public:
        server(ADDRINFO* addrinfo) {
            

            ZeroMemory(&hints, sizeof(hints));              // ������� ������ ���������
            hints.ai_family = addrinfo->ai_family;         // ����� ��������� ����������,                   AF_INET     - IpV4
            hints.ai_socktype = addrinfo->ai_socktype;    // ����� ����������� ������,                      SOCK_STREAM - ��������� �����
            hints.ai_protocol = addrinfo->ai_protocol;   // ����� ������������� ���������,                  IPPROTO_TCP - Tcp/IP
            hints.ai_flags = addrinfo->ai_flags;        // ��������� ���������� ��� ��������� �������       AI_PASSIVE  - ��������� ���������



        }

        ~server() {

            ReleaseResources();
        }

        // ���������� �������
        int ReleaseResources() {

            closesocket(ClientSocket);
            freeaddrinfo(addrResult);
            WSACleanup();

            return 0;
        }

        // ���������� ������ �������
        int SetWSADATA() {

            // �������� ������ ������� WINDOWS: MAKEWORD(2, 2)
            // � ������ �� ��������� WsaData
            result = WSAStartup(MAKEWORD(2, 2), &wsaDta);

            if (result != 0)
            {
                std::cout << "WSAStartup failed, result: " << result << std::endl;

                return 1;
            }

            return 0;
        }

        // ��������� ������� ���˨����� ���� � ����, � �������� ��������� � ���� ����
        int CheckSocket(PCSTR port) {

            result = getaddrinfo(NULL, port, &hints, &addrResult);
            if (result != 0)
            {
                std::cout << "getaddrinfo() failed, with error: " << result << std::endl;
                WSACleanup(); // � ������ ����� �������

                return 1;
            }

            return 0;
        }

        // ������� �����
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

        // ��������� ������ �� �������������        
        int SetListening() {

            result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen); // ���������:
            // ��������� �����:         ConnectSocket
            // ����� ��� ����������:    addrResult->ai_addr
            // ����� ����� ������:      addrResult->ai_addrlen

            // �������� ����������
            if (result == SOCKET_ERROR)
            {
                // � ������ ������ ���������� ����������� �������
                std::cout << "Binding socket failed" << std::endl;
                ListenSocket = INVALID_SOCKET;
                ReleaseResources();

                return 1;
            }

            return 0;
        }

        // ������������� ����� ������������� ������
        int SetListeningMode() {

            // listen() - �������� ����������� ��������, �Ĩ� ��������� �������� �����
            result = listen(ListenSocket, SOMAXCONN); // ���������:
            // ����� ��� �������������                  ListenSocket
            // ������������ ���-�� ����������           SOMAXCONN - ������������ ���-��

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

        // ��������� ���������� � ������� ���������� �����
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

        // ��������� ��������� �� ����
        int GetUpdates() {

            do
            {
                // ������� ����� ��� ��Ȩ��
                ZeroMemory(recvBuffer, 512);

                result = recv(ClientSocket, recvBuffer, 512, 0); // ���������:
                // �����:              ConnectSocket
                // ���� ���������:     recvBuffer
                // ������ �������      512   
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

        // ��������� ����� ��� ��������
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
        ADDRINFO hints;                                      // ��������� ������
        ADDRINFO* addrResult = NULL;                         // ��������� ��� ������ getaddrinfo()
        SOCKET ClientSocket = INVALID_SOCKET;                // �������� ������ � �������������,         INVALID_SOCKET - �������� ����� 

        int result = 0;
        const char* sendBuffer = "Hello from Server";        // ����� ��� �������� ��������� �� ������
        char recvBuffer[512] = "";                           // ����� ��� ��Ȩ�� ��������� � �������

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
            result = WSAStartup(MAKEWORD(2, 2), &wsaDta); // �������� ������ ������� WINDOWS: MAKEWORD(2, 2)
            // � ������ �� ��������� WsaData
            if (result != 0)
            {
                std::cout << "WSAStartup failed, result: " << result << std::endl;
                return 1;
            }
            return 0;
        }

        int CheckSocket(PCSTR port) {

            result = getaddrinfo("localhost", port, &hints, &addrResult); // ��������� ������� ���˨����� ���� � ����, � �������� ��������� � ���� ����
            if (result != 0)
            {
                std::cout << "getaddrinfo() failed, with error: " << result << std::endl;
                WSACleanup(); // � ������ ����� �������
                return 1;
            }
            return 0;
        }

        int InitConnection() {

            // ������������� ����� ����������
            ClientSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol); // ���������:
            // ��������� ���������: addrResult->ai_family
            // ��� ������:       addrResult->ai_socktype
            // ��� ���������:    addrResult->ai_protocol
            // ������� �������� ������
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

            // ��������� ����������
            result = connect(ClientSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen); // ���������:
            // ��������� �����:         ConnectSocket
            // ����� ��� ����������:    addrResult->ai_addr
            // ����� ����� ������:      addrResult->ai_addrlen

            // �������� ����������
            if (result == SOCKET_ERROR)
            {
                // � ������ ������ ���������� ����������� �������
                std::cout << "Connection error!" << std::endl;
                ClientSocket = INVALID_SOCKET;
                ReleaseResources(ClientSocket);
                return 1;
            }
            return 0;
        }

        int Send() {

            // �������� ��������� �� ������
            result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), 0); // ���������:
            // ����� ����������:     ConnectSocket
            // ���������:            sendBuffer
            // ����� ���������:      (int)strlen(sendBuffer)

            // ���������� ���������� ���������� ������ ���� SOCKET_ERROR
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

            // ���������� ������
            result = shutdown(ClientSocket, SD_SEND); // ���������:
            // �����:                    ConnectSocket
            // �������� ����������:      SD_SEND         - ���������� �� ��������
            // � ������ ������ ����������� �Ѩ � ������� 
            if (result == SOCKET_ERROR)
            {
                std::cout << "Shutdown error: " << result << std::endl;
                ReleaseResources(ClientSocket);
                return 1;

            }
            return 0;

        }

        int GetUpdates() {

            // ��Ȩ� ������ �� �������� ����������
            do
            {
                // ������� ����� ��� ��Ȩ��
                ZeroMemory(recvBuffer, 512);

                result = recv(ClientSocket, recvBuffer, 512, 0); // ���������:
                // �����:              ConnectSocket
                // ���� ���������:     recvBuffer
                // ������ �������      520   
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
