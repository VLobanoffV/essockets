#include "essockets.h"

int main() {

	ADDRINFO addr;
	addr.ai_family = AF_INET;         
	addr.ai_socktype = SOCK_STREAM;   
	addr.ai_protocol = IPPROTO_TCP;   
	addr.ai_flags = AI_PASSIVE;       

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