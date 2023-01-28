#include "types.h"
#include "user.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf(2, "hd usage: hd <file>\n"); //TODO: what is this 0?
        exit();
    }

    int fd;
    if((fd = open(argv[1], 0)) < 0) {
        printf(2, "hd cannot open %s\n", argv[1]);
        exit();
    }

    int n;
    char buf[16];
    int address = 0;

    while ((n = read(fd, buf, 16)) != 0) {
        //address
        printf(1, "%08x  ", address);
        
        //bytes in hex
        for (int i = 0; i < 16; i++) {
            if (i < n)
                printf(1, "%02x ", buf[i]);
            else
                printf(1, "   ");
            if (i == 7)
                printf(1, " ");
        }

        //bytes as characters or .
        printf(1, " |");
        for (int i = 0; i < 16; i++) {
            if (i >= n)
                printf(1, " ");
            else if (buf[i] != 0xa)
                printf(1, "%c", buf[i]);
            else
                printf(1, ".");
        }
        printf(1, "|\n");

        address += 16;
    }

    //hd also prints address of next line
    printf(1, "%08x\n", address);
    close(fd);

    exit();
}
