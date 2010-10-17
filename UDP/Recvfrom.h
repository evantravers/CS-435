// CS 435 Evan Travers
ssize_t Recvfrom() {}
	ssize_t e_recv = recvfrom(socket, buffer, length, flags, address, address_len);
	if (e_recv==-1) {
		perror("the recvfrom has failed");
		exit(1);
	}
	else {
		return e_recv;
	}
}
