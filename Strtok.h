// CS 435 Evan Travers
char *Strtok(char *restrict str, const char *restrict sep) {
	char e = strtok(str, sep);
	if (e==NULL) {
		perror("the read has failed");
		exit(1);
	} else {
		return e;
	}
}
