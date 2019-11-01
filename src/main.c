#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>

int main() {
    char x[9], y[9];
    char buffer[2] = {'\0'} ;
    int data_processed;
    int x_pipe[2];
    int y_pipe[2];
    int result[2];

    if (pipe(x_pipe) || pipe(y_pipe) || pipe(result)) {
        printf("Error pipe\n");
        exit(2);
    }
    
    pid_t proc = fork();
    if (proc < 0) {
        printf("Error fork\n");
        exit(1);
    }

    if (proc == 0) {

        close(y_pipe[1]);
        close(x_pipe[1]);
        
        data_processed = read(x_pipe[0], x, sizeof(x));
        data_processed = read(y_pipe[0], y, sizeof(y));
        
        close(y_pipe[0]);
        close(x_pipe[0]);
        
        if (x[0] != '-' && y[0] != '-'){
            buffer[0] = '1';
        }
        if (x[0] == '-' && y[0] != '-'){
            buffer[0] = '2';
        }
        if (x[0] == '-' && y[0] == '-'){
            buffer[0] = '3';
        }
        if (x[0] != '-' && y[0] == '-'){
            buffer[0] = '4';
        }

        close(result[0]);
        data_processed = write(result[1], buffer, sizeof(buffer));
        close(result[1]);
        exit(EXIT_SUCCESS);
    }
    else if (proc > 0) {
        read(0, x, sizeof(x));
        read(0, y, sizeof(y));
        
        close(y_pipe[0]);
        close(x_pipe[0]);
        
        data_processed = write(x_pipe[1], x, sizeof(x));
        data_processed = write(y_pipe[1], y, sizeof(y));

        close(y_pipe[1]);
        close(x_pipe[1]);

        int how;
        wait(&how);
        close(result[1]);
        data_processed = read(result[0], buffer, sizeof(buffer));
        write(1, buffer, sizeof(buffer));
        close(result[0]);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_SUCCESS);
}
