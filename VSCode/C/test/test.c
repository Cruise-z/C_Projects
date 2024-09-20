#include <stdio.h>
#include <direct.h>  // For _getcwd
#include "../.lib/HashLib/Hash.h"

int main() {
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }

    return 0;
}
