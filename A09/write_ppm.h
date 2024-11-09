#ifndef WRITE_PPM_H_
#define WRITE_PPM_H_

#include "read_ppm.h"

int write_ppm(const char* filename, struct ppm_pixel* pxs, int w, int h);
int write_ppm_2d(const char* filename, struct ppm_pixel** pxs, int w, int h);

#endif // WRITE_PPM_H_
