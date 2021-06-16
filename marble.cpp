#include "marble.h"

marble::marble()
{
	m_active		= true;
	m_color			= MARBLECOLOR::BLACK;
	m_position		= CVEC2(0, 0);
	m_displaypos	= CVEC2(0, 0);
	m_motion		= CVEC2(0, 0);
	m_toberemoved	= false;
	m_state			= MARBLESTATE::EMPTY;
	
}

marble::marble(const marble &obj)
{
	m_active		= obj.m_active;
	m_color			= obj.m_color;
	m_position		= obj.m_position;
	m_displaypos	= obj.m_displaypos;
	m_motion		= obj.m_motion;
	m_toberemoved	= obj.m_toberemoved;
	m_state			= obj.m_state;
}

marble::~marble()
{
}

void marble::operator=(const marble &obj)
{
	m_active		= obj.m_active;
	m_color			= obj.m_color;
	m_position		= obj.m_position;
	m_displaypos	= obj.m_displaypos;
	m_motion		= obj.m_motion;
	m_toberemoved	= obj.m_toberemoved;
	m_state			= obj.m_state;
}

void marble::initialize(MARBLECOLOR color, CVEC2 pos)
{
	m_active		= true;
	m_color			= color;
	m_position		= pos;
	m_displaypos	= pos;
	m_motion		= CVEC2(0, 0);
	m_toberemoved	= false;
	m_state			= MARBLESTATE::EMPTY;
}

void marble::update()
{
	if(m_active)
	{
		if(m_state == MARBLESTATE::DROPS)
		{
			m_displaypos = m_displaypos + m_motion;
			if (m_displaypos.cy >= m_position.cy)
			{
				stop();
				m_displaypos.cy = m_position.cy;
				m_state = MARBLESTATE::READY;
			}
		}
		
		if (m_state == MARBLESTATE::TWIST)
		{
			m_displaypos = m_displaypos + m_motion;
			m_movecounter--;
			if(m_movecounter == 0)
			{
				stop();
				m_displaypos = m_position;
				m_state = MARBLESTATE::READY;

			}
		}
    }
}

// Adds new marbles to the top of the screen
void marble::addmarble(MARBLECOLOR color, CVEC2 pos, CVEC2 mot)
{
	m_active		= true;
	m_color			= color;
	m_position		= pos;
	m_displaypos 	= CVEC2(pos.cx, 0);
	m_motion		= CVEC2(0, MOVESPEED);
	m_toberemoved	= false;
	m_state			= MARBLESTATE::DROPS;
}

bool marble::dropmarble(MARBLECOLOR color, CVEC2 pos, CVEC2 displaypos)
{
	m_active		= true;
	m_color			= color;
	m_position		= pos;
	m_displaypos 	= CVEC2(pos.cx, 0);
	m_motion		= CVEC2(0, DROPSPEED);
	m_toberemoved	= false;
	m_state			= MARBLESTATE::DROPS;
	
	return true;
}

void marble::remove()
{
	m_active		= false;
	m_state			= MARBLESTATE::EMPTY;
}

void marble::stop()
{
	m_motion		= CVEC2(0, 0);
	m_state			= MARBLESTATE::READY;
}
