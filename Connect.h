// CS 435 Evan Travers
int Connect(int socket, const struct sockaddr *address, socklen_t address_len) {
	int e_con = connect(socket, address, address_len);
	if (e_con==-1) {
		perror("connect has failed");
		exit(0);
	}
	else {
		return e_con;
	}
}