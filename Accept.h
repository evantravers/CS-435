// CS 435 Evan Travers
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
	int socket_to_client = accept(sockfd, addr, addrlen);
	if (socket_to_client==-1) {
		perror("socket accept has failed");
		exit(0);
	}
	return socket_to_client;
}