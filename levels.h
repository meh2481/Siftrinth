#ifndef LEVELS_H
#define LEVELS_H

#define TILE_WIDTH		8
#define TILE_HEIGHT		8
#define NUM_MAPS		12
#define TILEMAP_HEIGHT	16
#define TILEMAP_WIDTH	16

const char tilemap[NUM_MAPS][TILEMAP_HEIGHT][TILEMAP_WIDTH] = {{
{0,20,2,0,0,2,4,2,0,2,0,0,0,0,0,0},	//Level 1
{0,4,2,0,0,0,0,0,0,0,4,0,2,0,0,2},
{0,1,0,0,0,0,0,2,0,2,0,3,1,0,20,2},
{0,0,0,0,2,0,0,1,1,0,0,2,0,1,1,0},
{0,0,0,1,6,20,0,0,0,0,0,0,0,0,0,0},
{5,1,0,0,1,1,1,0,0,0,2,0,0,1,1,5},
{0,0,0,0,0,0,0,0,7,1,1,0,0,0,0,0},
{1,0,1,6,0,0,1,1,2,0,6,0,0,3,0,1},
{0,0,0,2,0,4,0,0,22,0,3,0,0,2,0,0},
{1,0,1,0,0,7,1,0,1,0,2,0,0,1,0,1},
{0,0,2,0,0,2,0,2,4,0,3,1,2,0,0,0},
{5,3,0,0,0,0,0,1,1,1,0,0,0,4,0,0},
{0,0,0,0,0,4,0,0,0,0,2,0,0,0,0,0},
{0,0,0,3,1,1,1,23,1,3,0,0,7,1,0,0},
{0,0,1,0,0,6,0,0,0,0,0,0,2,0,0,5},
{20,4,0,0,0,0,0,2,0,2,4,0,0,0,0,20},
},{
{0,20,2,0,0,0,2,2,0,2,2,0,0,0,22,0},	//Level 2
{0,4,2,0,0,4,2,0,0,0,6,0,0,0,6,0},
{0,0,0,0,2,1,0,2,0,2,1,0,2,0,0,0},
{0,0,0,1,3,0,0,1,1,0,0,21,3,0,0,0},
{0,4,2,0,0,4,0,0,0,0,4,0,0,0,6,0},
{1,1,0,0,0,0,3,1,1,1,2,0,0,0,1,1},
{0,0,0,2,0,4,0,0,0,0,4,0,0,2,0,0},
{1,0,1,3,0,0,0,1,1,2,0,0,1,3,0,1},
{0,0,0,2,0,0,0,0,20,2,0,0,0,2,0,0},
{1,0,1,3,0,4,0,1,1,0,4,0,1,3,0,1},
{0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0},
{1,5,2,0,0,4,1,1,1,1,4,0,0,0,7,1},
{0,0,0,0,22,0,0,0,0,0,0,0,2,0,0,0},
{0,0,0,1,3,0,0,3,1,2,0,1,3,0,0,0},
{0,4,2,0,0,5,2,0,0,0,7,0,0,0,6,0},
{0,20,2,0,0,0,2,2,0,2,2,0,0,0,22,0},
},{
{0,0,0,0,0,2,0,2,0,2,0,0,0,0,0,4},	//Level 3
{0,0,0,2,0,2,0,0,0,0,0,0,6,0,0,0},
{0,0,4,2,4,2,0,2,0,2,3,5,1,21,2,0},
{0,0,20,2,0,2,0,1,1,0,2,0,0,0,2,0},
{0,1,1,0,0,2,0,0,0,0,2,0,0,1,0,0},
{0,0,0,0,0,22,4,0,0,0,1,1,1,1,1,5},
{0,0,0,0,0,3,1,1,1,1,5,2,0,0,0,0},
{1,0,1,6,0,2,4,0,0,0,0,2,0,3,0,1},
{0,0,0,3,0,2,0,0,23,0,0,2,0,2,0,0},
{1,0,1,0,0,2,0,0,1,3,1,0,0,1,0,5},
{0,0,0,0,0,2,6,0,0,0,0,0,0,0,0,0},
{0,0,7,1,1,3,0,0,4,0,0,3,21,2,0,0},
{0,0,22,0,0,2,0,0,0,0,0,2,1,0,0,4},
{1,1,1,1,0,1,1,3,1,3,1,1,1,1,1,5},
{0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,20},
{0,0,0,0,5,2,0,2,0,6,0,0,7,0,0,4},
},{
{0,0,0,4,0,0,0,2,0,2,0,0,0,0,2,20},	//Level 4
{0,0,0,0,0,0,0,2,0,0,2,0,0,4,2,0},
{0,0,3,1,1,21,2,1,1,3,0,0,0,0,0,0},
{0,0,2,0,0,0,0,0,0,2,0,5,2,0,0,5},
{0,0,2,0,0,0,0,0,0,2,0,20,2,0,0,0},
{0,0,1,1,1,1,2,0,7,1,1,1,0,3,1,5},
{4,0,0,0,0,0,2,0,2,0,0,0,0,2,0,20},
{1,1,1,2,0,0,2,0,2,0,2,0,3,1,0,1},
{0,0,0,2,0,7,0,0,22,0,2,0,2,0,0,0},
{1,0,1,2,1,2,0,0,3,1,0,0,1,1,0,1},
{0,0,0,2,0,2,0,1,2,0,0,6,0,0,0,0},
{0,0,0,6,0,2,4,4,2,1,1,1,7,1,0,0},
{0,0,4,2,0,2,0,20,6,0,0,0,0,0,0,0},
{0,0,0,2,0,7,1,1,1,1,1,7,1,1,1,21},
{0,2,0,1,0,2,0,0,0,0,0,2,0,0,0,0},
{5,0,0,0,4,2,0,2,0,2,0,0,0,0,0,4},
},{
{0,0,2,0,20,2,0,2,0,2,0,0,4,22,0,4},	//Level 5
{0,0,0,4,0,2,0,0,0,0,0,0,0,6,0,0},
{4,0,0,0,0,2,0,0,0,0,7,0,0,3,0,0},
{0,0,0,0,1,2,0,0,0,0,2,0,0,2,0,0},
{0,0,4,0,0,2,0,2,0,2,0,0,20,2,0,0},
{0,3,1,1,2,3,1,1,5,3,1,1,1,0,0,0},
{0,0,0,0,0,2,0,0,0,0,0,0,0,0,3,0},
{1,1,0,1,1,7,2,0,0,2,0,0,0,0,2,1},
{0,0,0,0,0,2,2,0,22,6,2,4,0,0,2,0},
{5,1,0,3,1,2,0,0,1,1,0,0,0,0,2,1},
{0,0,0,2,0,2,0,0,4,0,0,0,0,0,1,0},
{0,1,1,0,0,2,0,0,3,1,1,1,1,1,1,1},
{0,0,4,0,20,2,0,0,2,0,0,4,0,0,0,0},
{5,1,1,1,1,0,0,0,22,4,1,1,1,3,0,0},
{0,0,0,0,0,0,0,1,1,1,0,0,0,6,0,0},
{4,20,0,0,0,6,0,2,0,2,0,0,0,0,0,0},
},{
{20,0,0,0,0,0,0,2,0,2,0,0,0,0,0,4},	//Level 6
{0,3,6,0,0,2,0,0,0,3,0,0,3,0,20,0},
{0,5,0,0,0,1,5,3,1,0,0,0,2,0,0,0},
{0,0,0,0,2,0,0,0,0,2,0,0,0,0,0,4},
{5,1,1,1,2,0,0,0,23,1,5,1,1,1,1,1},
{0,0,0,0,0,0,6,0,2,0,0,0,0,2,1,2},
{0,0,0,0,0,0,1,1,2,0,7,1,1,0,0,2},
{1,0,1,1,7,0,0,0,2,0,0,0,0,0,0,2},
{0,0,0,0,2,0,0,2,21,2,0,4,2,0,7,0},
{1,0,1,1,0,0,0,7,7,0,0,0,2,0,2,0},
{0,0,0,0,0,1,21,0,2,0,0,1,0,0,22,0},
{0,0,4,2,0,0,0,0,2,0,0,0,2,0,1,0},
{5,1,1,1,1,1,1,1,0,0,2,0,2,0,0,0},
{0,23,2,7,0,0,3,1,1,3,0,0,1,3,0,0},
{0,2,0,2,0,0,2,0,0,0,0,4,0,0,0,0},
{0,0,0,1,0,0,0,2,0,2,0,0,0,0,0,6},
},{
{0,0,0,4,0,0,0,2,0,2,0,0,0,0,0,4},	//Level 7
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20},
{0,0,7,1,21,7,1,1,1,1,1,0,1,1,23,1},
{0,4,2,0,0,2,0,0,0,0,0,0,0,0,2,4},
{0,0,2,0,0,3,1,1,1,1,1,5,2,0,2,0},
{0,0,2,0,0,2,0,0,0,0,0,0,2,0,6,0},
{0,0,2,0,0,2,4,0,0,0,0,0,2,0,2,0},
{4,0,2,4,0,2,0,0,2,1,2,0,2,4,2,1},
{0,0,2,0,0,2,0,0,22,0,2,0,2,0,2,0},
{0,0,2,0,0,2,0,0,1,1,0,0,2,0,2,1},
{20,20,2,0,0,2,0,4,0,0,0,4,2,0,2,0},
{1,1,0,0,0,1,1,1,1,0,1,1,0,0,2,0},
{0,0,2,0,0,0,4,0,0,0,0,0,0,0,2,4},
{4,0,1,1,1,1,1,1,5,1,1,1,1,1,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,4,0,0,0,22,2,0,0,4,0,0,0},
},{
{4,0,0,0,0,0,0,2,0,6,0,0,0,0,0,4},	//Level 8
{0,0,0,0,3,0,0,0,0,2,3,1,1,1,1,2},
{5,1,5,23,2,0,0,2,0,2,2,3,1,1,2,22},
{0,0,0,2,1,0,0,2,0,2,2,2,23,2,2,2},
{0,0,0,6,3,0,0,1,5,2,2,2,0,2,2,2},
{0,0,0,1,6,0,0,0,0,2,2,1,1,0,2,2},
{0,0,0,0,1,0,0,0,0,2,1,1,1,1,0,0},
{1,0,1,2,0,0,3,0,1,1,23,7,2,3,0,1},
{0,0,0,3,1,1,2,0,22,0,2,2,2,2,0,0},
{5,0,1,0,0,0,1,0,1,1,0,2,2,1,0,1},
{0,0,4,0,0,5,2,0,0,0,4,2,6,0,0,0},
{0,0,0,0,0,0,3,2,0,0,0,2,1,1,1,2},
{0,2,0,2,0,1,0,23,1,3,5,2,0,2,0,2},
{0,1,3,0,0,0,0,6,0,0,0,2,0,6,0,0},
{0,0,2,0,6,0,2,1,21,2,0,6,0,1,2,0},
{0,4,2,0,1,7,0,0,0,2,0,0,0,0,6,4},
},{
{0,0,0,0,0,20,4,2,0,6,20,0,0,0,0,0},	//Level 9
{0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0},
{0,0,0,1,2,0,0,2,0,2,0,0,3,0,0,0},
{1,5,2,0,0,0,4,2,0,6,0,0,0,0,3,21},
{0,0,2,0,0,1,1,3,1,3,1,0,1,2,2,4},
{0,0,1,1,0,0,0,2,0,2,0,0,0,0,2,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0},
{1,0,1,1,3,1,0,0,0,0,1,1,3,1,0,0},
{0,0,0,0,2,0,0,0,20,0,0,0,2,0,0,0},
{1,0,1,1,1,1,0,0,0,0,1,3,0,0,5,1},
{0,4,0,0,0,0,2,0,0,0,0,6,0,0,0,0},
{0,0,0,1,1,1,1,2,0,3,0,1,6,0,0,0},
{0,0,4,0,0,0,0,2,0,2,0,0,2,0,0,0},
{1,1,1,1,2,0,1,22,20,2,0,0,2,0,0,0},
{0,0,2,0,0,0,4,2,0,6,0,0,1,1,1,0},
{4,20,6,0,0,4,3,4,4,1,6,0,0,4,0,0},
},{
{4,0,0,2,0,0,0,2,0,2,0,0,0,0,0,0},	//Level 10
{0,0,0,2,0,2,0,0,0,0,0,0,2,0,0,0},
{0,2,0,2,0,1,1,2,0,2,0,5,2,0,1,5},
{0,2,4,2,0,0,0,1,3,0,0,0,2,0,0,0},
{0,2,1,2,3,1,5,2,1,1,1,3,5,1,2,0},
{0,2,0,6,2,0,0,2,4,0,20,2,0,0,2,0},
{20,2,0,1,0,3,1,1,1,5,3,0,0,0,2,20},
{1,0,0,0,0,2,0,0,0,0,0,0,7,2,1,1},
{0,0,2,0,0,0,0,0,21,2,0,0,2,2,0,0},
{5,1,1,1,1,1,1,1,1,2,3,1,0,1,0,1},
{1,0,0,0,0,2,0,0,0,2,6,2,0,2,0,0},
{0,0,0,23,1,1,1,1,2,1,1,0,0,3,1,21},
{0,2,0,2,0,2,0,4,6,0,0,0,0,2,4,0},
{0,1,1,2,0,1,1,3,1,7,1,7,1,2,1,1},
{0,0,0,1,1,2,0,0,0,0,0,0,0,0,4,0},
{0,7,0,0,20,2,0,2,0,2,4,2,4,2,0,0},
},{
{0,0,0,2,0,0,0,2,0,2,0,0,0,0,0,0},	//Level 11
{0,0,0,2,0,6,0,2,0,2,4,3,1,1,1,2},
{0,1,2,1,0,22,0,2,0,22,0,2,3,1,1,0},
{0,4,2,0,0,3,1,0,0,1,1,0,1,1,1,0},
{0,1,6,0,0,2,0,2,0,2,0,0,7,1,1,1},
{0,0,1,1,2,2,0,3,1,2,0,0,0,0,0,4},
{0,0,0,0,22,2,0,2,0,3,1,1,2,0,0,0},
{1,0,1,2,1,0,0,2,0,0,0,0,0,3,0,1},
{0,0,0,2,0,7,1,0,22,2,0,0,0,2,0,0},
{1,0,1,0,0,0,0,0,5,7,1,21,2,1,0,1},
{1,1,1,1,1,1,1,1,1,0,0,0,2,7,1,21},
{0,7,1,1,0,0,0,4,0,0,0,0,2,2,0,0},
{0,2,0,3,1,5,1,1,1,1,1,1,1,0,3,0},
{0,0,0,0,0,0,0,3,1,2,0,0,0,0,6,0},
{4,0,0,0,7,1,2,0,0,0,0,7,2,0,1,0},
{21,1,0,0,2,0,0,2,0,2,0,2,0,4,0,0},
},{
{20,0,0,2,0,0,0,2,0,2,0,0,4,22,0,0},	//Level 12
{5,1,0,2,0,0,2,0,0,0,2,0,0,1,1,0},
{0,0,23,6,0,0,1,1,1,1,0,0,0,0,0,0},
{0,0,2,5,1,1,1,1,1,1,7,1,21,3,1,0},
{0,2,2,0,0,0,0,0,0,4,2,0,0,2,0,4},
{4,2,1,1,1,0,0,3,1,1,0,0,0,2,3,5},
{1,0,0,0,0,1,1,0,0,0,0,0,0,2,2,0},
{1,0,1,1,2,0,7,0,3,1,2,1,5,0,2,1},
{0,0,0,0,2,0,2,0,22,0,2,0,0,0,2,0},
{1,0,1,1,0,1,0,0,7,0,1,1,1,1,2,1},
{1,5,3,1,1,1,1,1,0,0,0,4,0,0,0,0},
{0,1,0,0,2,0,4,0,0,0,0,0,0,0,0,0},
{0,0,0,0,2,0,3,1,1,1,2,0,0,0,7,21},
{0,4,7,1,0,0,2,0,0,0,2,0,0,0,2,0},
{0,0,2,0,0,0,0,0,0,0,0,5,2,0,2,0},
{20,0,2,0,6,0,0,2,0,2,0,0,2,0,0,0},
}};











#endif