// CS 435 Evan Travers
int Listen(int socket, int backlog) {
	int e_lis = listen(socket, backlog);
	if (e_lis==-1) {
		perror("the listen has failed");
		exit(-1);
	}
	else {
		return e_lis;
	}
}