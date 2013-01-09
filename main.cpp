/*
 * Siftrinth - Marble labyrinth program
 */

#include "CubeBoard.h"
#define NUM_CUBES	3

static AssetSlot MainSlot = AssetSlot::allocate()
    .bootstrap(GameAssets);

static Metadata M = Metadata()
    .title("Mark H's Siftrinth")
    .package("com.markh.siftrinth", "1.0")
    .icon(Icon)
    .cubeRange(NUM_CUBES);

static CubeBoard boards[NUM_CUBES];
static int g_iCurColor;
static int g_iCurMode;
static int g_iScore;

static void onNeighborAdd(void* ctxt, unsigned cube0, unsigned side0, unsigned cube1, unsigned side1) 
{
	if(g_iCurMode == BOARD_WAITPORTAL && boards[cube0].touched(side0, &boards[cube1], side1, g_iCurColor + 1))
	{
		g_iCurColor++;
		g_iCurMode &= ~BOARD_WAITPORTAL;
		for(int iCube = 0; iCube < NUM_CUBES; iCube++)
		{
			//Hide any flashy arrows
			boards[iCube].hideArrows();
			boards[iCube].resetFlashTimer();
		}
	}
}
	
void main()
{
	g_iCurColor = -1;
	g_iScore = -1;
    TimeStep ts;

	//Initialize our boards
	for(int i = 0; i < NUM_CUBES; i++)
		boards[i].init(i);
	Events::neighborAdd.set(onNeighborAdd);	//Function for when two cubes touch each other
	
	//Add marble to one
	Float2 fVel;
	fVel.set(0,0);
	Float2 fPos = LCD_center;
	fPos.x += TILE_WIDTH/2.0;
	fPos.y += TILE_HEIGHT/2.0;
	boards[0].addMarble(fPos, fVel);
	
	//Main loop
    while (1) 
	{
        ts.next();
		for(int i = 0; i < NUM_CUBES; i++)
		{
			int iMode;
			switch(g_iCurMode)
			{
				case BOARD_NOTHING:
					iMode = boards[i].update(float(ts.delta()));
					if(iMode & BOARD_GOTPOINT)
					{
						iMode ^= BOARD_GOTPOINT;
						g_iScore++;
						//TODO sound
					}
					if(iMode & BOARD_DIED)
					{
						//TODO game over screen
						iMode ^= BOARD_DIED;
					}
					g_iCurMode = iMode;
					break;
				case BOARD_WAITPORTAL:
					boards[i].waitPortal(float(ts.delta()));
					break;
			}
		}
		
        System::paint();
    }
}
