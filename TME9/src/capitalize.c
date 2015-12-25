#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <ctype.h>
#include <semaphore.h>
#include <signal.h>

#define STR_SIZE 50

sem_t *r_mutex;
sem_t *w_mutex;

char *string = NULL;

void
int_handler()
{
    munmap(string, STR_SIZE*sizeof(char));
    shm_unlink("monshm");

    sem_close(r_mutex);
    sem_close(w_mutex);

    sem_unlink("w_mut");
    sem_unlink("r_mut");

    exit(0);
}

int
main(void)
{
    sigset_t sig;
    sigfillset(&sig);
    sigdelset(&sig, SIGINT);
    sigprocmask(SIG_SETMASK, &sig, NULL);

    struct sigaction action_on_int;
    action_on_int.sa_mask = sig;
    action_on_int.sa_flags = 0;
    action_on_int.sa_handler = int_handler;
    sigaction(SIGINT, &action_on_int, 0);

    int shm_fd = -1;

    if ((shm_fd = shm_open("monshm", O_RDWR | O_CREAT, 0600)) == -1) {
            perror("shm_open");
            exit(1);
    }

    if (ftruncate(shm_fd, STR_SIZE*sizeof(char)) == -1) {
            perror("ftruncate");
            exit(1);
    }

    if ((string = mmap(NULL, STR_SIZE*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0))== MAP_FAILED) {
            perror("mmap");
            exit(1);
    }

    r_mutex = sem_open("r_mut", O_CREAT, 0644, 1);
    w_mutex = sem_open("w_mut", O_CREAT, 0644, 0);

    pid_t pid = fork();
    int i = 0;
    int count = 0;

    if (pid == 0) {
        printf("fils pid %d\n", getpid());
    } else {
        printf("pere pid %d\n", getpid());
        sleep(3);
    }

    while (1) {

        //consumer
        if (pid > 0) {
            sem_wait(w_mutex);
            for (i = 0; i < STR_SIZE; i++) {
                string[i] = toupper(string[i]);
            }

            printf("Parent says : %s\n", string);
            sem_post(r_mutex);
        }

        // producer
        if (pid == 0) {
            sem_wait(r_mutex);
            count = read(STDIN_FILENO, string, STR_SIZE);
            string[count] = '\0';
            sem_post(w_mutex);
        }

    }

    return 0;
}

