#ifndef TWISTED_MARBLE_H
#define TWISTED_MARBLE_H

#include <libcti/libcti.h>

#define MOVESPEED	4
#define MOVESTEPS	16

#define DROPSPEED	8

enum class MARBLESTATE
{
	EMPTY = 0,		// No marble
	READY = 1,		// Active and ready
	DROPS = 2,		// Dropping
	TWIST = 3,		// Twisting
	LEAVE = 4,		// Leaving board
	
};

enum class MARBLECOLOR
{
	BLACK = 0,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	PURPLE,
};

class marble
{
private:
	bool		m_active;
	bool		m_toberemoved;

	CVEC2		m_position;
	CVEC2		m_displaypos;
	CVEC2		m_motion;

	MARBLESTATE	m_state;
	MARBLECOLOR	m_color;

	int			m_movecounter;

public:

	marble();
	marble(const marble &obj);
	~marble();

	void operator=(const marble &obj);

	void initialize(MARBLECOLOR color, CVEC2 pos);
	void update();

	CVEC2	getposition()		{	return m_position;		}
	CVEC2	getdisplaypos()		{	return m_displaypos;	}
	CVEC2	getmotion()			{	return m_motion;		}
	MARBLESTATE getstate()		{	return m_state;			}
	MARBLECOLOR getcolor()		{	return m_color;			}

	int getmovecounter()		{	return m_movecounter;	}

	bool isactive()				{	return m_active;		}
	bool istoberemoved()		{	return m_toberemoved;	}

	void setposition(CVEC2 pos)	{	m_position = pos;	}
	void setdisplaypos(CVEC2 pos)	{	m_displaypos = pos;	}
	void setmotion(CVEC2 mot)		{	m_motion = mot;		}
	void setstate(MARBLESTATE state){	m_state = state;	}
	void setcolor(MARBLECOLOR color){	m_color = color;	}
	void setmovecounter(int count)	{	m_movecounter = count; }

	void setactive(bool active)		{	m_active = active;	}
	void setremoval(bool remove)	{	m_toberemoved = remove; }

	void addmarble(MARBLECOLOR color, CVEC2 pos, CVEC2 mot);
	bool dropmarble(MARBLECOLOR color, CVEC2 pos, CVEC2 displaypos);
	void remove();

	void stop();

};

#endif



