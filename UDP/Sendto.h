// CS 435 Evan Travers
ssize_t Sendto(int socket, const void *buffer, size_t length, int flags, const struct sockaddr *dest_addr, socklen_t dest_len) {
	ssize_t e_send = sendto(socket, buffer, length, flags, dest_addr, dest_len);
	if (e_send==-1) {
		perror("the sendto has failed");
		exit(1);
	}
	else {
		return e_send;
	}
}