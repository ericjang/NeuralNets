#include "indexing.h"

void sub2ind(int w, int h, int r, int c, int &idx) {
    idx = r*h+c;
}

void ind2sub(int w, int h, int idx, int &r, int &c) {
    c = idx%w;
    r = idx/h; // relies on int rounding
}
