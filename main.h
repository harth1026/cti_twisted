#ifndef MRSDL_MAIN_H
#define MRSDL_MAIN_H

#include <libcti/libcti.h>

#include <string.h>
#include <stdio.h>

//#include "medialayer.h"
#include "game.h"
#include "menu.h"




//medialayer ml;

menu mymenu;
game mygame;


//const int FRAMERATE = 25;


// TITLE
// MENU

// GAME


bool	g_quit		= false;

bool	g_pause		= false;
bool	g_gamestart = false;

bool	g_escb_up	= true;
bool	g_retb_up	= true;
bool	g_upb_up	= true;
bool	g_downb_up	= true;
bool	g_leftb_up	= true;
bool	g_rightb_up	= true;
bool	g_pb_up		= true;

int		g_vol_music	= 60;
int		g_vol_sound = 60;


#endif
