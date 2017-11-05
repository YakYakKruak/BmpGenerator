#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} __attribute__((packed)) bmp_header;

int main() {
    double (*a)[3] = malloc(sizeof(double)*3*8);
    a[0][0] = 0;
    a[0][1] = 0;
    a[0][2] = 0;
    a[1][0] = 1;
    a[1][1] = 0;
    a[1][2] = 0;
    a[2][0] = 0;
    a[2][1] = 1;
    a[2][2] = 0;
    a[3][0] = 0;
    a[3][1] = 0;
    a[3][2] = 1;
    a[4][0] = 1;
    a[4][1] = 1;
    a[4][2] = 0;
    a[5][0] = 1;
    a[5][1] = 0;
    a[5][2] = 1;
    a[6][0] = 0;
    a[6][1] = 1;
    a[6][2] = 1;
    a[7][0] = 1;
    a[7][1] = 1;
    a[7][2] = 1;

    double (*c)[2] = malloc(sizeof(double)*2*8);

    for (int i = 0; i < 8 ; ++i) {
        c[i][0] = sqrt(3)*a[i][0] - sqrt(3)*a[i][2];
        c[i][1] = a[i][0] + 2*a[i][1] + a[i][2];
        printf("x_%d = %f, y_%d = %f\n",i,c[i][0],i,c[i][1]);

    }

    typedef struct pixel_t { uint8_t b,g,r; } pixel_t;

    bmp_header header;
    header.bfType = 19778;
    header.biWidth =  700;
    header.biHeight = -700;
    header.bOffBits = 54;
    header.biBitCount = 24;
    header.biSizeImage = abs(header.biHeight) * header.biWidth * header.biBitCount / 8;
    header.bfileSize = header.bOffBits + header.biSizeImage;
    header.bfReserved = 0;
    header.biCompression = 0;
    header.biSize = 40;
    header.biPlanes = 1;
    header.biClrUsed = 0;
    header.biClrImportant = 0;
    header.biXPelsPerMeter = 2834;
    header.biYPelsPerMeter = 2834;

    FILE *img = fopen("cube.bmp","wb");
    fwrite(&header, sizeof(bmp_header),1,img);

    pixel_t image[700][700];

    for (int j = 0; j < abs(header.biHeight); ++j) {
        for (int i = 0; i < header.biWidth; ++i) {
            pixel_t pixel;
            pixel.b = 255;
            pixel.r = 255;
            pixel.g = 255;
            image[i][j] = pixel;
        }
    }

    for (int k = 0; k < 8; ++k) {
        int x = 100*c[k][0]+250;
        int y = 100*c[k][1]+250;
        pixel_t pixel;
        pixel.b = 0;
        pixel.r = 0;
        pixel.g = 0;
        image[y-1][x-1] = pixel;
        image[y-1][x] = pixel;
        image[y-1][x+1] = pixel;
        image[y][x-1] = pixel;
        image[y][x] = pixel;
        image[y][x+1] = pixel;
        image[y+1][x-1] = pixel;
        image[y+1][x] = pixel;
        image[y+1][x+1] = pixel;
    }


    for (int l = 0; l < 8; ++l) {
        for (int i = 0; i < 8; ++i) {
            if (i == l) {
                continue;
            }
            pixel_t pixel;
            pixel.b = 0;
            pixel.r = 0;
            pixel.g = 0;
            int x_0 = 100 * c[l][0] + 250;
            int y_0 = 100 * c[l][1] + 250;
            int x_1 = 100 * c[i][0] + 250;
            int y_1 = 100 * c[i][1] + 250;

            if (x_0 != x_1) {
                double k = (double)abs(y_0-y_1)/(double)abs(x_0 - x_1);
                

                int x_start;
                int x_end;
                int y_start;

                if(x_0 > x_1) {
                    x_start = x_1;
                    x_end = x_0;
                    y_start = y_1;
                    if(y_1<y_0) {
                        for (int j = 0; j < x_end-x_start; ++j) {
                            int y = k*j + y_start;
                            image[y][x_start+j] = pixel;
                        }
                    } else {
                        for (int j = 0; j < x_end-x_start; ++j) {
                            int y = -k*j + y_start;
                            image[y][x_start+j] = pixel;
                        }
                    }
                } else {
                    x_start = x_0;
                    y_start = y_0;
                    x_end = x_1;
                    if(y_1>y_0) {
                        for (int j = 0; j < x_end - x_start; ++j) {
                            int y = k*j + y_start;
                            image[y][x_start+j] = pixel;
                        }
                    } else {
                        for (int j = 0; j < x_end - x_start; ++j) {
                            int y = -k*j + y_start;
                            image[y][x_start+j] = pixel;
                        }
                    }
                }
            } else {
                int y_start;
                int y_end;
                if (y_0 > y_1) {
                    y_start = y_1;
                    y_end = y_0;
                } else {
                    y_start = y_0;
                    y_end = y_1;
                }

                for (int j = y_start; j < y_end; ++j) {
                    image[j][x_0] = pixel;
                }
            }
        }
    }
        fwrite(image, sizeof(image), 1, img);

        return 0;

    }