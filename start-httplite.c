#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *servers[][4] = {
        {"httpd", NULL},
        {"apache2", NULL},
        {"python3", "-m", "http.server", NULL},
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
