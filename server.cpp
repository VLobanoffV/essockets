#include "essockets.h"

int main() {

	ADDRINFO addr;
	addr.ai_family = AF_INET;         // бшанп яелеиярбю опнрнйнкнб,                   AF_INET     - IpV4
	addr.ai_socktype = SOCK_STREAM;   // бшанп онксвюелнцн янйерю,                     SOCK_STREAM - онрнйнбши янйер
	addr.ai_protocol = IPPROTO_TCP;   // бшанп хяонкэгселнцн опнрнйнкю,                IPPROTO_TCP - Tcp/IP
	addr.ai_flags = AI_PASSIVE;       // онксвемхе хмтнплюжхх дкъ оюяяхбмни ярнпнмш    AI_PASSIVE  - оюяяхбмне онксвемхе

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