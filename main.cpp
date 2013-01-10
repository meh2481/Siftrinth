/*
 * Siftrinth - Marble labyrinth program
 */

#include "CubeBoard.h"
#define NUM_CUBES			3
#define TAP_PROMPT_DELAY	3.0
#define NUM_COLORS			6
#define NUM_STARS_CUBE		7

//Define our sound channels
#define MUSIC_CHANNEL		0
#define PORTAL_CHANNEL		1
#define BALL_SFX_CHANNEL	2
#define BOARD_SFX_CHANNEL	3

static AssetSlot MainSlot = AssetSlot::allocate()
    .bootstrap(GameAssets);

static Metadata M = Metadata()
    .title("Siftrinth")
    .package("com.markh.siftrinth", "1.0")
    .icon(Icon)
    .cubeRange(NUM_CUBES);

static CubeBoard boards[NUM_CUBES];
static short g_starsCollected[NUM_CUBES];
static bool g_bColorsUsed[NUM_COLORS];
static int g_iCurMode;
static int g_iScore;
static int g_iBoardReset;	//What board to reset if it's empty
static AudioChannel* channels[4];

int findNextColor()
{
	for(int i = 0; i < NUM_COLORS; i++)
	{
		if(!g_bColorsUsed[i])
			return i;
	}
	return 0;	//Error
}

static void onNeighborAdd(void* ctxt, unsigned cube0, unsigned side0, unsigned cube1, unsigned side1) 
{
	int iNextColor = findNextColor();
	if(g_iCurMode == BOARD_WAITPORTAL && boards[cube0].touched(side0, &boards[cube1], side1, iNextColor))
	{
		g_bColorsUsed[iNextColor] = true;
		g_iCurMode ^= BOARD_WAITPORTAL;
		for(int iCube = 0; iCube < NUM_CUBES; iCube++)
		{
			//Hide any flashy arrows
			boards[iCube].hideArrows();
			boards[iCube].resetFlashTimer();
		}
		//Play sound effect for exiting a portal
		channels[PORTAL_CHANNEL]->play(sPortalExit);
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
		for(int i = 0; i < NUM_COLORS; i++)
			g_bColorsUsed[i] = false;
		g_iScore = -1;
		g_iCurMode = BOARD_NOTHING;
		for(int i = 0; i < NUM_CUBES; i++)
			g_starsCollected[i] = 0;
		g_iBoardReset = -1;
	}
	if(g_iCurMode & BOARD_WAITPORTAL && boards[cube].hasMarble())	//Tap on the flashy arrow cube to ignore portal
	{
		boards[cube].spitBack();	//Spit marble back out
		//Play sound effect for exiting a portal
		channels[PORTAL_CHANNEL]->play(sPortalExit);
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
	//Create our audio channels
	AudioChannel a1(0);
	AudioChannel a2(2);
	AudioChannel a3(3);
	AudioChannel a4(4);
	channels[0] = &a1;
	channels[1] = &a2;
	channels[2] = &a3;
	channels[3] = &a4;
	
	//AudioChannel(0).play(sMusic, AudioChannel(0).REPEAT);	//Start playing music
	for(int i = 0; i < NUM_CUBES; i++)
		g_starsCollected[i] = 0;
	for(int i = 0; i < NUM_COLORS; i++)
		g_bColorsUsed[i] = false;
	g_iBoardReset = -1;
	g_iScore = -1;
    TimeStep ts;
	float fTapPromptDelay = 0.0;
	int iBoardDied;

	//Initialize our boards
	for(int i = 0; i < NUM_CUBES; i++)
		boards[i].init(i);
	Events::neighborAdd.set(onNeighborAdd);	//Function for when two cubes touch each other
	Events::cubeTouch.set(onTouch);			//Function for when a cube is tapped
	
	//Add the marble to one of them
	Float2 fVel;
	fVel.set(0,0);
	Float2 fPos = LCD_center;
	fPos.x += TILE_WIDTH/2.0;
	fPos.y += TILE_HEIGHT/2.0;
	boards[0].addMarble(fPos, fVel);
	
	TextDraw td;
	bool bFirstSound = true;
	
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
						if(++g_starsCollected[i] == NUM_STARS_CUBE)
							g_iBoardReset = i;
						//Play sound for getting a star, but not right on reset
						if(bFirstSound)
							bFirstSound = false;
						else
							channels[BALL_SFX_CHANNEL]->play(sGetStar);
					}
					if(iMode & BOARD_DIED)
					{
						//Show game over screen
						iMode ^= BOARD_DIED;
						iMode |= MODE_GAMEOVER;
						td.draw(boards[i].getVid(), "Game over", 6);
						String<64> s;
						s << "Score: " << g_iScore;
						td.draw(boards[i].getVid(), s.c_str(), 8);
						fTapPromptDelay = 0.0;
						iBoardDied = i;
						bFirstSound = true;
					}
					if(iMode & BOARD_LEFT)
					{
						iMode ^= BOARD_LEFT;
						if(g_iBoardReset == i)
						{
							boards[i].reset(g_bColorsUsed);
							g_starsCollected[i] = 0;
							g_iBoardReset = -1;
							g_iScore += 3;	//Three points for clearing board
							//TODO: Some kind of sound or effect for clearing board
						}
						//Play pass-through-portal sound
						else if(!channels[PORTAL_CHANNEL]->isPlaying())
							channels[PORTAL_CHANNEL]->play(sThroughPortal);
					}
					if(iMode & BOARD_WAITPORTAL)
					{
						//Play sound effect for entering a portal
						channels[PORTAL_CHANNEL]->play(sPortalEnter);
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
