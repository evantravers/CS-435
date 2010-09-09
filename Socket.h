int Socket(int domain, int type, int protocol) {
	int server_socket = socket(domain,type,protocol);
	if (server_socket==-1) {
		perror("The socket has failed");
		exit(-1);
	}
	else {
		return server_socket;
	}
}