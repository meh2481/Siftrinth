#ifndef CUBEBOARD_H
#define CUBEBOARD_H

#include <sifteo.h>
#include "assets.gen.h"
#include "levels.h"
#include "Circle.h"
#include "TextDraw.h"
using namespace Sifteo;

//Return codes for CubeBoard::update()
#define BOARD_NOTHING		0
#define BOARD_WAITPORTAL	1
#define BOARD_DIED			2
#define BOARD_GOTPOINT		4
#define MODE_GAMEOVER		8

#define MARBLE_RADIUS	3

//Physics constants for marble movement
#define DEAD_ZONE		2.0f
#define ACCEL_SCALE		18.0f
#define DAMPING			0.98f
#define RESTITUTION		0.3f

#define NUM_PHYSICS_STEPS 3	//In case of lag or a large accelerometer reading, to prevent tunnelling

#define NUM_STARS	7	//How many stars per map
#define STAR_UPDATE_TIME	0.05

class Portal;
class CubeBoard;

class Portal
{
public:
	CubeBoard *other;
	int thisSide, otherSide;
};

class CubeBoard 
{
protected:
	VideoBuffer m_vid;
	bool m_bHasMarble;			//If the marble is in this cube or not
	Circle m_marble;			//Current marble position (If the marble's in this cube)
	Float2 m_marbleVelocity;	//Current marble speed (If the marble's in this cube)
	Portal m_pPortals[4];		//Portals to other cubes - one for each side
	short m_iPortalColors[4];	//The color each portal on each side is
	short m_iTilemap;			//What tilemap we're using
	bool m_bVWallSegments[TILEMAP_HEIGHT][TILEMAP_WIDTH-1];	//Line segments for horizontal walls
	bool m_bHWallSegments[TILEMAP_HEIGHT-1][TILEMAP_WIDTH];	//And vertical walls
	bool m_bFirstCycle;	//HACK
	bool m_bShowingArrows;	//If we're currently showing or hiding arrows
	float m_fAccumulatedArrowTime;
	int m_iSideOut;
	int m_iNumStars;
	int m_iCurStarFrames[NUM_STARS];
	float m_fUpdateStars;
	
	//Private methods
	bool isHole();	//If the marble is in a hole
	bool isStar();	//If the marble hit a star
	void checkWallCollision(Float2 candidate);	//Make sure the marble isn't hitting a wall, and bounce off it if so
	void makePortalColor(int side, int color);	//Make this portal in this cube have this color

public:
	CubeBoard();
	~CubeBoard();
	
	//Accessor methods
	bool hasMarble()		{return m_bHasMarble;};		//If this cube board has the marble in it
	void addMarble(Float2 pos, Float2 vel);				//Put the marble in this cube board
	void takeMarble()		{m_bHasMarble = false;};	//So we don't end up with broken arrows and such on reset
	VideoBuffer* getVid()	{return &m_vid;};
	
	//General methods
	int update(float fTimestep);	//Update the board and marble and such 
	void waitPortal(float fTimestep);	//Wait for a portal, flashing arrows at player
	void init(CubeID cube);			//Initialize this board to a particular cube number, setting up the display and such
	void initTilemap();				//Initialize to a random map
	bool touched(unsigned mySide, CubeBoard* other, unsigned otherSide, int iColor);	//When this cube touches another
	void resetFlashTimer()			{m_fAccumulatedArrowTime = 1.0f;};	//Reset the timer for flashing arrows
	void showArrows();							//Show flashing arrows for portals
	void hideArrows();							//Hide flashing arrows for portals
};




























#endif