//#include "essockets.h"
//
//int main() {
//
//	ADDRINFO addr;
//	addr.ai_family = AF_INET;         // бшанп яелеиярбю опнрнйнкнб,                   AF_INET     - IpV4
//	addr.ai_socktype = SOCK_STREAM;   // бшанп онксвюелнцн янйерю,                     SOCK_STREAM - онрнйнбши янйер
//	addr.ai_protocol = IPPROTO_TCP;   // бшанп хяонкэгселнцн опнрнйнкю,                IPPROTO_TCP - Tcp/IP
//	addr.ai_flags = AI_PASSIVE;       // онксвемхе хмтнплюжхх дкъ оюяяхбмни ярнпнмш    AI_PASSIVE  - оюяяхбмне онксвемхе
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