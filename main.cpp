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
#define BALL_ROLL_CHANNEL	4
#define BALL_HIT_CHANNEL	5

#define REPEAT	channels[0]->REPEAT	//Annoying enums within classes...
#define MAX_VOLUME	channels[0]->MAX_VOLUME

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
static AudioChannel* channels[6];

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
		//Play sound effect for clearing board (also board reset-- why not?)
		channels[BOARD_SFX_CHANNEL]->play(sBoardClear);
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

void hitWallNoise(float fVelocity)
{
	fVelocity *= fVelocity;
		
	fVelocity /= 10.0;
	if(fVelocity > MAX_VOLUME)
		fVelocity = MAX_VOLUME;
	channels[BALL_HIT_CHANNEL]->play(sHitWall);
	channels[BALL_HIT_CHANNEL]->setVolume(fVelocity);
}
	
void main()
{
	//Create our audio channels
	AudioChannel a1(0);
	AudioChannel a2(1);
	AudioChannel a3(2);
	AudioChannel a4(3);
	AudioChannel a5(4);
	AudioChannel a6(5);
	channels[0] = &a1;
	channels[1] = &a2;
	channels[2] = &a3;
	channels[3] = &a4;
	channels[4] = &a5;
	channels[5] = &a6;
	
	channels[BALL_ROLL_CHANNEL]->play(sRollLoop, REPEAT);	//Start playing rolling marble noise
	channels[BALL_ROLL_CHANNEL]->setVolume(0);
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
					//Update our rolling sound to the right volume
					if(boards[i].hasMarble())
					{
						float fVol = boards[i].getMarbleVelocity() * 0.9;
						if(fVol > MAX_VOLUME)
							fVol = MAX_VOLUME;
						channels[BALL_ROLL_CHANNEL]->setVolume(fVol);
					}
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
						channels[BALL_ROLL_CHANNEL]->setVolume(0);
						channels[BALL_SFX_CHANNEL]->play(sDie);
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
							//Play sound effect for clearing board
							channels[BOARD_SFX_CHANNEL]->play(sBoardClear);
						}
						//Play pass-through-portal sound
						else if(!channels[PORTAL_CHANNEL]->isPlaying())
							channels[PORTAL_CHANNEL]->play(sThroughPortal);
					}
					if(iMode & BOARD_WAITPORTAL)
					{
						//Play sound effect for entering a portal
						channels[PORTAL_CHANNEL]->play(sPortalEnter);
						channels[BALL_ROLL_CHANNEL]->setVolume(0);
					}
					g_iCurMode = iMode;
					break;
				case BOARD_WAITPORTAL:
					boards[i].waitPortal(float(ts.delta()));
					channels[BALL_ROLL_CHANNEL]->setVolume(0);
					break;
				case MODE_GAMEOVER:
					fTapPromptDelay += float(ts.delta()) / 3.0;
					if(fTapPromptDelay >= TAP_PROMPT_DELAY && fTapPromptDelay < 100.0)
					{
						fTapPromptDelay = 100;
						td.draw(boards[iBoardDied].getVid(), "Tap to restart", 14);
					}
					channels[BALL_ROLL_CHANNEL]->setVolume(0);
			}
		}
		
        System::paint();
    }
}
