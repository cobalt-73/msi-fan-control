#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>

typedef struct update {
    uint8_t offs;
    uint8_t val;
} update_t;

int main(void)
{
    static const update_t ups[] = {
        /* cpu fan speeds, using default frequency ranges */
        { 0x72, 0x00 },
        { 0x73, 0x24 }, /* 28 */
        { 0x74, 0x48 }, /* 4b */
        { 0x75, 0x64 },
        { 0x76, 0x96 },
        { 0x77, 0x96 },

        /* gpu fan speeds, using default frequency ranges */
        { 0x8a, 0x00 },
        { 0x8b, 0x32 },
        { 0x8c, 0x3c },
        { 0x8d, 0x50 },
        { 0x8e, 0x64 },
        { 0x8f, 0x96 },

        /* extreme performance */
        { 0xd2, 0xc4 },
        { 0xd4, 0x4d },
        { 0x91, 0x5f },

        /* extreme cooling? 0x02 is off, 0x82 is on */
        { 0x98, 0x02 },
    };

    int fd = open("/sys/kernel/debug/ec/ec0/io", O_RDWR);
    if(fd<0) {
        fprintf(stderr, "failed to open EC: %d %d\n", fd, errno);
        return 1;
    }

    for(int i=0; i<(sizeof(ups)/sizeof(*ups)); ++i) {
        off_t lret = lseek(fd, ups[i].offs, SEEK_SET);
        if(lret!=ups[i].offs) {
            fprintf(stderr, "failed to seek to EC offset 0x%x: %ld %d\n", ups[i].offs, lret, errno);
            return 1;
        }

        int ret = write(fd, &ups[i].val, sizeof(ups[i].val));
        if(ret!=sizeof(ups[i].val)) {
            fprintf(stderr, "failed to write 0x%x to EC at offset 0x%x: %d %d\n", ups[i].val, ups[i].offs, ret, errno);
            return 1;
        }
    }

    return 0;
}

