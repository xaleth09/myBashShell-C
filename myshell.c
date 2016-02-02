/* Includes */
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <sys/wait.h>   /* Wait for Process Termination */
#include <stdlib.h>     /* General Utilities */
#include <string.h>
#include <signal.h>

#define MAX_CHILD 5
#define MAX_CMD 10


int myparse(char *cmat[MAX_CHILD][MAX_CMD], char* cmd){
	
	int cnum;
	int argindx;
	char *copy;	
	char *copy2 = malloc(32*sizeof(char));
	char *child[MAX_CHILD];
	copy = strdup(cmd);

	child[0] = strtok(copy, ";\n");
	
	int i = 1;
	while( (child[i++] = strtok(NULL, ";\n")) && i < MAX_CHILD );


	cnum = 0;
	while(child[cnum] != NULL){
		copy = strdup(child[cnum]);
		cmat[cnum][0] = strdup(strtok(copy, " \n"));
		argindx = 1;
		while( (copy2 = strtok(NULL, " \n")) ){
			cmat[cnum][argindx++] = strdup(copy2);
		}
			cmat[cnum][argindx] = NULL;
		cnum++;
	}



	free(copy);
	free(copy2);
	return i-1;
}



int spawn2(char *cmat[MAX_CHILD]){

		pid_t childpid;
		if ((childpid = fork()) > 0) { //parent	
			return childpid;		
		}else if (childpid == 0) { //child
			execvp(cmat[0],cmat);
		}else{
			perror("fork");
			abort();
		}
}

void myshell(){

	int max_in = 126;
	int chld_itr = 0;
	int numchild = 0;
	char *cmat[MAX_CHILD][MAX_CMD];
	char *input = malloc(max_in*sizeof(char));
	pid_t chldpid;


	while(1){
		numchild = 0;	
		printf("myshell> ");

		fgets(input, max_in, stdin);

		numchild = myparse(cmat, input);
	
		chld_itr = 0;
		for(chld_itr = 0; chld_itr < numchild; chld_itr++){
			chldpid = spawn2(cmat[chld_itr]);
			waitpid(chldpid, NULL,0);
		}

		
	}
	
	free(input);
}

void myshellonce(){

	int max_in = 126;
	int chld_itr = 0;
	int numchild = 0;
	char *cmat[MAX_CHILD][MAX_CMD];
	char *input = malloc(max_in*sizeof(char));
	pid_t chldpid;



	numchild = 0;	
	printf("myshell> ");

	fgets(input, max_in, stdin);

	numchild = myparse(cmat, input);

	chld_itr = 0;
	for(chld_itr = 0; chld_itr < numchild; chld_itr++){
		chldpid = spawn2(cmat[chld_itr]);
		waitpid(chldpid, NULL,0);
	}

		

	
	free(input);
}
 
int main()
{
	
	if(isatty(0) == 1 ){
		myshell();
	}else{
		myshellonce();
	}

 return 0;  
}
