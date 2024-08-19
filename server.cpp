#include "essockets.h"

int main() {

	ADDRINFO addr;
	addr.ai_family = AF_INET;         // ����� ��������� ����������,                   AF_INET     - IpV4
	addr.ai_socktype = SOCK_STREAM;   // ����� ����������� ������,                     SOCK_STREAM - ��������� �����
	addr.ai_protocol = IPPROTO_TCP;   // ����� ������������� ���������,                IPPROTO_TCP - Tcp/IP
	addr.ai_flags = AI_PASSIVE;       // ��������� ���������� ��� ��������� �������    AI_PASSIVE  - ��������� ���������

	web::server server(&addr);
	server.SetWSADATA();
	server.CheckSocket("666");
	server.InitConnection();
	server.SetListening();
	server.SetListeningMode();
	server.AcceptConnection();
	server.GetUpdates();
	server.CloseForSending();


	return 0;
}