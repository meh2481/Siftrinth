#include "CubeBoard.h"

Random r;			//For generating random numbers
static TextDraw td;			//For drawing text

CubeBoard::CubeBoard()
{
	m_marble.radius = 3.0f;
	m_marble.pos.x = m_marble.pos.y = 0;
	m_iTilemap = 0;
	m_bHasMarble = false;
	m_marbleVelocity.x = m_marbleVelocity.y = 0;
	for(int i = 0; i < 4; i++)
	{
		m_pPortals[i].other = NULL;
		m_pPortals[i].bActivatable = true;
	}
	for(int i = 0; i < TILEMAP_HEIGHT; i++)
	{
		for(int j = 0; j < TILEMAP_HEIGHT-1; j++)
		{
			m_bHWallSegments[i][j] = false;
			m_bVWallSegments[j][i] = false;
		}
	}
	m_fAccumulatedArrowTime = 1.0f;
	m_bShowingArrows = false;
	m_iSideOut = -1;
	m_iNumStars = 0;
	m_fUpdateStars = 0.0f;
	m_iCurStyle = 0;
	m_vid.bg1.erase(Transparent);
}

CubeBoard::~CubeBoard()
{
}

bool CubeBoard::isStar()
{
	for(int iCurStar = 1; iCurStar <= m_iNumStars; iCurStar++)
	{
		Int2 starGridPos = m_vid.sprites[iCurStar].position() / TILE_WIDTH;
		Int2 ballGridPos = {m_marble.pos.x / TILE_WIDTH, m_marble.pos.y / TILE_HEIGHT};
		if(starGridPos.x == ballGridPos.x && starGridPos.y == ballGridPos.y && !m_vid.sprites[iCurStar].isHidden())
		{
			m_vid.sprites[iCurStar].hide();
			return true;
		}
	}
	return false;
}

bool CubeBoard::isHole()
{
	//Enable cheating by turning upside-down, just like real life
	float accel = m_vid.physicalAccel().z;
	if(accel < 0)
		return false;
		
	Int2 gridPos;
	gridPos.set(m_marble.pos.x / TILE_WIDTH, m_marble.pos.y / TILE_HEIGHT);
	if(gridPos.x < 0 || gridPos.x >= TILEMAP_WIDTH ||
	   gridPos.y < 0 || gridPos.y >= TILEMAP_HEIGHT)
		return false;	//Off the edge of screen; don't test for hole
	char cGridSquare = tilemap[m_iTilemap][gridPos.y][gridPos.x];
	if(cGridSquare == 4 ||
	   cGridSquare == 5 ||
	   cGridSquare == 6 ||
	   cGridSquare == 7)
	{
		//We're in a grid pos with a hole in it
		Circle c;
		c.pos.set(gridPos.x * TILE_WIDTH + TILE_WIDTH/2.0, gridPos.y * TILE_WIDTH + TILE_WIDTH/2.0);
		c.radius = TILE_WIDTH/2.0 - 1.5;
		if(c.touching(&m_marble.pos))
			return true;
	}
	return false;
}

void CubeBoard::checkWallCollision(Float2 candidate)
{
	if(m_marble.pos.x < 0 || m_marble.pos.x > LCD_width ||
	   m_marble.pos.y < 0 || m_marble.pos.y > LCD_height)
		return;
	//Grab the grid square the marble was originally in before moving
	Int2 gridSquare;
	gridSquare.set(m_marble.pos.x / TILE_WIDTH, m_marble.pos.y / TILE_HEIGHT);
	bool bHit = false;
	
	if(int(candidate.x - m_marble.radius - 1) / TILE_WIDTH < gridSquare.x && m_marbleVelocity.x < 0)	//Edge of ball is over in tile to left
	{
		if(gridSquare.x > 0 && m_bVWallSegments[gridSquare.y][gridSquare.x-1])
		{
			m_marbleVelocity.x *= -RESTITUTION;	//Bounce off
			hitWallNoise(m_marbleVelocity.x);
			bHit = true;
		}
	}
	else if(int(candidate.x + m_marble.radius - 1) / TILE_WIDTH > gridSquare.x && m_marbleVelocity.x > 0)	//Edge of ball is over in tile to right
	{
		if(gridSquare.x < TILEMAP_WIDTH-1 && m_bVWallSegments[gridSquare.y][gridSquare.x])
		{
			m_marbleVelocity.x *= -RESTITUTION;	//Bounce off
			hitWallNoise(m_marbleVelocity.x);
			bHit = true;
		}
	}
	if(int(candidate.y - m_marble.radius - 1) / TILE_HEIGHT < gridSquare.y && m_marbleVelocity.y < 0)	//Edge of ball is over in tile above
	{
		if(gridSquare.y > 0 && m_bHWallSegments[gridSquare.y-1][gridSquare.x])
		{
			m_marbleVelocity.y *= -RESTITUTION;	//Bounce off
			hitWallNoise(m_marbleVelocity.y);
			bHit = true;
		}
	}
	else if(int(candidate.y + m_marble.radius - 1) / TILE_HEIGHT > gridSquare.y && m_marbleVelocity.y > 0)	//Edge of ball is over in tile below
	{
		if(gridSquare.y < TILEMAP_HEIGHT-1 && m_bHWallSegments[gridSquare.y][gridSquare.x])
		{
			m_marbleVelocity.y *= -RESTITUTION;	//Bounce off
			hitWallNoise(m_marbleVelocity.y);
			bHit = true;
		}
	}
	
	if(!bHit)
	{	
		//Check corners -- sorta hackish and not entirely accurate, but works for the time being
		Circle c = m_marble;
		c.pos = candidate;
		c.radius = m_marble.radius-1.0;
		Float2 fCorner;
		//Check upper left corner
		fCorner.set(gridSquare.x * TILE_WIDTH, gridSquare.y * TILE_HEIGHT);
		if(gridSquare.x > 0 && gridSquare.y > 0 && m_bHWallSegments[gridSquare.y-1][gridSquare.x-1] && 
		   c.touching(&fCorner) && m_marbleVelocity.x < 0)
		{
			m_marbleVelocity.x *= -RESTITUTION;	//Bounce back with less velocity
			hitWallNoise(m_marbleVelocity.x);
		}
		if(gridSquare.x > 0 && gridSquare.y > 0 && m_bVWallSegments[gridSquare.y-1][gridSquare.x-1] && 
		   c.touching(&fCorner) && m_marbleVelocity.y < 0)
		{
			m_marbleVelocity.y *= -RESTITUTION;	//Bounce back with less velocity
			hitWallNoise(m_marbleVelocity.y);
		}
		//Check lower left corner
		fCorner.set(gridSquare.x * TILE_WIDTH, (gridSquare.y+1) * TILE_HEIGHT);
		if(gridSquare.x > 0 && gridSquare.y < TILEMAP_HEIGHT-1 && m_bHWallSegments[gridSquare.y][gridSquare.x-1] && 
		   c.touching(&fCorner) && m_marbleVelocity.x < 0)
		{
			m_marbleVelocity.x *= -RESTITUTION;	//Bounce back with less velocity
			hitWallNoise(m_marbleVelocity.x);
		}
		if(gridSquare.x > 0 && gridSquare.y < TILEMAP_HEIGHT-1 && m_bVWallSegments[gridSquare.y][gridSquare.x-1] && 
		   c.touching(&fCorner) && m_marbleVelocity.y > 0)
		{
			m_marbleVelocity.y *= -RESTITUTION;	//Bounce back with less velocity
			hitWallNoise(m_marbleVelocity.y);
		}
		//Check upper right corner
		fCorner.set((gridSquare.x+1) * TILE_WIDTH, gridSquare.y * TILE_HEIGHT);
		if(gridSquare.x < TILEMAP_WIDTH-1 && gridSquare.y > 0 && m_bHWallSegments[gridSquare.y-1][gridSquare.x] && 
		   c.touching(&fCorner) && m_marbleVelocity.x > 0)
		{
			m_marbleVelocity.x *= -RESTITUTION;	//Bounce back with less velocity
			hitWallNoise(m_marbleVelocity.x);
		}
		if(gridSquare.x < TILEMAP_WIDTH-1 && gridSquare.y > 0 && m_bVWallSegments[gridSquare.y-1][gridSquare.x] && 
		   c.touching(&fCorner) && m_marbleVelocity.y < 0)
		{	
			m_marbleVelocity.y *= -RESTITUTION;	//Bounce back with less velocity
			hitWallNoise(m_marbleVelocity.y);
		}
		//Check lower right corner
		fCorner.set((gridSquare.x+1) * TILE_WIDTH, (gridSquare.y+1) * TILE_HEIGHT);
		if(gridSquare.x < TILEMAP_WIDTH-1 && gridSquare.y < TILEMAP_HEIGHT-1 && m_bHWallSegments[gridSquare.y][gridSquare.x] && 
		   c.touching(&fCorner) && m_marbleVelocity.x > 0)
		{
			m_marbleVelocity.x *= -RESTITUTION;	//Bounce back with less velocity
			hitWallNoise(m_marbleVelocity.x);
		}
		if(gridSquare.x < TILEMAP_WIDTH-1 && gridSquare.y < TILEMAP_HEIGHT-1 && m_bVWallSegments[gridSquare.y][gridSquare.x] && 
		   c.touching(&fCorner) && m_marbleVelocity.y > 0)
		{
			m_marbleVelocity.y *= -RESTITUTION;	//Bounce back with less velocity
			hitWallNoise(m_marbleVelocity.y);
		}
	}
	
}

int CubeBoard::update(float fTimestep)
{
	int iReturn = BOARD_NOTHING;
	
	if(m_bHasMarble)
	{
		const Float2 minPosition = { m_marble.radius, m_marble.radius };
		const Float2 maxPosition = { LCD_width - m_marble.radius, LCD_height - m_marble.radius };
		const float portal_dist = 7 * TILE_WIDTH;
		const float portal2_dist = 9 * TILE_WIDTH;
		
		Float2 accel = m_vid.physicalAccel().xy();
		
		//Test and see if we're past an edge
		if(m_marble.pos.x < 0 || m_marble.pos.x > LCD_width)
			accel.y = m_marbleVelocity.y = 0;	//Don't allow the marble to move up or down
		if(m_marble.pos.y < 0 || m_marble.pos.y > LCD_height)
			accel.x = m_marbleVelocity.x = 0;	//Don't allow movement left or right
			
		if (accel.len2() > DEAD_ZONE * DEAD_ZONE)
			m_marbleVelocity += accel * fTimestep * ACCEL_SCALE;	//v1 = v0 + at
		m_marbleVelocity *= DAMPING;

		//Step the physics multiple times per frame to avoid potential tunnelling
		float fStepTimestep = fTimestep / float(NUM_PHYSICS_STEPS);
		for(int iStep = 0; iStep < NUM_PHYSICS_STEPS && m_bHasMarble; iStep++)
		{
			Float2 candidate = m_marble.pos + m_marbleVelocity * fStepTimestep;	//This will be our next position if we hit nothing
			
			//Check for collision with the edges of the screen (Except for portals)
			if (candidate.x < minPosition.x)	//Left side
			{
				if((candidate.y < portal_dist || candidate.y > portal2_dist) || !m_pPortals[LEFT].bActivatable)
				{
					if(m_marbleVelocity.x < 0) 
					{
						m_marbleVelocity.x *= -RESTITUTION;
						hitWallNoise(m_marbleVelocity.x);
					}
				}
				else if(candidate.x < -m_marble.radius)
				{
					CubeBoard* other = m_pPortals[LEFT].other;
					if(other == NULL)
					{
						//Wait for another cube to attach
						iReturn |= BOARD_WAITPORTAL;
						m_iSideOut = LEFT;
					}
					else
					{
						Float2 newVel = m_marbleVelocity;
						Float2 newPos = candidate;
						switch(m_pPortals[LEFT].otherSide)
						{
							case TOP:
								newVel.y = -m_marbleVelocity.x;
								newVel.x = m_marbleVelocity.y;
								newPos.x = candidate.y;
								newPos.y = -m_marble.radius;
								break;
							case LEFT:
								newVel.y = -m_marbleVelocity.y;
								newVel.x = -m_marbleVelocity.x;
								newPos.x = -m_marble.radius;
								newPos.y = LCD_height - candidate.y + 2;
								break;
							case BOTTOM:
								newVel.y = m_marbleVelocity.x;
								newVel.x = -m_marbleVelocity.y;
								newPos.x = LCD_width - candidate.y + 2;
								newPos.y = LCD_height + m_marble.radius;
								break;
							case RIGHT:
								newPos.x = LCD_width + m_marble.radius;
								break;
						}
						m_pPortals[LEFT].other->addMarble(newPos, newVel);
						m_bHasMarble = false;
						iReturn |= BOARD_LEFT;
					}
				}
			}
			else if (candidate.x > maxPosition.x)	//Right side
			{
				if((candidate.y < portal_dist || candidate.y > portal2_dist) || !m_pPortals[RIGHT].bActivatable)
				{
					if(m_marbleVelocity.x > 0) 
					{
						m_marbleVelocity.x *= -RESTITUTION;
						hitWallNoise(m_marbleVelocity.x);
					}
				}
				else if(candidate.x > LCD_width + m_marble.radius)
				{
					CubeBoard* other = m_pPortals[RIGHT].other;
					if(other == NULL)
					{
						//Wait for another cube to attach
						iReturn |= BOARD_WAITPORTAL;
						m_iSideOut = RIGHT;
					}
					else
					{
						Float2 newVel = m_marbleVelocity;
						Float2 newPos = candidate;
						switch(m_pPortals[RIGHT].otherSide)
						{
							case TOP:
								newVel.y = m_marbleVelocity.x;
								newVel.x = -m_marbleVelocity.y;
								newPos.x = LCD_width - candidate.y + 2;
								newPos.y = -m_marble.radius;
								break;
							case LEFT:
								newPos.x = -m_marble.radius;
								break;
							case BOTTOM:
								newVel.y = -m_marbleVelocity.x;
								newVel.x = m_marbleVelocity.y;
								newPos.x = candidate.y;
								newPos.y = LCD_height + m_marble.radius;
								break;
							case RIGHT:
								newVel.y = -m_marbleVelocity.y;
								newVel.x = -m_marbleVelocity.x;
								newPos.y = LCD_height - candidate.y + 2;
								newPos.x = LCD_width + m_marble.radius;
								break;
						}
						m_pPortals[RIGHT].other->addMarble(newPos, newVel);
						m_bHasMarble = false;
						iReturn |= BOARD_LEFT;
					}
				}
			}

			else if (candidate.y < minPosition.y)	//Top side
			{
				if((candidate.x < portal_dist || candidate.x > portal2_dist) || !m_pPortals[TOP].bActivatable)
				{
					if(m_marbleVelocity.y < 0)
					{
						m_marbleVelocity.y *= -RESTITUTION;
						hitWallNoise(m_marbleVelocity.y);
					}
				}
				else if(candidate.y < -m_marble.radius)
				{
					CubeBoard* other = m_pPortals[TOP].other;
					if(other == NULL)
					{
						//Wait for another cube to attach
						iReturn |= BOARD_WAITPORTAL;
						m_iSideOut = TOP;
					}
					else
					{
						Float2 newVel = m_marbleVelocity;
						Float2 newPos = candidate;
						switch(m_pPortals[TOP].otherSide)
						{
							case TOP:
								newVel.y = -m_marbleVelocity.y;
								newVel.x = -m_marbleVelocity.x;
								newPos.x = LCD_width - candidate.x + 2;
								newPos.y = -m_marble.radius;
								break;
							case LEFT:
								newVel.y = m_marbleVelocity.x;
								newVel.x = -m_marbleVelocity.y;
								newPos.y = candidate.x;
								newPos.x = -m_marble.radius;
								break;
							case BOTTOM:
								newPos.y = LCD_height + m_marble.radius-1;
								break;
							case RIGHT:
								newVel.y = -m_marbleVelocity.x;
								newVel.x = m_marbleVelocity.y;
								newPos.x = LCD_width + m_marble.radius;
								newPos.y = LCD_width - candidate.x + 2;
								break;
						}
						m_pPortals[TOP].other->addMarble(newPos, newVel);
						m_bHasMarble = false;
						iReturn |= BOARD_LEFT;
					}
				}
			}
			else if (candidate.y > maxPosition.y)	//Bottom side
			{
				if((candidate.x < portal_dist || candidate.x > portal2_dist) || !m_pPortals[BOTTOM].bActivatable)
				{
					if(m_marbleVelocity.y > 0) 
					{
						m_marbleVelocity.y *= -RESTITUTION;
						hitWallNoise(m_marbleVelocity.y);
					}
				}
				else if(candidate.y > LCD_height + m_marble.radius)
				{
					CubeBoard* other = m_pPortals[BOTTOM].other;
					if(other == NULL)
					{
						//Wait for another cube to attach
						iReturn |= BOARD_WAITPORTAL;
						m_iSideOut = BOTTOM;
					}
					else
					{
						Float2 newVel = m_marbleVelocity;
						Float2 newPos = candidate;
						switch(m_pPortals[BOTTOM].otherSide)
						{
							case TOP:
								newPos.y = -m_marble.radius;
								break;
							case LEFT:
								newVel.y = -m_marbleVelocity.x;
								newVel.x = m_marbleVelocity.y;
								newPos.y = LCD_width - candidate.x + 2;
								newPos.x = -m_marble.radius;
								break;
							case BOTTOM:
								newVel.y = -m_marbleVelocity.y;
								newVel.x = -m_marbleVelocity.x;
								newPos.x = LCD_width - candidate.x + 2;
								newPos.y = LCD_height - m_marble.radius; //This seems botched for some reason unless we do this...
								break;
							case RIGHT:
								newVel.y = m_marbleVelocity.x;
								newVel.x = -m_marbleVelocity.y;
								newPos.y = candidate.x;
								newPos.x = LCD_width - m_marble.radius;
								break;
						}
						m_pPortals[BOTTOM].other->addMarble(newPos, newVel);
						m_bHasMarble = false;
						iReturn |= BOARD_LEFT;
					}
				}
			}
			
			//Check for collisions with walls of surrounding grid squares
			checkWallCollision(candidate);
			
			//Update the marble position
			m_marble.pos += m_marbleVelocity * fStepTimestep;
		}
		
		//Draw the marble in this new position
		Float2 drawPos = m_marble.pos;
		m_vid.sprites[0].move(m_marble.pos.x - TILE_WIDTH/2.0, m_marble.pos.y - TILE_HEIGHT/2.0);
		if(isHole())
		{
			iReturn |= BOARD_DIED;
			m_vid.sprites[0].hide();
		}
		if(isStar())
			iReturn |= BOARD_GOTPOINT;
	}
	
	//Update star animation
	m_fUpdateStars += fTimestep;
	if(m_fUpdateStars >= STAR_UPDATE_TIME)
	{
		m_fUpdateStars -= STAR_UPDATE_TIME;
		for(int i = 1; i <= m_iNumStars; i++)
		{
			if(m_vid.sprites[i].isHidden())
				continue;
			m_iCurStarFrames[i-1]++;
			if(m_iCurStarFrames[i-1] >= Star.numFrames())
				m_iCurStarFrames[i-1] = 0;
			m_vid.sprites[i].setImage(Star, m_iCurStarFrames[i-1]);
		}
	}
	return iReturn;
}

void CubeBoard::init(CubeID cube)
{
	m_vid.initMode(BG0_SPR_BG1);
	m_vid.attach(cube);
	initTilemap();
}

//Read in a random maze
void CubeBoard::initTilemap()
{
	m_vid.bg1.erase(Transparent);
	//If there are any portals, destroy them
	for(int i = 0; i < 4; i++)
	{
		//Reset all portals to be activateable
		m_pPortals[i].bActivatable = true;
		if(m_pPortals[i].other != NULL)
		{
			for(int j = 0; j < 4; j++)
			{
				if(m_pPortals[i].other->m_pPortals[j].other == this &&
				   m_pPortals[i].other->m_pPortals[j].thisSide == m_pPortals[i].otherSide &&
				   m_pPortals[i].other->m_pPortals[j].otherSide == m_pPortals[i].thisSide)
					m_pPortals[i].other->m_pPortals[j].other = NULL;	//Destroy other link
			}
			m_pPortals[i].other = NULL;	//And this link
		}
	}
	m_iNumStars = 0;
	m_iTilemap = r.randint(0,NUM_MAPS-1);	//Hang onto this for collision testing later
	m_iCurStyle = r.randint(0,3);	//Style of tilemap we're using this time around
	for(int y = 0; y < TILEMAP_HEIGHT; y++)
	{
		for(int x = 0; x < TILEMAP_WIDTH; x++)
		{
			Byte2 pos = {x, y};
			char iVal = tilemap[m_iTilemap][y][x];
			//Add star if it's supposed to go here
			if(iVal >= 20)
			{
				iVal -= 20;
				m_iCurStarFrames[m_iNumStars] = r.randint(0, Star.numFrames()-1);	//Init star to random frame
				m_iNumStars++;
				m_vid.sprites[m_iNumStars].setImage(Star, m_iCurStarFrames[m_iNumStars-1]);
				m_vid.sprites[m_iNumStars].move(pos.x * TILE_WIDTH, pos.y * TILE_HEIGHT);
			}
			//Draw the proper background image for this tile
			switch(m_iCurStyle)
			{
				case 0:
					m_vid.bg0.image(pos, BackgroundTiles1, iVal);
					break;
				case 1:
					m_vid.bg0.image(pos, BackgroundTiles2, iVal);
					break;
				case 2:
					m_vid.bg0.image(pos, BackgroundTiles3, iVal);
					break;
				default:
					m_vid.bg0.image(pos, BackgroundTiles4, iVal);
					break;
			}
			//Add collision data for walls
			if(x != 0)
			{
				if(iVal == 2 ||
				   iVal == 3 ||
				   iVal == 6 ||
				   iVal == 7)
					m_bVWallSegments[y][x-1] = true;	//There's a vertical wall here
				else
					m_bVWallSegments[y][x-1] = false;
			}
			if(y != 0)
			{
				if(iVal == 1 ||
				   iVal == 3 ||
				   iVal == 5 ||
				   iVal == 7)
					m_bHWallSegments[y-1][x] = true;	//There's a horizontal wall here
				else
					m_bHWallSegments[y-1][x] = false;	
			}
		}
	}
	
	//Determine sides that the portals can form on
	//Left side
	char iVal = tilemap[m_iTilemap][7][0];
	if(iVal >= 20)
		iVal -= 20;
	if(!(iVal == 1 || iVal == 5))	//Not the correct wall here; no portal
		m_pPortals[LEFT].bActivatable = false;
	//Right side
	iVal = tilemap[m_iTilemap][7][15];
	if(iVal >= 20)
		iVal -= 20;
	if(!(iVal == 1 || iVal == 5 || iVal == 3 || iVal == 7))	//Not the correct wall here; no portal
		m_pPortals[RIGHT].bActivatable = false;
	//Top side
	iVal = tilemap[m_iTilemap][0][7];
	if(iVal >= 20)
		iVal -= 20;
	if(!(iVal == 2 || iVal == 6))	//Not the correct wall here; no portal
		m_pPortals[TOP].bActivatable = false;
	//Bottom side
	iVal = tilemap[m_iTilemap][15][7];
	if(iVal >= 20)
		iVal -= 20;
	if(!(iVal == 2 || iVal == 6 || iVal == 3 || iVal == 7))	//Not the correct wall here; no portal
		m_pPortals[BOTTOM].bActivatable = false;
	
	
	//Now draw white (blank) portals
	for(int i = 0; i < 4; i++)
	{
		if(m_pPortals[i].bActivatable)
			makePortalColor(i, -1);
	}
}

//Add a marble to this board
void CubeBoard::addMarble(Float2 pos, Float2 vel)
{
	m_marble.pos = pos;
	m_marbleVelocity = vel; 
	m_bHasMarble = true;
	m_vid.sprites[0].setImage(Ball,0);
	pos.x -= TILE_WIDTH/2.0;
	pos.y -= TILE_HEIGHT/2.0;
	m_vid.sprites[0].move(pos);
	m_marble.radius = MARBLE_RADIUS;
	m_vid.bg1.erase(Transparent);
}

bool CubeBoard::touched(unsigned mySide, CubeBoard* other, unsigned otherSide, int iColor)
{
	if(!m_bHasMarble)
	{
		if(other->hasMarble())
			return other->touched(otherSide, this, mySide, iColor);
		return false;	//If neither has the marble, ignore
	}
	//If either portal is claimed already, return
	if(m_pPortals[mySide].other != NULL || other->m_pPortals[otherSide].other != NULL)
		return false;
	
	//If either side cannot make a portal there, return
	if(!m_pPortals[mySide].bActivatable || !other->m_pPortals[otherSide].bActivatable)
		return false;
	
	if(m_iSideOut != mySide)	//Only create portal on side the ball went through
		return false;
	
	//Make connection
	m_pPortals[mySide].other = other;
	m_pPortals[mySide].otherSide = otherSide;
	m_pPortals[mySide].thisSide = mySide;
	other->m_pPortals[otherSide].other = this;
	other->m_pPortals[otherSide].thisSide = otherSide;
	other->m_pPortals[otherSide].otherSide = mySide;
	
	//Make portal colors
	makePortalColor(mySide, iColor);
	other->makePortalColor(otherSide, iColor);
	
	return true;
}

void CubeBoard::makePortalColor(int side, int color)
{
	Int2 pos;
	m_pPortals[side].color = color;	//Save this color
	switch(side)
	{
		case LEFT:
			pos.x = 0;
			pos.y = 7;
			switch(m_iCurStyle)
			{
				case 0:
					m_vid.bg0.image(pos, PortalsL1, color+1);
					break;
				case 1:
					m_vid.bg0.image(pos, PortalsL2, color+1);
					break;
				case 2:
					m_vid.bg0.image(pos, PortalsL3, color+1);
					break;
				default:
					m_vid.bg0.image(pos, PortalsL4, color+1);
					break;
			}
			break;
		case TOP:
			pos.x = 7;
			pos.y = 0;
			switch(m_iCurStyle)
			{
				case 0:
					m_vid.bg0.image(pos, PortalsT1, color+1);
					break;
				case 1:
					m_vid.bg0.image(pos, PortalsT2, color+1);
					break;
				case 2:
					m_vid.bg0.image(pos, PortalsT3, color+1);
					break;
				default:
					m_vid.bg0.image(pos, PortalsT4, color+1);
					break;
			}
			break;
		case RIGHT:
			pos.x = 15;
			pos.y = 7;
			switch(m_iCurStyle)
			{
				case 0:
					m_vid.bg0.image(pos, PortalsR1, color+1);
					break;
				case 1:
					m_vid.bg0.image(pos, PortalsR2, color+1);
					break;
				case 2:
					m_vid.bg0.image(pos, PortalsR3, color+1);
					break;
				default:
					m_vid.bg0.image(pos, PortalsR4, color+1);
					break;
			}
			break;
		case BOTTOM:
			pos.y = 15;
			pos.x = 7;
			switch(m_iCurStyle)
			{
				case 0:
					m_vid.bg0.image(pos, PortalsB1, color+1);
					break;
				case 1:
					m_vid.bg0.image(pos, PortalsB2, color+1);
					break;
				case 2:
					m_vid.bg0.image(pos, PortalsB3, color+1);
					break;
				default:
					m_vid.bg0.image(pos, PortalsB4, color+1);
					break;
			}
			break;
	}
}

void CubeBoard::waitPortal(float fTimestep)
{
	m_fAccumulatedArrowTime += fTimestep;
	if(m_fAccumulatedArrowTime > 1.0)
	{
		m_fAccumulatedArrowTime -= 1.0;
		if(m_bShowingArrows)
			hideArrows();
		else
			showArrows();
	}
}

UInt2 getPos(int iSide)
{
	UInt2 pos;
	switch(iSide)
	{
		case LEFT:
			pos.x = 1;
			pos.y = 7;
			break;
		case TOP:
			pos.x = 7;
			pos.y = 1;
			break;
		case RIGHT:
			pos.x = 13;
			pos.y = 7;
			break;
		case BOTTOM:
			pos.x = 7;
			pos.y = 13;
			break;
	}
	return pos;
}

void CubeBoard::showArrows()
{
	if(m_bShowingArrows)
		return;
	
	BG1Mask mask;
	mask.clear();
	UInt2 size = {2,2};
	
	if(m_bHasMarble)	//This is where the marble's at
	{
		if(m_pPortals[m_iSideOut].other == NULL)
		{
			UInt2 pos = getPos(m_iSideOut);
			mask.fill(pos, size);
			m_vid.bg1.setMask(mask);
			m_vid.bg1.image(pos, ArrowsRed, m_iSideOut);
		}
		td.draw(&m_vid, "Tap to cancel", 10);
		if(m_iSideOut == BOTTOM)	//HACK: Why this no work otherwise?
		{
			UInt2 pos = getPos(m_iSideOut);
			m_vid.bg1.image(pos, ArrowsRed, m_iSideOut);	//Why do we have to draw this again?
		}
	}
	else
	{
		for(int iSide = 0; iSide < 4; iSide++)
		{
			if(m_pPortals[iSide].other != NULL || !m_pPortals[iSide].bActivatable)
				continue;
			
			UInt2 pos = getPos(iSide);
			
			mask.fill(pos, size);
		}
		m_vid.bg1.setMask(mask);
		for(int iSide = 0; iSide < 4; iSide++)
		{
			if(m_pPortals[iSide].other != NULL || !m_pPortals[iSide].bActivatable)
				continue;
			
			UInt2 pos = getPos(iSide);
			m_vid.bg1.image(pos, Arrows, iSide);
		}
	}
	
	m_bShowingArrows = true;
}

void CubeBoard::hideArrows()
{
	if(!m_bShowingArrows)
		return;
	
	//BG1Mask mask;
	//mask.clear();
	//m_vid.bg1.setMask(mask);
	m_vid.bg1.erase(Transparent);
	
	m_bShowingArrows = false;
}

void CubeBoard::spitBack()
{
	m_marbleVelocity *= -1;
	switch(m_iSideOut)
	{
		case LEFT:
			m_marble.pos.x += m_marble.radius;
			break;
		case TOP:
			m_marble.pos.y += m_marble.radius;
			break;
		case RIGHT:
			m_marble.pos.x -= m_marble.radius;
			break;
		case BOTTOM:
			m_marble.pos.y -= m_marble.radius;
			break;
	}
}

void CubeBoard::reset(bool* bColorList)
{
	//We have to do this here outside of initTilemap() as well in order to make sure the colors of portals come out right 
	for(int i = 0; i < 4; i++)
	{
		if(m_pPortals[i].other != NULL)
		{
			bColorList[m_pPortals[i].color] = false;
			m_pPortals[i].other->m_pPortals[m_pPortals[i].otherSide].other = NULL;
			//Reset portal color on other cube
			m_pPortals[i].other->makePortalColor(m_pPortals[i].otherSide, -1);
			m_pPortals[i].other = NULL;
			m_pPortals[i].bActivatable = true;
		}
	}
	initTilemap();
	//Draw portals on this board white
	/*for(int i = 0; i < 4; i++)
	{
		if(m_pPortals[i].bActivatable)
			makePortalColor(i, -1);
	}*/
}










