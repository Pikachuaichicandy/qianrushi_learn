#include <stdio.h>
#include <errno.h>
int main() {
   fopen("/opt","a+");
    printf("Error number: %d\n", errno);

    // Print the corresponding error message
    perror("Error occurred");

    return 0;
}