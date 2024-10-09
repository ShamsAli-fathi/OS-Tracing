#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int num;
    FILE *file_ptr;

    file_ptr = fopen("output.txt", "w");
    if (file_ptr == NULL)
    {
        exit(1);
    }
    fprintf(file_ptr, "Hello World!\n");
    fclose(file_ptr);
    
    pid_t pid = fork();

    if (pid < 0) {
        return 1;
    } else if (pid == 0) {
        printf("This is the child process");
        printf(" (PID: %d)\n", getpid());
    } else {
        printf("This is the Parent process");
        printf(" (PID: %d, Child PID: %d)\n", getpid(), pid);
    }
    
    return 0;
}