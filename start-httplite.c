#include <unistd.h>
int main() {
  char *args[] = {"/usr/local/bin/httpd", NULL};
  execv(args[0], args);
  return 1;
}
