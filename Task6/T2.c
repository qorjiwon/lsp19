#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void
sigCHLDHandler(int signo){
        pid_t pid;
        pid = getpid();
        printf("Received a SIGCHLD \n");
	printf("%d\n", pid); // Print pid. Maybe Parnt's
	exit(1); // 종료
}

main()
{
        pid_t   pid;
        int             status;

        signal(SIGCHLD, sigCHLDHandler);

        if ((pid = fork()) < 0)  { // error
                perror("fork");
                exit(1);
        }
        else if (pid == 0)  {
                printf("I'm a child\n");
        	printf("My pid is %d\n",getpid()); // Print chile process pid
	}
        else  { // Parnt Process
                while(1) ; // loop
        }
}
