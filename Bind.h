// CS 435 Evan Travers
int Bind(int the_socket, struct sockaddr *address, socklen_t address_len) {
	int bind_num = bind(the_socket, address, address_len);
	if (bind_num==-1) {
		perror("The bind failed");
		exit(1);
	}
	else {
		return bind_num;
	}
}