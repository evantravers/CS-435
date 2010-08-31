int Socket() {
	int server_socket = socket(AF_INET,SOCK_STREAM,0);
	if (server_socket==-1) {
		perror("The socket has failed");
		exit(-1);
	}
	else {
		return server_socket;
	}
}