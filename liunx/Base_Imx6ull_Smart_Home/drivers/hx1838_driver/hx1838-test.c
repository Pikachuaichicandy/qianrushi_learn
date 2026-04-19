#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

int main()
{
    int fd = open("/dev/hx1838", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    uint8_t code;
    while (1) {
        int ret = read(fd, &code, sizeof(code));
        if (ret == sizeof(code)) {
            printf("IR Code: 0x%02X\n", code);
        } 
    }

    close(fd);
    return 0;
}
