//#include "essockets.h"
//
//int main() {
//
//	ADDRINFO addr;
//	addr.ai_family = AF_INET;         // ����� ��������� ����������,                   AF_INET     - IpV4
//	addr.ai_socktype = SOCK_STREAM;   // ����� ����������� ������,                     SOCK_STREAM - ��������� �����
//	addr.ai_protocol = IPPROTO_TCP;   // ����� ������������� ���������,                IPPROTO_TCP - Tcp/IP
//	addr.ai_flags = AI_PASSIVE;       // ��������� ���������� ��� ��������� �������    AI_PASSIVE  - ��������� ���������
//
//
//	web::client client(&addr);
//	client.SetWSADATA();
//	client.CheckSocket("666");
//	client.InitConnection();
//	client.CheckConnection();
//	client.Send();
//	client.CloseSocket();
//	client.GetUpdates();
//
//	return 0;
//}