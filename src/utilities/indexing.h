#ifndef INDEXING_H
#define INDEXING_H

// 2D sub2ind and ind2sub functions, row-major
void sub2ind(int w, int h, int r, int c, int &idx);
void ind2sub(int w, int h, int idx, int &r, int &c);

#endif // INDEXING_H
