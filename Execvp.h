// CS 435 Evan Travers
int Execvp(const char *file, char *const argv[]) {
	int e = execvp(file, argv);
	if (e==-1) {
		perror("the read has failed");
		exit(1);
	} else {
		return e;
	}
}