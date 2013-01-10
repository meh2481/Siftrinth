/*
 * Siftrinth - Marble labyrinth program
 */

#include "CubeBoard.h"
#define NUM_CUBES	3
#define TAP_PROMPT_DELAY	3.0

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

static void onTouch(void* ctxt, unsigned cube)
{
	if(g_iCurMode & MODE_GAMEOVER && boards[cube].hasMarble())	//Tap on the "game over" cube to restart
	{
		for(int i = 0; i < NUM_CUBES; i++)
		{
			boards[i].getVid()->bg1.eraseMask();	//Wipe "game over" screen
			boards[i].initTilemap();	//Load a new tilemap into each
			boards[i].takeMarble();
		}
		//Add marble to first cube
		Float2 fVel;
		fVel.set(0,0);
		Float2 fPos = LCD_center;
		fPos.x += TILE_WIDTH/2.0;
		fPos.y += TILE_HEIGHT/2.0;
		boards[0].addMarble(fPos, fVel);
		
		//Other starting tasks
		g_iCurColor = -1;
		g_iScore = -1;
		g_iCurMode = BOARD_NOTHING;
	}
	if(g_iCurMode & BOARD_WAITPORTAL && boards[cube].hasMarble())	//Tap on the flashy arrow cube to ignore portal
	{
		boards[cube].spitBack();	//Spit marble back out
		g_iCurMode = BOARD_NOTHING;
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
	float fTapPromptDelay = 0.0;
	int iBoardDied;

	//Initialize our boards
	for(int i = 0; i < NUM_CUBES; i++)
		boards[i].init(i);
	Events::neighborAdd.set(onNeighborAdd);	//Function for when two cubes touch each other
	Events::cubeTouch.set(onTouch);			//Function for when a cube is tapped
	
	//Add marble to one
	Float2 fVel;
	fVel.set(0,0);
	Float2 fPos = LCD_center;
	fPos.x += TILE_WIDTH/2.0;
	fPos.y += TILE_HEIGHT/2.0;
	boards[0].addMarble(fPos, fVel);
	
	TextDraw td;
	
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
						iMode |= MODE_GAMEOVER;
						td.draw(boards[i].getVid(), "Game over", 6);
						String<64> s;
						s << "Score: " << g_iScore;
						td.draw(boards[i].getVid(), s.c_str(), 8);
						fTapPromptDelay = 0.0;
						iBoardDied = i;
					}
					g_iCurMode = iMode;
					break;
				case BOARD_WAITPORTAL:
					boards[i].waitPortal(float(ts.delta()));
					break;
				case MODE_GAMEOVER:
					fTapPromptDelay += float(ts.delta()) / 3.0;
					if(fTapPromptDelay >= TAP_PROMPT_DELAY && fTapPromptDelay < 100.0)
					{
						fTapPromptDelay = 100;
						td.draw(boards[iBoardDied].getVid(), "Tap to restart", 14);
					}
			}
		}
		
        System::paint();
    }
}
