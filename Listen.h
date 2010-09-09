int Listen(int socket, int backlog) {
	int e_lis = listen(socket, backlog);
	if (e_lis==-1) {
		perror("the listen has failed");
		exit(-1);
	}
}