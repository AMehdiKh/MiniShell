#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char *awk_args[] = { "/usr/bin/awk", "-F:", "{print $1}", "/etc/passwd", NULL };
    execve("/usr/bin/awk", awk_args, NULL);
    perror("execve failed");
    return 1;
}

