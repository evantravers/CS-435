// CS 435 Evan Travers
int Close(int fildes) {
	int e_clo = close(fildes);
	if (e_clo==-1) {
		perror("the close has failed");
		exit(1);
	}
	else {
		return e_clo;
	}
}
