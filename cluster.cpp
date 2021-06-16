#include "cluster.h"
#include <stdio.h>

cluster::cluster()
{
	wait_marbledrop = 0;
	wait_addanakins = 0;
	wait_aimove = 0;
	srand((unsigned int)time(NULL));

}

cluster::~cluster()
{
}

bool cluster::initialize(AI_LEVEL playerai, CVEC2 home)
{
	m_ai = playerai;
	m_homepos = home;

	m_cursorpos = CVEC2(-1, -1);
	m_cursorgridpos = CVEC2(-1, -1);
	m_cursortwistable = false;
	m_marblestobeadded = 0;

//	CVEC2 pos;
	int pos_x;
	int pos_y;

	// standard setup
	// empty grid
    for (int x=0; x<NUM_COLS; x++)
    {	
		totalmarbles[x] = 0;
		for (int y=0; y<NUM_ROWS; y++)
		{
			pos_x = x*64;
			pos_y = y*64;

			mg[x][y].initialize((MARBLECOLOR)(rand()%6), CVEC2(pos_x, pos_y));
			mg[x][y].setactive(false);
//			mg[x][y].m_unlocked = true;
			mg[x][y].setstate(MARBLESTATE::EMPTY);
        }
    }

	for (int x = 0; x < NUM_COLS - 1; x++)
	{
		for (int y = 0; y < NUM_ROWS - 1; y++)
		{
			m_moveable[x][y] = false;
		}
	}

	return true;
}

bool cluster::uninitialize()
{
	return true; 
}

bool cluster::update(int timepoint, int *matches)
{
	if(timepoint - wait_addanakins > WAIT_ANAKIN)
	{
		if(updatecount() < 24)
		{
			m_marblestobeadded += 6;
		}
		wait_addanakins = timepoint;
	}

	if(m_marblestobeadded > 0)
	{
		if(timepoint - wait_marbledrop > WAIT_DROP)
		{
			addmarbleset();
			wait_marbledrop = timepoint;
		}
	}

	CVEC2 nextmove;
	switch (m_ai)
	{
	case AI_LEVEL::NONE:
	case AI_LEVEL::HUMAN:
		break;

	case AI_LEVEL::JACK:
		if (timepoint - wait_aimove > WAIT_AI_JACK)
		{
			if(calculateaimove_random(&nextmove))
				starttwist(nextmove);
			wait_aimove = timepoint;
		}
		break;

	case AI_LEVEL::ERIK:
		if (timepoint - wait_aimove > WAIT_AI_ERIK)
		{
			if (calculateaimove_best(&nextmove))
				starttwist(nextmove);
			else
			{
				if (calculateaimove_random(&nextmove))
					starttwist(nextmove);

			}
			wait_aimove = timepoint;
		}
		break;

	case AI_LEVEL::NINI:
		if (timepoint - wait_aimove > WAIT_AI_NINI)
		{
			if (calculateaimove_best(&nextmove))
				starttwist(nextmove);
			else
			{
				if (calculateaimove_random(&nextmove))
					starttwist(nextmove);

			}
			wait_aimove = timepoint;
		}
		break;

	case AI_LEVEL::SVEN:
		if (timepoint - wait_aimove > WAIT_AI_SVEN)
		{
			if (calculateaimove_best(&nextmove))
				starttwist(nextmove);
			else
			{
				if (calculateaimove_random(&nextmove))
					starttwist(nextmove);

			}
			wait_aimove = timepoint;
		}
		break;

	case AI_LEVEL::JUDD:
		if (timepoint - wait_aimove > WAIT_AI_JUDD)
		{
			if (calculateaimove_random(&nextmove))
				starttwist(nextmove);
			wait_aimove = timepoint;
		}
		break;

	case AI_LEVEL::ANNA:
		if (timepoint - wait_aimove > WAIT_AI_ANNA)
		{
			if (calculateaimove_best(&nextmove))
				starttwist(nextmove);
			else
			{
				if (calculateaimove_random(&nextmove))
					starttwist(nextmove);

			}
			wait_aimove = timepoint;
		}
		break;

	case AI_LEVEL::VLAD:
		if (timepoint - wait_aimove > WAIT_AI_VLAD)
		{
			if (calculateaimove_best(&nextmove))
				starttwist(nextmove);
			else
			{
				if (calculateaimove_random(&nextmove))
					starttwist(nextmove);

			}
			wait_aimove = timepoint;
		}
		break;
	}

    for (int y=0; y<NUM_ROWS; y++)
    {
        for (int x=0; x<NUM_COLS; x++)
        {	
			mg[x][y].update();
		}
	}

	int cx = m_cursorgridpos.cx;
	int cy = m_cursorgridpos.cy;

	if(m_cursorgridpos == CVEC2(-1, -1))
	{
		m_cursortwistable = false;
	}
	else
	{
		if ((mg[cx][cy].getstate() == MARBLESTATE::READY) &&
			(mg[cx+1][cy].getstate() == MARBLESTATE::READY) &&
			(mg[cx+1][cy+1].getstate() == MARBLESTATE::READY) &&
			(mg[cx][cy+1].getstate() == MARBLESTATE::READY) )
		{
			m_cursortwistable = true;
		}
		else
		{
			m_cursortwistable = false;
		}
		
	}

    for (int y=1; y<NUM_ROWS; y++)
    {
        for (int x=0; x<NUM_COLS; x++)
        {	
			if ((mg[x][y].getstate() == MARBLESTATE::EMPTY) && (mg[x][y-1].getstate() == MARBLESTATE::READY))
			{
				CVEC2 temp = mg[x][y].getposition();
				mg[x][y] = mg[x][y-1];
				mg[x][y].setposition(temp);
				mg[x][y].setmotion(CVEC2(0, DROPSPEED));
				mg[x][y].setstate(MARBLESTATE::DROPS);
				mg[x][y-1].setstate(MARBLESTATE::EMPTY);

			}
		}
	}

	*matches = checkmatches();
	if(*matches > 0)
	{
		removematches();
	}

	return true;
}

bool cluster::handle_event()
{
	bool result = false;
	return result;
}

bool cluster::draw()
{
	// objects are not drawn at this level.
	return true;
}

bool cluster::addmarble()
{
	// TRUE if marble added
	// FALSE if board is full
	
	int leastmarbles = -1;
	int marblecount = 100;

    for (int x=0; x<NUM_COLS; x++)
    {
		if(totalmarbles[x] < marblecount)
		{
			leastmarbles = x;
			marblecount = totalmarbles[x];
			
		}
	}
	
	if(leastmarbles != -1)
	{
		return addmarbletocolumn(leastmarbles);
	}
	
	return false;
}

bool cluster::addmarbleset()
{
	// sorts least to most
	bool result = false;

	int selectedcolumn;
	int least;
	int x;

	int *order = new int[NUM_COLS];
	bool *skip = new bool[NUM_COLS];
	bool evencols = true;

	int checkeven = totalmarbles[0];

	for (x=0; x<NUM_COLS; x++)
	{
		if (checkeven != totalmarbles[x])
			evencols = false;
		skip[x] = false;
	}


	for (int i=0; i<NUM_COLS; i++)
	{
		selectedcolumn = -1;
		least = 100;
		for (x=0; x<NUM_COLS; x++)
		{
			if(!skip[x])
			{
				if(totalmarbles[x] < least)
				{
					selectedcolumn = x;
					least = totalmarbles[x];
				}
			}
		}
		order[i] = selectedcolumn;

//		if(!evencols)
			skip[selectedcolumn] = true;
	}

	for (x=0; x<NUM_COLS; x++)
	{
//		if(!skip[x])
		{
			if ((totalmarbles[order[x]] < NUM_ROWS) && (m_marblestobeadded > 0))
			{
				if (addmarbletocolumn(order[x]))
				{
					m_marblestobeadded--;
					result = true;
				}
			}
		}
	}

	delete[] order;
	delete[] skip;

	return result;
}


bool cluster::addmarbletocolumn(int column)
{
	bool result = false;

	if(mg[column][0].getstate() == MARBLESTATE::EMPTY)
	{
		CVEC2 pos  = CVEC2(column*64, 0);
		CVEC2 dpos = CVEC2(column*64, 0);
		MARBLECOLOR color = (MARBLECOLOR)(rand()%NUM_COLORS);
		result = mg[column][0].dropmarble(color, pos, dpos);
		totalmarbles[column]++;
	}

	return result;
}

bool cluster::removemarble(CVEC2 sm)
{

	if(mg[sm.cx][sm.cy].isactive())
	{
		mg[sm.cx][sm.cy].remove();
		totalmarbles[sm.cx]--;
	}

	return false;
}

bool cluster::istwistable(CVEC2 sm)
{
	bool result = false;
	int x = sm.cx;
	int y = sm.cy;
	if ((mg[x][y].getstate() == MARBLESTATE::READY) &&
		(mg[x + 1][y].getstate() == MARBLESTATE::READY) &&
		(mg[x + 1][y + 1].getstate() == MARBLESTATE::READY) &&
		(mg[x][y + 1].getstate() == MARBLESTATE::READY))
	{
		result = true;
	}
	return result;
}


// startmarble is the top left marble in the group
bool cluster::starttwist(CVEC2 sm)
{
	bool result = false;
	if(istwistable(sm))
	{
		marble temp;
		temp = marble(mg[sm.cx][sm.cy]);

		CVEC2 tmp_nw = mg[sm.cx][sm.cy].getposition();
		CVEC2 tmp_sw = mg[sm.cx][sm.cy+1].getposition();
		CVEC2 tmp_se = mg[sm.cx+1][sm.cy+1].getposition();
		CVEC2 tmp_ne = mg[sm.cx+1][sm.cy].getposition();

		// NW
		mg[sm.cx][sm.cy] = marble(mg[sm.cx][sm.cy+1]);
		mg[sm.cx][sm.cy].setposition(tmp_nw);
		mg[sm.cx][sm.cy].setmotion(CVEC2(0, -MOVESPEED));
		mg[sm.cx][sm.cy].setmovecounter(MOVESTEPS);
		mg[sm.cx][sm.cy].setstate(MARBLESTATE::TWIST);

		// SW
		mg[sm.cx][sm.cy+1] = marble(mg[sm.cx+1][sm.cy+1]);
		mg[sm.cx][sm.cy+1].setposition(tmp_sw);
		mg[sm.cx][sm.cy+1].setmotion(CVEC2(-MOVESPEED, 0));
		mg[sm.cx][sm.cy+1].setmovecounter(MOVESTEPS);
		mg[sm.cx][sm.cy+1].setstate(MARBLESTATE::TWIST);

		// SE
		mg[sm.cx+1][sm.cy+1] = marble(mg[sm.cx+1][sm.cy]);
		mg[sm.cx+1][sm.cy+1].setposition(tmp_se);
		mg[sm.cx+1][sm.cy+1].setmotion(CVEC2(0, MOVESPEED));
		mg[sm.cx+1][sm.cy+1].setmovecounter(MOVESTEPS);
		mg[sm.cx+1][sm.cy+1].setstate(MARBLESTATE::TWIST);

		// NE
		mg[sm.cx+1][sm.cy] = marble(temp);
		mg[sm.cx+1][sm.cy].setposition(tmp_ne);
		mg[sm.cx+1][sm.cy].setmotion(CVEC2(MOVESPEED, 0));
		mg[sm.cx+1][sm.cy].setmovecounter(MOVESTEPS);
//		mg[sm.cx+1][sm.cy].m_twisting = true;
//		mg[sm.cx+1][sm.cy].m_rem_moveable = false;
		mg[sm.cx+1][sm.cy].setstate(MARBLESTATE::TWIST);

	}

	return result;
}


int cluster::updatecount()
{
	int allmarbles = 0;
    for (int x=0; x<NUM_COLS; x++)
    {	
		totalmarbles[x] = 0;

		for (int y=0; y<NUM_ROWS; y++)
		{
			if(mg[x][y].getstate() != MARBLESTATE::EMPTY)
			{
				totalmarbles[x]++;
				allmarbles++;
			}
		}
	}

	return allmarbles;
}

int cluster::checkmatches()
{
	int completematches = 0;

	MARBLECOLOR color;

	for(int x=0; x<NUM_COLS; x++)
	{
		for(int y=0; y<NUM_ROWS; y++)
		{
//			mg[x][y].m_rem_moveable = false;
			mg[x][y].setremoval(false);

			if (mg[x][y].getstate() == MARBLESTATE::READY)
			{
//				mg[x][y].m_rem_moveable = true;
			}
		}
	}

	for(int x=0; x<NUM_COLS; x++)
	{
		for(int y=NUM_ROWS-1; y>=0; y--)
		{
			if (mg[x][y].getstate() == MARBLESTATE::READY)
			{
				color = mg[x][y].getcolor();

				// check N
				if(y > 1)
				{
					if( (mg[x][y-1].getcolor()==color) && mg[x][y-1].getstate() == MARBLESTATE::READY &&
						(mg[x][y-2].getcolor()==color) && mg[x][y-2].getstate() == MARBLESTATE::READY )
					{
						mg[x][y].setremoval(true);
						mg[x][y-1].setremoval(true);
						mg[x][y-2].setremoval(true);
						completematches++;
					}
				}

				// check E
				if(x < NUM_COLS-2)
				{
					if( (mg[x+1][y].getcolor() == color) && mg[x+1][y].getstate() == MARBLESTATE::READY &&
						(mg[x+2][y].getcolor() == color) && mg[x+2][y].getstate() == MARBLESTATE::READY )
					{
						mg[x][y].setremoval(true);
						mg[x+1][y].setremoval(true);
						mg[x+2][y].setremoval(true);
						completematches++;
					}
				}
			}
		}
	}

	return completematches;
}

bool cluster::removematches()
{
	for(int x=0; x<NUM_COLS; x++)
	{
		for(int y=0; y<NUM_ROWS; y++)
		{
			if(mg[x][y].istoberemoved())
			{
				removemarble(CVEC2(x, y));
			}
		}
	}

	return true;
}

bool cluster::checkmoveable()
{
	for (int x = 0; x < NUM_COLS-1; x++)
	{
		for (int y = 0; y < NUM_ROWS-1; y++)
		{
			m_moveable[x][y] = false;
			if ((mg[x][y].getstate() == MARBLESTATE::READY) &&
				(mg[x + 1][y + 1].getstate() == MARBLESTATE::READY) &&
				(mg[x][y + 1].getstate() == MARBLESTATE::READY) &&
				(mg[x + 1][y].getstate() == MARBLESTATE::READY) )
			{
				m_moveable[x][y] = true;
			}
		}
	}
	return true;
}

bool cluster::handle_keydown(int key)
{
/*
		// DEBUG TEST
	switch(key)
	{
	case SDLK_SPACE:
//		addmarble();
		m_marblestobeadded++;

		break;
	case SDLK_z:
		removemarble(CVEC2(0, NUM_ROWS-1));
		break;
	case SDLK_x:
		removemarble(CVEC2(1, NUM_ROWS-1));
		break;
	case SDLK_c:
		removemarble(CVEC2(2, NUM_ROWS-1));
		break;
	case SDLK_v:
		removemarble(CVEC2(3, NUM_ROWS-1));
		break;
	case SDLK_b:
		removemarble(CVEC2(4, NUM_ROWS-1));
		break;
	case SDLK_n:
		removemarble(CVEC2(5, NUM_ROWS-1));
		break;


	default:
		break;
	}
*/
	return false;
}

bool cluster::handle_mousemove(CVEC2 mouse)
{
	CVEC2 boardpos = mouse - m_homepos;

	if( (boardpos.cx > 32 ) && (boardpos.cy > 32 ) &&
		(boardpos.cx < 352) && (boardpos.cy < 608))
	{
		m_cursorgridpos.cx = ((boardpos.cx - 32) / 64);
		m_cursorgridpos.cy = ((boardpos.cy - 32) / 64);

		m_cursorpos.cx = ((m_cursorgridpos.cx) * 64 + m_homepos.cx - 1);
		m_cursorpos.cy = ((m_cursorgridpos.cy) * 64 + m_homepos.cy - 1);

	}
	else
	{
		m_cursorgridpos = CVEC2(-1, -1);
		m_cursorpos = CVEC2(-1, -1);
	}
	return true;
}

bool cluster::handle_mousedownleft(CVEC2 mouse)
{
	bool result = false;
	if(m_cursortwistable)
	{
		result = starttwist(m_cursorgridpos);
	}
	
	return result;
}

bool cluster::calculateaimove_random(CVEC2 *nexttwist)
{
	bool result = false;

	int totalmovable = 0;
	checkmoveable();

	for (int y = 0; y < NUM_ROWS - 1; y++)
	{
		for (int x = 0; x < NUM_COLS - 1; x++)
		{
			if (m_moveable[x][y])
			{
				totalmovable++;
			}
		}
	}

	if (totalmovable > 0)
	{
		int selectedmove = rand() % totalmovable;
		for (int y = 0; y < NUM_ROWS - 1; y++)
		{
			for (int x = 0; x < NUM_COLS - 1; x++)
			{
				if (m_moveable[x][y])
				{
					selectedmove--;
					if (selectedmove <= 0)
					{
						nexttwist->cx = (x);
						nexttwist->cy = (y);
						return true;
					}
				}
			}
		}
	}
	return result;
}

bool cluster::calculateaimove_best(CVEC2 *nexttwist)
{
	bool result = false;

	int selectedmove = -1;
	checkmoveable();

	int temp = -1;
	int mostbreak = -1;
	int totalmovable = 0;

	for (int y = 0; y < NUM_ROWS - 1; y++)
	{
		for (int x = 0; x < NUM_COLS - 1; x++)
		{
			if (m_moveable[x][y])
			{
				totalmovable++;
				temp = calculatematches(CVEC2(x, y));
				if (temp >= mostbreak)
				{
					mostbreak = temp;
					nexttwist->cx = (x);
					nexttwist->cy = (y);
					result = true;
				}
			}
		}
	}

	return result;
}

int	cluster::calculatematches(CVEC2 twist)
{
	int result = 0;
	marble tempmarble;

/*
	for (int y = 0; y < NUM_ROWS; y++)
	{
		for (int x = 0; x < NUM_COLS; x++)
		{
			mg[x][y].m_counted = false;
		}
	}
*/
	// rotate once
	tempmarble = mg[twist.cx][twist.cy];
	mg[twist.cx][twist.cy] = mg[twist.cx][twist.cy+1];
	mg[twist.cx][twist.cy+1] = mg[twist.cx+1][twist.cy+1];
	mg[twist.cx+1][twist.cy+1] = mg[twist.cx+1][twist.cy];
	mg[twist.cx+1][twist.cy] = tempmarble;

	int tx;
	int ty;

	// MARBLE NW
	tx = twist.cx;
	ty = twist.cy;
	result += calculatemarblematches(tx, ty);
	// MARBLE NE
	tx = twist.cx + 1;
	ty = twist.cy;
	result += calculatemarblematches(tx, ty);
	// MARBLE SE
	tx = twist.cx + 1;
	ty = twist.cy + 1;
	result += calculatemarblematches(tx, ty);
	// MARBLE SW
	tx = twist.cx;
	ty = twist.cy + 1;
	result += calculatemarblematches(tx, ty);

	// rotate back
	tempmarble = mg[twist.cx][twist.cy];
	mg[twist.cx][twist.cy] = mg[twist.cx + 1][twist.cy];
	mg[twist.cx + 1][twist.cy] = mg[twist.cx + 1][twist.cy + 1];
	mg[twist.cx + 1][twist.cy + 1] = mg[twist.cx][twist.cy + 1];
	mg[twist.cx][twist.cy + 1] = tempmarble;

	return result;
}


int cluster::calculatemarblematches(int tx, int ty)
{
	int result = 0;

	MARBLECOLOR color = mg[tx][ty].getcolor();
	// N
	if (ty > 1)
	{
		if ((mg[tx][ty - 1].getcolor() == color) && (mg[tx][ty - 2].getcolor() == color))
			result++;
	}
	// S
	if (ty < NUM_ROWS - 2)
	{
		if ((mg[tx][ty + 1].getcolor() == color) && (mg[tx][ty + 2].getcolor() == color))
			result++;
	}
	// W
	if (tx > 1)
	{
		if ((mg[tx - 1][ty].getcolor() == color) && (mg[tx - 2][ty].getcolor() == color))
			result++;
	}
	// E
	if (tx < NUM_COLS - 2)
	{
		if ((mg[tx + 1][ty].getcolor() == color) && (mg[tx + 2][ty].getcolor() == color))
			result++;
	}

	return result;
}