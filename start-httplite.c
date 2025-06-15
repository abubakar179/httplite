#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int port_is_free(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return 0;

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    int result = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    close(sock);

    return (result == 0);
}

int main() {
    int ports_to_try[] = {8000, 8080, 8081, 8082};
    char port_str[6];

    // Try python3 with first free port
    for (int i = 0; i < sizeof(ports_to_try)/sizeof(ports_to_try[0]); i++) {
        int port = ports_to_try[i];
        if (port_is_free(port)) {
            snprintf(port_str, sizeof(port_str), "%d", port);
            const char *args[] = {"python3", "-m", "http.server", port_str, NULL};

            // Check python3 exists
            if (system("which python3 > /dev/null 2>&1") == 0) {
                execvp("python3", (char * const *)args);
                perror("execvp failed");
                return 1;
            }
            break; // python3 not found, break to fallback below
        }
    }

    // Fallback servers without port checking
    const char *servers[][4] = {
        {"httpd", NULL},
        {"apache2", NULL},
        {"busybox", "httpd", NULL},
        {"nginx", NULL},
    };

    for (int i = 0; i < sizeof(servers)/sizeof(servers[0]); i++) {
        char *cmd = (char *)servers[i][0];
        char whichCmd[256];
        snprintf(whichCmd, sizeof(whichCmd), "which %s > /dev/null 2>&1", cmd);
        int ret = system(whichCmd);
        if (ret == 0) {
            execvp(cmd, (char * const *)servers[i]);
            perror("execvp failed");
            return 1;
        }
    }

    fprintf(stderr,
        "httplite: No HTTP server found. Looks like you’re running the world’s fastest invisible server!\n"
        "Or maybe just a fancy launcher for nothing.\n"
        "Try installing apache2, httpd, nginx, or just Python for a real server.\n"
        "Until then, enjoy the silence...\n"
    );
    return 1;
}
