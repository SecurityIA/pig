#include "mkrnd.h"
#include <stdlib.h>

static unsigned int mk_rnd_ipv4(const int msb_floor);

unsigned char mk_rnd_u1() {
    return (rand() & 0x1);
}

unsigned char mk_rnd_u3() {
    return (rand() & 0x7);
}

unsigned char mk_rnd_u4() {
    return (rand() & 0xf);
}

unsigned char mk_rnd_u6() {
    return (rand() & 0x3f);
}

unsigned char mk_rnd_u8() {
    return (rand() & 0xff);
}

unsigned short mk_rnd_u13() {
    return (rand() & 0x1fff);
}

unsigned short mk_rnd_u16() {
    return (rand() & 0xffff);
}

unsigned int mk_rnd_u32() {
    return (rand() & 0xffffffff);
}

static unsigned int mk_rnd_ipv4(const int msb_floor) {
    unsigned char b0 = msb_floor + (rand() % 2);
    unsigned char b1 = 1 + (rand() % 254);
    unsigned char b2 = 1 + (rand() % 254);
    unsigned char b3 = 1 + (rand() % 254);
    return ((unsigned int) b0 << 24) |
           ((unsigned int) b1 << 16) |
           ((unsigned int) b2 <<  8) |
           ((unsigned int) b3);
}

unsigned int mk_rnd_european_ipv4() {
    return mk_rnd_ipv4(194);
}

unsigned int mk_rnd_north_american_ipv4() {
    return mk_rnd_ipv4(198);
}

unsigned int mk_rnd_south_american_ipv4() {
    return mk_rnd_ipv4(200);
}

unsigned int mk_rnd_asian_ipv4() {
    return mk_rnd_ipv4(202);
}

unsigned int mk_rnd_ipv4_by_mask(const pig_target_addr_ctx *mask) {
    unsigned int retval = 0;
    unsigned int maskval = 0;
    if (mask == NULL || mask->addr == NULL) {
        return 0;
    }

    switch (mask->type) {

        case kWild:
            retval = (rand() % 0xffffffff) & *((unsigned int *)mask->addr);
            break;

        case kAddr:
            retval = *(unsigned int *)mask->addr;
            break;

        case kCidr:
            maskval = 0xffffffff;
            maskval = maskval << mask->cidr_range;
            maskval |= *(unsigned int *)mask->addr;
            retval = (rand() % 0xffffffff) | maskval;
            while ((retval | *(unsigned int *)mask->addr) != retval) {
                retval = (rand() % 0xffffffff) | maskval;
            }
            break;
    }

    return retval;
}
