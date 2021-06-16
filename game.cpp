

#include <stdio.h>
#include <libcti/libcti.h>

#include "game.h"

game::game()
{
	ai_west = AI_LEVEL::HUMAN;
	ai_east = AI_LEVEL::HUMAN;
	winner = -1;
	m_quit = false;


	gamestate = GAMESTATE::GAME_START;
	timestart = 0;

	for (int i = 0; i < NUM_COLORS; i++)
		img_marble[i] = nullptr;
	img_cursor = nullptr;
	img_back = nullptr;
	img_bannerwin = nullptr;
	img_bannerlose = nullptr;

	img_versus = nullptr;
	img_ai1name = nullptr;
	img_ai2name = nullptr;

	wav_break01 = nullptr;
}

game::game(AI_LEVEL ai1, AI_LEVEL ai2)
{
	ai_west = ai1;
	ai_east = ai2;
	winner = -1;
	m_quit = false;

	gamestate = GAMESTATE::GAME_START;
	timestart = 0;

	for (int i = 0; i < NUM_COLORS; i++)
		img_marble[i] = nullptr;
	img_cursor = nullptr;
	img_back = nullptr;
	img_bannerwin = nullptr;
	img_bannerlose = nullptr;

	img_versus = nullptr;
	img_ai1name = nullptr;
	img_ai2name = nullptr;

	wav_break01 = nullptr;
}

game::~game()
{
}

bool game::initialize(AI_LEVEL ai1, AI_LEVEL ai2)
{
	ai_west = ai1;
	ai_east = ai2;
	winner = -1;
	m_quit = false;

	if (!clust_west.initialize(ai_west, CVEC2(128, 80)))
		return false;

	if (!clust_east.initialize(ai_east, CVEC2(766, 80)))
		return false;

	cti_addimage("pics/marble_black.bmp", &img_marble[0], true);
	cti_addimage("pics/marble_red.bmp", &img_marble[1], true);
	cti_addimage("pics/marble_green.bmp", &img_marble[2], true);
	cti_addimage("pics/marble_blue.bmp", &img_marble[3], true);
	cti_addimage("pics/marble_yellow.bmp", &img_marble[4], true);
	cti_addimage("pics/marble_purple.bmp", &img_marble[5], true);

	for (int i = 0; i < NUM_COLORS; i++)
	{
		if (img_marble[i] == NULL)
			return false;
	}

	cti_addimage("pics/cursor.bmp", &img_cursor, true);
	cti_addimage("pics/64black_space.bmp", &img_back, false);
	cti_addimage("pics/win.bmp", &img_bannerwin, true);
	cti_addimage("pics/lose.bmp", &img_bannerlose, true);
	cti_addimage("pics/versus.bmp", &img_versus, false);

	if (img_cursor == NULL || img_back == NULL ||
		img_bannerwin == NULL || img_bannerlose == NULL ||
		img_versus == NULL)
		return false;

	switch (ai1)
	{
	case AI_LEVEL::HUMAN:	cti_addimage("pics/menuplay_you.bmp",  &img_ai1name, false);	break;
	case AI_LEVEL::JACK:	cti_addimage("pics/menuplay_jack.bmp", &img_ai1name, false);	break;
	case AI_LEVEL::ERIK:	cti_addimage("pics/menuplay_erik.bmp", &img_ai1name, false);	break;
	case AI_LEVEL::NINI:	cti_addimage("pics/menuplay_nini.bmp", &img_ai1name, false);	break;
	case AI_LEVEL::SVEN:	cti_addimage("pics/menuplay_sven.bmp", &img_ai1name, false);	break;
	case AI_LEVEL::JUDD:	cti_addimage("pics/menuplay_judd.bmp", &img_ai1name, false);	break;
	case AI_LEVEL::ANNA:	cti_addimage("pics/menuplay_anna.bmp", &img_ai1name, false);	break;
	case AI_LEVEL::VLAD:	cti_addimage("pics/menuplay_vlad.bmp", &img_ai1name, false);	break;
	}

	switch (ai2)
	{
	case AI_LEVEL::HUMAN:	cti_addimage("pics/menuplay_you.bmp",  &img_ai2name, false);	break;
	case AI_LEVEL::JACK:	cti_addimage("pics/menuplay_jack.bmp", &img_ai2name, false);	break;
	case AI_LEVEL::ERIK:	cti_addimage("pics/menuplay_erik.bmp", &img_ai2name, false);	break;
	case AI_LEVEL::NINI:	cti_addimage("pics/menuplay_nini.bmp", &img_ai2name, false);	break;
	case AI_LEVEL::SVEN:	cti_addimage("pics/menuplay_sven.bmp", &img_ai2name, false);	break;
	case AI_LEVEL::JUDD:	cti_addimage("pics/menuplay_judd.bmp", &img_ai2name, false);	break;
	case AI_LEVEL::ANNA:	cti_addimage("pics/menuplay_anna.bmp", &img_ai2name, false);	break;
	case AI_LEVEL::VLAD:	cti_addimage("pics/menuplay_vlad.bmp", &img_ai2name, false);	break;
	}

	if (img_ai1name == NULL || img_ai2name == NULL)
		return false;


#if USE_AUDIO
	m_ml->addwave("audio/break01.wav",			&wav_break01);
	if (wav_break01 == NULL)
		return false;

	m_ml->loadmusic("audio/spathi.mod");
	m_ml->playmusic();
#endif

	timestart = cti_getticks() + 5000;
	gamestate = GAMESTATE::GAME_START;

	return true;
}

bool game::uninitialize()
{
	clust_west.uninitialize();
	clust_east.uninitialize();

	cti_freeallimages();
	for (int i = 0; i < NUM_COLORS; i++)
		img_marble[i] = NULL;
	img_cursor = NULL;
	img_back = NULL;
	img_bannerwin = NULL;
	img_bannerlose = NULL;

#if USE_AUDIO
	m_ml->freeallwaves();
	wav_break01 = NULL;

	m_ml->stopmusic();
	m_ml->freemusic();
#endif
	return true;
}

STAGE game::update(int timepoint)
{
	STAGE nextstage = STAGE::GAME;

	int westmatches = 0;
	int eastmatches = 0;
	int maxmarbles = 0;
	switch (gamestate)
	{
	case GAMESTATE::GAME_START:
		if (cti_getticks() >= timestart)
			gamestate = GAMESTATE::GAME_MAIN;
		break;

	case GAMESTATE::GAME_MAIN:

#if USE_AUDIO
		if (!m_ml->ismusicplaying())
		{
			m_ml->loadmusic("audio/spathi.mod");
			m_ml->playmusic();
		}
#endif

		if (!clust_west.update(timepoint, &westmatches))
			return STAGE::QUIT;

		if (!clust_east.update(timepoint, &eastmatches))
			return STAGE::QUIT;

		if (westmatches > 0)
		{
			clust_east.queuemarblestoadd(westmatches * MATCHMULTIPLIER);
#if USE_AUDIO
			m_ml->playwave(wav_break01, false);
#endif
		}

		if (eastmatches > 0)
		{
			clust_west.queuemarblestoadd(eastmatches * MATCHMULTIPLIER);
#if USE_AUDIO
			m_ml->playwave(wav_break01, false);
#endif
		}

		maxmarbles = NUM_ROWS * NUM_COLS;
		if (clust_west.updatecount() >= maxmarbles)
		{
			winner = 2;
			timestart = cti_getticks() + 5000;
			gamestate = GAMESTATE::GAME_WINNER;

#if USE_AUDIO
			m_ml->stopmusic();
#endif
		}
		else if (clust_east.updatecount() >= maxmarbles)
		{
			winner = 1;
			timestart = cti_getticks() + 5000;
			gamestate = GAMESTATE::GAME_WINNER;
#if USE_AUDIO
			m_ml->stopmusic();
#endif
		}
		break;

	case GAMESTATE::GAME_WINNER:

		if (cti_getticks() >= timestart)
		{
			nextstage = STAGE::MENU;
		}
		break;
	}

	if (m_quit)
		nextstage = STAGE::MENU;

	return nextstage;
}

bool game::handle_event(int* response)
{
//	*response = 0;
//	if (m_ml->isescapepressed())
//		*response = -1;

	return true;
}

bool game::handle_keydown(EVENTKEY keycode)
{
	switch (keycode)
	{
	case EVENTKEY::K_ESCAPE:
		m_quit = true;
		break;
	}
	return true;
}


bool game::handle_mousemove(CVEC2 mouse)
{
	if (gamestate == GAMESTATE::GAME_MAIN)
		return clust_west.handle_mousemove(mouse);
	else
		return true;
}

bool game::handle_mousebuttondown(CVEC2 mouse)
{
	if (gamestate == GAMESTATE::GAME_MAIN)
		return clust_west.handle_mousedownleft(mouse);
	else
		return true;
}


bool game::draw()
{
	int x, y;
	MARBLECOLOR color = MARBLECOLOR::BLACK;
	CVEC2 size;
	cti_getscreensize(&size);


	char temptext[4];

	CVEC2 pos;
	CVEC2 backpos;

	switch(gamestate)
	{
	case GAMESTATE::GAME_START:
		x = (int)((timestart - cti_getticks()) / 1000);
		sprintf_s(temptext, 4, "%d", x);
		pos = size / 2; 
		cti_settexttodisplay(temptext, pos, CCOLOR(255, 255,255));
		break;
	
	case GAMESTATE::GAME_MAIN:
		cti_setimagetodisplay(img_versus,  CVEC2(size.cx / 2 - 32, size.cy / 2 - 32));
		cti_setimagetodisplay(img_ai1name, CVEC2(size.cx / 2 - 90, size.cy / 2 - 82));
		cti_setimagetodisplay(img_ai2name, CVEC2(size.cx / 2 - 90, size.cy / 2 + 32));

		for (y = 0; y < NUM_ROWS; y++)
		{
			for (x = 0; x < NUM_COLS; x++)
			{
				// west cluster
				if (clust_west.mg[x][y].getstate() != MARBLESTATE::EMPTY)
				{
					if (clust_west.mg[x][y].getstate() == MARBLESTATE::READY)
					{
						backpos = clust_west.mg[x][y].getposition() + clust_west.gethomepos();
						cti_setimagetodisplay(img_back, backpos);
					}

					color = clust_west.mg[x][y].getcolor();
					pos = clust_west.mg[x][y].getdisplaypos() + clust_west.gethomepos();
					cti_setimagetodisplay(img_marble[(int)color], pos);
				}

				// east cluster
				if (clust_east.mg[x][y].getstate() != MARBLESTATE::EMPTY)
				{
					if (clust_east.mg[x][y].getstate() == MARBLESTATE::READY)
					{
						backpos = clust_east.mg[x][y].getposition() + clust_east.gethomepos();
						cti_setimagetodisplay(img_back, backpos);
					}

					color = clust_east.mg[x][y].getcolor();
					pos = clust_east.mg[x][y].getdisplaypos() + clust_east.gethomepos();
					cti_setimagetodisplay(img_marble[(int)color], pos);
				}
			}
		}

		// draw cursor
		if ((clust_west.getcursorpos() != CVEC2(-1, -1)) && clust_west.iscursortwistable())
			cti_setimagetodisplay(img_cursor, clust_west.getcursorpos());
		break;

	case GAMESTATE::GAME_WINNER:
		for (y = 0; y < NUM_ROWS; y++)
		{
			for (x = 0; x < NUM_COLS; x++)
			{
				// west cluster
				if (clust_west.mg[x][y].getstate() != MARBLESTATE::EMPTY)
				{
					if (clust_west.mg[x][y].getstate() == MARBLESTATE::READY)
					{
						backpos = clust_west.mg[x][y].getposition() + clust_west.gethomepos();
						cti_setimagetodisplay(img_back, backpos);
					}

					color = clust_west.mg[x][y].getcolor();
					pos = clust_west.mg[x][y].getdisplaypos() + clust_west.gethomepos();
					cti_setimagetodisplay(img_marble[(int)color], pos);
				}

				// east cluster
				if (clust_east.mg[x][y].getstate() != MARBLESTATE::EMPTY)
				{
					if (clust_east.mg[x][y].getstate() == MARBLESTATE::READY)
					{
						backpos = clust_east.mg[x][y].getposition() + clust_east.gethomepos();
						cti_setimagetodisplay(img_back, backpos);
					}

					color = clust_east.mg[x][y].getcolor();
					pos = clust_east.mg[x][y].getdisplaypos() + clust_east.gethomepos();
					cti_setimagetodisplay(img_marble[(int)color], pos);
				}
			}
		}

		CVEC2 bannersize;
		cti_getimagesize(img_bannerwin, &bannersize);

		if (winner == 1)
		{
			cti_setimagetodisplay(img_bannerwin,  CVEC2((clust_west.gethomepos().cx) + 64, (size.cy / 2) - (bannersize.cy / 2)));
			cti_setimagetodisplay(img_bannerlose, CVEC2((clust_east.gethomepos().cx) + 64, (size.cy / 2) - (bannersize.cy / 2)));
		}
		if (winner == 2)
		{
			cti_setimagetodisplay(img_bannerwin,  CVEC2((clust_east.gethomepos().cx) + 64, (size.cy / 2) - (bannersize.cy / 2)));
			cti_setimagetodisplay(img_bannerlose, CVEC2((clust_west.gethomepos().cx) + 64, (size.cy / 2) - (bannersize.cy / 2)));
		}
		break;
	}

	return true;
}

