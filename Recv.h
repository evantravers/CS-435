// CS 435 Evan Travers
ssize_t Recv(int socket, void *buffer, size_t length, int flags) {
	ssize_t e_rec = recv(socket, buffer, length, flags);
	if (e_rec==-1) {
		perror("the recv has failed");
		exit(0);
	}
	else {
		return e_rec;
	}
}