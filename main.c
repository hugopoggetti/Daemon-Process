#include <complex.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define INFO "[ INFO ]"
#define LOG "[ LOG  ]"
#define ERROR "[ ERROR ]"
#define PORT 8000

static FILE *log_file = NULL;
static int sock_fd = -1;

void write_logs(const char *buffer, const char *msg, const char *mode)
{
    if (log_file != NULL) {
        int log_fd = fileno(log_file);
        time_t now = time(NULL);
        char *now_string = ctime(&now);
        now_string[strlen(now_string) - 1] = '\0'; // Ends with a \n by default
        fputs(now_string, log_file);
        fputs("  ", log_file);        
        fputs(mode, log_file);
        fputs("  ", log_file);
        fputs(buffer, log_file);  
        if (msg != NULL)
            fputs(msg, log_file);
        fputc('\n', log_file);
    } 
}

void clean_all(void)
{
    write_logs("Cleaning all", NULL, INFO);
    if (log_file != NULL)
        fclose(log_file);
    if (sock_fd != -1)
        close(sock_fd);
    exit(EXIT_SUCCESS);
}

void tcp_server(void)
{
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        write_logs("socket creation failed", NULL, ERROR);
        clean_all();
    }
    // Set the SO_REUSEADDR option to avoid the "Address already in use" error
    int option = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof option);
    struct sockaddr_in addr = {
        addr.sin_family = AF_INET,
        addr.sin_port = htons(PORT),
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK),
    };
    if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        write_logs("Failed to bind socket", NULL, ERROR);
        clean_all();
    }
    if (listen(sock_fd, 31) < 0) {
        write_logs("Failed to listen socket", NULL, ERROR);
        clean_all();
    }
    write_logs("Waiting for client connection", NULL, INFO);

    bool is_running = true;

    while (is_running) {
        int client_fd = accept(sock_fd, NULL, NULL);
        if (client_fd == -1)
            write_logs("Failed to accpet new client", NULL, ERROR);
        while (1) {
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            size_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
            if (bytes_read == -1)
                write_logs("Read client error", NULL, ERROR);
            if (bytes_read == 0) {
                write_logs("Client disconected", NULL, INFO);
                clean_all();
                break;
            }
            buffer[bytes_read] = '\0';
            if (buffer[bytes_read - 1] == '\n')
                buffer[bytes_read - 1] = '\0';
            write_logs("Read: ", buffer, LOG);
            if (strncmp(buffer, "quit", 4) == 0) {
                write_logs("Quit after 'quit'", NULL, INFO);
                is_running = false;
                close(client_fd);
                break;
            }
        }
    }
    
}

int main(void)
{
    pid_t child_pid = fork();
    if (child_pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (child_pid > 0)
        exit(EXIT_SUCCESS);
    if (setsid() == -1) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }
    child_pid = fork();
    if (child_pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (child_pid > 0)
        exit(EXIT_SUCCESS);
    
    const char *log_file_path = "deamon.log";
    log_file = fopen(log_file_path, "a");
    if (log_file == NULL)
        perror("failder to open log file");
    
    write_logs("Started", NULL, INFO);
    tcp_server();
    clean_all();
    return 0;
}
