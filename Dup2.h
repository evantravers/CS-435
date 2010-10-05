// CS 435 Evan Travers
int Dup2(int fildes, int fildes2) {
	int e = dup2(fildes, fildes2);
	if (e==-1) {
		perror("the read has failed");
		exit(1);
	} else {
		return e;
	}
}