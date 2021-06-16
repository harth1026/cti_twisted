#ifndef TWISTED_CLUSTER_H
#define TWISTED_CLUSTER_H

#define NUM_COLORS		6
#define NUM_COLS		6
#define NUM_ROWS		10

#define WAIT_DROP		750		// time until next drop
#define WAIT_ANAKIN		1000	// brings balance 

// AI move speed
#define WAIT_AI_JACK	1000
#define WAIT_AI_ERIK	3000
#define WAIT_AI_NINI	2500
#define WAIT_AI_SVEN	2000
#define WAIT_AI_JUDD	300
#define WAIT_AI_ANNA	1500
#define WAIT_AI_VLAD	1000

#include <stdlib.h>
#include <time.h>

#include "marble.h"
#include <libcti/libcti.h>

enum class AI_LEVEL
{
	NONE,
	HUMAN,
	JACK,
	ERIK,
	NINI,
	SVEN,
	JUDD,		// Ice Cream Truck Driver
	ANNA,
	VLAD,
};

class cluster
{
private:
	int		totalmarbles[NUM_COLS];

	CVEC2 m_homepos;			// real home position
	CVEC2 m_cursorpos;		// real cursor position
	CVEC2 m_cursorgridpos;	// cursor grid position
	bool	m_cursortwistable;

	AI_LEVEL	m_ai;					// ai level

	bool	m_moveable[NUM_COLS - 1][NUM_ROWS - 1];
	int		m_marblestobeadded;	// marbles to be added

	int		wait_marbledrop;	// how often new marbles will be dropped
	int		wait_addanakins;	// how often new marbles are added to the queue
	int		wait_aimove;		// how often an AI will move

	bool	istwistable(CVEC2 sm);

public:
	marble	mg[NUM_COLS][NUM_ROWS];

	cluster();
	~cluster();

	bool	initialize(AI_LEVEL playerai, CVEC2 home);
	bool	uninitialize();
	bool	update(int timepoint, int *matches);
	bool	handle_event();
	bool	draw();

	CVEC2	gethomepos()					{ return m_homepos;			}
	CVEC2 getcursorpos()					{ return m_cursorpos;		}
	CVEC2 getcursorgridpos()				{ return m_cursorgridpos;	}
	bool	iscursortwistable()				{ return m_cursortwistable;	}

	void	sethomepos(CVEC2 pos)			{ m_homepos = pos;			}
	void	setcursorpos(CVEC2 pos)			{ m_cursorpos = pos;		}
	void	setcursorgridpos(CVEC2 pos)		{ m_cursorgridpos = pos;	}
	void	queuemarblestoadd(int count)	{ m_marblestobeadded += count; }

	bool	addmarble();
	bool	addmarbleset();
	bool	addmarbletocolumn(int column);
	bool	removemarble(CVEC2 sm);

	bool	starttwist(CVEC2 sm);
	int		updatecount();

	int		checkmatches();
	bool	removematches();
	bool	checkmoveable();

	// controls
	bool	handle_keydown(int key);
	bool	handle_mousemove(CVEC2 mouse);
	bool	handle_mousedownleft(CVEC2 mouse);

	// ai
	bool	calculateaimove_random(CVEC2 *nexttwist);
	bool	calculateaimove_best(CVEC2 *nexttwist);

	int		calculatematches(CVEC2 twist);
	int		calculatemarblematches(int tx, int ty);
};

#endif


