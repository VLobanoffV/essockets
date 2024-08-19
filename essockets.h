#pragma once
// люйпняя менаундхлши б WINDOWS 
#define WIN32_LEAN_AND_MEAN 

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

// ондйкчвемхе DLL ахакхнрейх WinSock2 
#pragma comment(lib, "Ws2_32.lib") 

namespace web {

    class server {

        WSADATA wsaDta;
        ADDRINFO hints;                                      // срнвмемхъ янйерю
        ADDRINFO* addrResult = NULL;                         // ярпсйрспю дкъ нрберю getaddrinfo()
        SOCKET ClientSocket = INVALID_SOCKET;                // янгдюмхе янйерю х хмхжхюкхгюжхъ,         INVALID_SOCKET - мебепмши янйер 
        SOCKET ListenSocket = INVALID_SOCKET;                // яксыючыхи янйер

        int result = 0;
        const char* sendBuffer = "Hello from Server";        // астеп дкъ нропюбйх яннаыемхъ мю яепбеп
        char recvBuffer[512] = "";                           // астеп дкъ опх╗лю яннаыемхъ я яепбепю




    public:
        server(ADDRINFO* addrinfo) {
            

            ZeroMemory(&hints, sizeof(hints));              // нвхярйю оюлърх ярпсйрспш
            hints.ai_family = addrinfo->ai_family;         // бшанп яелеиярбю опнрнйнкнб,                   AF_INET     - IpV4
            hints.ai_socktype = addrinfo->ai_socktype;    // бшанп онксвюелнцн янйерю,                      SOCK_STREAM - онрнйнбши янйер
            hints.ai_protocol = addrinfo->ai_protocol;   // бшанп хяонкэгселнцн опнрнйнкю,                  IPPROTO_TCP - Tcp/IP
            hints.ai_flags = addrinfo->ai_flags;        // онксвемхе хмтнплюжхх дкъ оюяяхбмни ярнпнмш       AI_PASSIVE  - оюяяхбмне онксвемхе



        }

        ~server() {

            ReleaseResources();
        }

        // нябнандхрэ пеяспяш
        int ReleaseResources() {

            closesocket(ClientSocket);
            freeaddrinfo(addrResult);
            WSACleanup();

            return 0;
        }

        // сярюмнбхрэ бепяхч янйернб
        int SetWSADATA() {

            // оепедю╗л бепяхч янйернб WINDOWS: MAKEWORD(2, 2)
            // х яяшкйс мю ярпсйрспс WsaData
            result = WSAStartup(MAKEWORD(2, 2), &wsaDta);

            if (result != 0)
            {
                std::cout << "WSAStartup failed, result: " << result << std::endl;

                return 1;
            }

            return 0;
        }

        // опнбепъер мюкхвхе сдюк╗ммнцн сгкю б яерх, х ошрюеряъ онярпнхрэ й мелс осрэ
        int CheckSocket(PCSTR port) {

            result = getaddrinfo(NULL, port, &hints, &addrResult);
            if (result != 0)
            {
                std::cout << "getaddrinfo() failed, with error: " << result << std::endl;
                WSACleanup(); // б яксвюе теикю нвхыюел

                return 1;
            }

            return 0;
        }

        // янгдю╗р янйер
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

        // сярюмнбйю янйерю мю опняксьхбюмхе        
        int SetListening() {

            result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen); // опхмхлюер:
            // янгдюммши янйер:         ConnectSocket
            // юдпея дкъ янедхмемхъ:    addrResult->ai_addr
            // дкхмю хлемх юдпеяю:      addrResult->ai_addrlen

            // опнбепйю янедхмемхъ
            if (result == SOCKET_ERROR)
            {
                // б яксвюе ньхайх янедхмемхъ нябнанфдюел пеяспяш
                std::cout << "Binding socket failed" << std::endl;
                ListenSocket = INVALID_SOCKET;
                ReleaseResources();

                return 1;
            }

            return 0;
        }

        // сярюмюбкхбюел пефхл опняксьхбюмхъ янйерю
        int SetListeningMode() {

            // listen() - ъбкъеряъ акнйхпсчыеи тсмйжхеи, фд╗р яннаыемхе акнйхпсъ онрнй
            result = listen(ListenSocket, SOMAXCONN); // опхмхлюер:
            // янйер дкъ опняксьхбюмхъ                  ListenSocket
            // люйяхлюкэмне йнк-бн янедхмемхи           SOMAXCONN - люйяхлюкэмне йнк-бн

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

        // пюгпеьюел янедхмемхе х янгдю╗л йкхемряйхи янйер
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

        // опхмхлюел яннаыемхъ нр сгкю
        int GetUpdates() {

            do
            {
                // нвхыюел астеп дкъ опх╗лю
                ZeroMemory(recvBuffer, 512);

                result = recv(ClientSocket, recvBuffer, 512, 0); // опхмхлюер:
                // янйер:              ConnectSocket
                // йсдю опхмхлюрэ:     recvBuffer
                // пюглеп асттепю      512   
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

        // гюйпшбюел янйер дкъ нропюбйх
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
        ADDRINFO hints;                                      // срнвмемхъ янйерю
        ADDRINFO* addrResult = NULL;                         // ярпсйрспю дкъ нрберю getaddrinfo()
        SOCKET ClientSocket = INVALID_SOCKET;                // янгдюмхе янйерю х хмхжхюкхгюжхъ,         INVALID_SOCKET - мебепмши янйер 

        int result = 0;
        const char* sendBuffer = "Hello from Server";        // астеп дкъ нропюбйх яннаыемхъ мю яепбеп
        char recvBuffer[512] = "";                           // астеп дкъ опх╗лю яннаыемхъ я яепбепю

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
            result = WSAStartup(MAKEWORD(2, 2), &wsaDta); // оепедю╗л бепяхч янйернб WINDOWS: MAKEWORD(2, 2)
            // х яяшкйс мю ярпсйрспс WsaData
            if (result != 0)
            {
                std::cout << "WSAStartup failed, result: " << result << std::endl;
                return 1;
            }
            return 0;
        }

        int CheckSocket(PCSTR port) {

            result = getaddrinfo("localhost", port, &hints, &addrResult); // опнбепъер мюкхвхе сдюк╗ммнцн сгкю б яерх, х ошрюеряъ онярпнхрэ й мелс осрэ
            if (result != 0)
            {
                std::cout << "getaddrinfo() failed, with error: " << result << std::endl;
                WSACleanup(); // б яксвюе теикю нвхыюел
                return 1;
            }
            return 0;
        }

        int InitConnection() {

            // хмхжхюкхгюжхъ янйер янедхмемхъ
            ClientSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol); // опхмхлюер:
            // яелеиярбн опнрнйнкю: addrResult->ai_family
            // рхо янйерю:       addrResult->ai_socktype
            // рхо опнрнйнкю:    addrResult->ai_protocol
            // опбепйю янгдюмхъ янйерю
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

            // сярюмнбйю янедхмемхъ
            result = connect(ClientSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen); // опхмхлюер:
            // янгдюммши янйер:         ConnectSocket
            // юдпея дкъ янедхмемхъ:    addrResult->ai_addr
            // дкхмю хлемх юдпеяю:      addrResult->ai_addrlen

            // опнбепйю янедхмемхъ
            if (result == SOCKET_ERROR)
            {
                // б яксвюе ньхайх янедхмемхъ нябнанфдюел пеяспяш
                std::cout << "Connection error!" << std::endl;
                ClientSocket = INVALID_SOCKET;
                ReleaseResources(ClientSocket);
                return 1;
            }
            return 0;
        }

        int Send() {

            // нропюбйю яннаыемхъ мю яепбеп
            result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), 0); // опхмхлюер:
            // янйер янедхмемхъ:     ConnectSocket
            // яннаыемхе:            sendBuffer
            // дкхмю яннаыемхъ:      (int)strlen(sendBuffer)

            // бнгбпюыюер йнкхвеярбн оепедюммшу дюммшу кхан SOCKET_ERROR
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

            // бшйкчвемхе янйерю
            result = shutdown(ClientSocket, SD_SEND); // опхмхлюер:
            // янйер:                    ConnectSocket
            // оюпюлерп бшйкчвемхъ:      SD_SEND         - бшйкчвемхе мю оепедювс
            // б яксвюе ньхайх нябнанфдюел бя╗ х бшундхл 
            if (result == SOCKET_ERROR)
            {
                std::cout << "Shutdown error: " << result << std::endl;
                ReleaseResources(ClientSocket);
                return 1;

            }
            return 0;

        }

        int GetUpdates() {

            // опх╗л дюммшу дн гюйпшрхъ янедхмемхъ
            do
            {
                // нвхыюел астеп дкъ опх╗лю
                ZeroMemory(recvBuffer, 512);

                result = recv(ClientSocket, recvBuffer, 512, 0); // опхмхлюер:
                // янйер:              ConnectSocket
                // йсдю опхмхлюрэ:     recvBuffer
                // пюглеп асттепю      520   
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
