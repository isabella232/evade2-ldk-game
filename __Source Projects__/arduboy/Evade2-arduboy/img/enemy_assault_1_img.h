
#ifndef ENEMY_ASSAULT_1_IMG_H
#define ENEMY_ASSAULT_1_IMG_H
// SVG Graphic source: svg_docs/output_from_ai/assault-1.svg
// Number bytes 59
const PROGMEM struct enemy_assault_1_img {
	uint8_t w;
	uint8_t h;
	uint8_t r;
	int8_t data[14*4];
} enemy_assault_1_img = {
	.w = 128,    // Width (128 px)
	.h = 46,    // Height (54 px)
	.r = 8,    // Number of rows of coords (14)
	.data = {
//  x0,     y0,    x1,    y1
    -9,    -23,    37,    23,
    -37,    23,    9,    -23,
    -9,    -23,    -27,    -5,
    -27,    -5,    -9,    14,
    9,    14,    27,    -5,
    27,    -5,    9,    -23,
    -37,    23,    -64,    -5,
    37,    23,    64,    -5,
},
};
#endif

