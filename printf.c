#include "types.h"
#include "stat.h"
#include "user.h"

static void putc(int fd, char c)
{
    write(fd, &c, 1);
}

static void printint(int fd, int xx, int base, int sgn, int pad_len, char pad_type)
{
    static char digits[] = "0123456789abcdef";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if(sgn && xx < 0){
        neg = 1;
        x = -xx;
    } else {
        x = xx;
    }

    i = 0;
    do{
        buf[i++] = digits[x % base];
    }while((x /= base) != 0);
    if(neg)
        buf[i++] = '-';

    while(--pad_len >= i)
        putc(fd, pad_type);
    while(--i >= 0)
        putc(fd, buf[i]);
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void printf(int fd, const char *fmt, ...)
{
    char *s;
    int c, i, state;
    uint *ap;

    state = 0;
    ap = (uint*)(void*)&fmt + 1;
    i = 0;
    while (fmt[i]) {
        c = fmt[i] & 0xff;
        if(state == 0){
            if(c == '%'){
                state = '%';
            } else {
                putc(fd, c);
            }
        } else if(state == '%'){
            //checking for padding
            int padding_len = 0;
            char type = ' ';
            if (c == '0')
                type = '0';
            if ('0' <= c && c <= '9') {
                padding_len = atoi(&fmt[i]);
            }
            while ('0' <= c && c <= '9') {
                i++;
                c = fmt[i] & 0xff;
            }

            if(c == 'd'){
                printint(fd, *ap, 10, 1, padding_len, type);
                ap++;
            } else if(c == 'x' || c == 'p'){
                printint(fd, *ap, 16, 0, padding_len, type);
                ap++;
            } else if(c == 's'){
                s = (char*)*ap;
                ap++;
                if(s == 0)
                    s = "(null)";
                while (padding_len > strlen(s)) {
                    putc(fd, ' ');
                    padding_len--;
                }
                while(*s != 0){
                    putc(fd, *s);
                    s++;
                }
            } else if(c == 'c'){
                while (padding_len > 1) {
                    putc(fd, ' ');
                    padding_len--;
                }
                putc(fd, *ap);
                ap++;
            } else if(c == '%'){
                putc(fd, c);
            } else {
                // Unknown % sequence.  Print it to draw attention.
                putc(fd, '%');
                putc(fd, c);
            }
            state = 0;
        }
        i++;
    }
}
