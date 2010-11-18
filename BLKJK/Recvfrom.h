// CS 435 Evan Travers
ssize_t Recvfrom(int socket, void *buffer, size_t length, int flags, struct sockaddr *dest_addr, socklen_t *dest_len) {
	ssize_t e_recv = recvfrom(socket, buffer, length, flags, dest_addr, dest_len);
	if (e_recv==-1) {
		perror("the recvfrom has failed");
		exit(1);
	}
	else {
		return e_recv;
	}
}
