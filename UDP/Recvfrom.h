// CS 435 Evan Travers
ssize_t Recvfrom(int socket, void *restrict buffer, size_t length, int flags, struct sockaddr *restrict address, socklen_t *restrict address_len) {
	ssize_t e_recv = recvfrom(socket, buffer, length, flags, address, address_len);
	if (e_recv==-1) {
		perror("the recvfrom has failed");
		exit(1);
	}
	else {
		return e_recv;
	}
}