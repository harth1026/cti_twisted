#include "menu.h"

menu::menu()
{
	m_quit = false;
	m_play = false;
	m_ai1 = AI_LEVEL::HUMAN;	// HUMAN
	m_ai2 = AI_LEVEL::JACK;	// JACK
	menustate = MENUSTATE::MAIN;

	m_bgipos_x = 0;
	m_bgipos_y = 0;

	img_bgi = nullptr;
	img_title = nullptr;
	img_menuback = nullptr;

	img_menumain_play = nullptr;
	img_menumain_opts = nullptr;
	img_menumain_exit = nullptr;

	img_menuplay_jack = nullptr;
	img_menuplay_erik = nullptr;
	img_menuplay_nini = nullptr;
	img_menuplay_sven = nullptr;
	img_menuplay_judd = nullptr;
	img_menuplay_anna = nullptr;
	img_menuplay_vlad = nullptr;

	img_menuopts_music = nullptr;
	img_menuopts_effects = nullptr;
	img_menuopts_volume = nullptr;
	img_menuopts_back = nullptr;
	
	wav_titlemusic = nullptr;
}

menu::~menu()
{
}

bool menu::initialize()
{
	bool result = false;
	m_quit = false;
	m_play = false;

	cti_addimage("pics/mrbg.bmp", &img_bgi, false);
	cti_addimage("pics/bgi_title.bmp", &img_title, false);
	cti_addimage("pics/bgi_menu.bmp", &img_menuback, false);

	cti_addimage("pics/menumain_play.bmp", &img_menumain_play, false);
	cti_addimage("pics/menumain_options.bmp", &img_menumain_opts, false);
	cti_addimage("pics/menumain_exit.bmp", &img_menumain_exit, false);

	cti_addimage("pics/menuplay_jack.bmp", &img_menuplay_jack, false);
	cti_addimage("pics/menuplay_erik.bmp", &img_menuplay_erik, false);
	cti_addimage("pics/menuplay_nini.bmp", &img_menuplay_nini, false);
	cti_addimage("pics/menuplay_sven.bmp", &img_menuplay_sven, false);
	cti_addimage("pics/menuplay_judd.bmp", &img_menuplay_judd, false);
	cti_addimage("pics/menuplay_anna.bmp", &img_menuplay_anna, false);
	cti_addimage("pics/menuplay_vlad.bmp", &img_menuplay_vlad, false);

	cti_addimage("pics/menuopts_music.bmp", &img_menuopts_music, false);
	cti_addimage("pics/menuopts_effects.bmp", &img_menuopts_effects, false);
	cti_addimage("pics/menuopts_volume.bmp", &img_menuopts_volume, false);
	cti_addimage("pics/menuopts_back.bmp", &img_menuopts_back, false);

	if(img_bgi &&
		img_title &&
		img_menuback &&
		img_menumain_play &&
		img_menumain_opts &&
		img_menumain_exit &&
		img_menuplay_jack &&
		img_menuplay_erik &&
		img_menuplay_nini &&
		img_menuplay_sven &&
		img_menuplay_judd &&
		img_menuplay_anna &&
		img_menuplay_vlad &&
		img_menuopts_music &&
		img_menuopts_effects &&
		img_menuopts_volume &&
		img_menuopts_back)
	{
		result = true;
	}


#if USE_AUDIO
	m_ml->loadmusic("audio/slyhome.mod");
	m_ml->playmusic();
#endif

	m_bgipos_x = -576;
	m_bgipos_y = 0;

	menustate = MENUSTATE::TITLE;
	return result;
}

bool menu::uninitialize()
{
	cti_stopmusic();
	cti_freeallimages();
	cti_freeallmusic();
	img_bgi = NULL;
	img_title = NULL;
	img_menuback = NULL;

	img_menumain_play = NULL;
	img_menumain_opts = NULL;
	img_menumain_exit = NULL;

	img_menuplay_jack = NULL;
	img_menuplay_erik = NULL;
	img_menuplay_nini = NULL;
	img_menuplay_sven = NULL;
	img_menuplay_judd = NULL;
	img_menuplay_anna = NULL;
	img_menuplay_vlad = NULL;

	img_menuopts_music = NULL;
	img_menuopts_effects = NULL;
	img_menuopts_volume = NULL;
	img_menuopts_back = NULL;

	return true;
}

AI_LEVEL menu::getAI1()
{
	return m_ai1;
}

AI_LEVEL menu::getAI2()
{
	return m_ai2;
}

STAGE menu::update(int timepoint)
{
	STAGE nextstage = STAGE::MENU;
	m_bgipos_x += 2;
	m_bgipos_y -= 1;

	if (m_bgipos_x > 0)
		m_bgipos_x -= 576;
	if (m_bgipos_y < -576)
		m_bgipos_y += 576;

#if USE_AUDIO
	if (!m_ml->ismusicplaying())
	{
		m_ml->loadmusic("audio/slyhome.mod");
		m_ml->playmusic();
	}
#endif

	if (m_quit)
		nextstage = STAGE::QUIT;

	if (m_play)
		nextstage = STAGE::GAME;

	return nextstage;
}

bool menu::handle_event(int* response)
{
//	*response = 0;
//	if (m_ml->isescapepressed())
//		*response = -1;

	return true;
}

bool menu::handle_keydown(EVENTKEY keycode)
{
	switch (keycode)
	{
	case EVENTKEY::K_ESCAPE:
		m_quit = true;
		break;

	case EVENTKEY::K_RETURN:
		switch (menustate)
		{
		case MENUSTATE::TITLE:
			menustate = MENUSTATE::MAIN;
			break;

		case MENUSTATE::MAIN:
			m_play = true;
			break;
		}
		break;
	}
	return true;
}

bool menu::handle_mousemove(CVEC2 mouse)
{
	return true;
}

bool menu::handle_mousebuttondown(CVEC2 mouse)
{
	CVEC2 size;
	cti_getscreensize(&size);
	int origin_x = size.cx / 2 - 350;		// maybe do this more dynamically
	int origin_y = size.cy / 2 - 250;

	switch (menustate)
	{
	case MENUSTATE::TITLE:
		menustate = MENUSTATE::MAIN;
		break;

	case MENUSTATE::MAIN:
		if ((mouse.cx >= origin_x + 222) && (mouse.cx < origin_x + 478))
		{
			if ((mouse.cy >= origin_y + 85) && (mouse.cy < origin_y + 135))
			{
				menustate = MENUSTATE::COMPUTERS;
			}

			if ((mouse.cy >= origin_y + 220) && (mouse.cy < origin_y + 270))
			{
				menustate = MENUSTATE::OPTIONS;
			}

			if ((mouse.cy >= origin_y + 355) && (mouse.cy < origin_y + 405))
			{
				m_quit = true;
			}
		}
		break;

	case MENUSTATE::COMPUTERS:

		if ((mouse.cx >= origin_x + 120) && (mouse.cx < origin_x + 300))
		{
			if ((mouse.cy >= origin_y + 60) && (mouse.cy < origin_y + 110))
			{
				// AI_JACK,
				m_ai2 = AI_LEVEL::JACK;
				m_play = true;
			}
			if ((mouse.cy >= origin_y + 170) && (mouse.cy < origin_y + 220))
			{
				// AI_NINI,		// Unicorn Girl
				m_ai2 = AI_LEVEL::NINI;
				m_play = true;
			}
			if ((mouse.cy >= origin_y + 280) && (mouse.cy < origin_y + 330))
			{
				// AI_JUDD,		// Ice Cream Truck Driver
				m_ai2 = AI_LEVEL::JUDD;
				m_play = true;
			}
		}

		if ((mouse.cx >= origin_x + 400) && (mouse.cx < origin_x + 580))
		{
			if ((mouse.cy >= origin_y + 60) && (mouse.cy < origin_y + 110))
			{
				// AI_ERIK,		// Pinoy Viking
				m_ai2 = AI_LEVEL::ERIK;
				m_play = true;
			}
			if ((mouse.cy >= origin_y + 170) && (mouse.cy < origin_y + 220))
			{
				// AI_SVEN,		// Lion Knight
				m_ai2 = AI_LEVEL::SVEN;
				m_play = true;
			}
			if ((mouse.cy >= origin_y + 280) && (mouse.cy < origin_y + 330))
			{
				// AI_ANNA,		// Rock Star
				m_ai2 = AI_LEVEL::ANNA;
				m_play = true;
			}
		}

		if ((mouse.cx >= origin_x + 260) && (mouse.cx < origin_x + 440))
		{
			if ((mouse.cy >= origin_y + 390) && (mouse.cy < origin_y + 440))
			{
				// AI_VLAD,		// Spearman
				m_ai2 = AI_LEVEL::VLAD;
				m_play = true;
			}
		}

		break;

	case MENUSTATE::OPTIONS:
		if ((mouse.cx >= origin_x + 260) && (mouse.cx < origin_x + 440))
		{
			if ((mouse.cy >= origin_y + 390) && (mouse.cy < origin_y + 440))
			{
				//back
				menustate = MENUSTATE::MAIN;
			}
		}
		break;

	}
	return true;
}

bool menu::draw()
{
	bool result = false;
	CVEC2 size;
	cti_getscreensize(&size);

	// menu background
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			cti_setimagetodisplay(img_bgi, CVEC2(m_bgipos_x+576*x, m_bgipos_y+576*y));
		}
	}

	int origin_x = size.cx / 2 - 350;		// maybe do this more dynamically
	int origin_y = size.cy / 2 - 250;

	switch (menustate)
	{
	case MENUSTATE::TITLE:
		cti_setimagetodisplay(img_title, CVEC2(origin_x, size.cy / 2 - 100));
		break;

	case MENUSTATE::MAIN:
		cti_setimagetodisplay(img_menuback, CVEC2(origin_x, origin_y));
		cti_setimagetodisplay(img_menumain_play, CVEC2(origin_x + 222, origin_y + 85));
		cti_setimagetodisplay(img_menumain_opts, CVEC2(origin_x + 222, origin_y + 220));
		cti_setimagetodisplay(img_menumain_exit, CVEC2(origin_x + 222, origin_y + 355));

		break;

	case MENUSTATE::COMPUTERS:
		cti_setimagetodisplay(img_menuback, CVEC2(origin_x, origin_y));
		cti_setimagetodisplay(img_menuplay_jack, CVEC2(origin_x + 120, origin_y + 60));
		cti_setimagetodisplay(img_menuplay_erik, CVEC2(origin_x + 400, origin_y + 60));
		cti_setimagetodisplay(img_menuplay_nini, CVEC2(origin_x + 120, origin_y + 170));
		cti_setimagetodisplay(img_menuplay_sven, CVEC2(origin_x + 400, origin_y + 170));
		cti_setimagetodisplay(img_menuplay_judd, CVEC2(origin_x + 120, origin_y + 280));
		cti_setimagetodisplay(img_menuplay_anna, CVEC2(origin_x + 400, origin_y + 280));
		cti_setimagetodisplay(img_menuplay_vlad, CVEC2(origin_x + 260, origin_y + 390));
		break;

	case MENUSTATE::OPTIONS:
		cti_setimagetodisplay(img_menuback, CVEC2(origin_x, origin_y));
		cti_setimagetodisplay(img_menuopts_volume, CVEC2(origin_x + 222, origin_y + 60));
		cti_setimagetodisplay(img_menuopts_music, CVEC2(origin_x + 222, origin_y + 170));
		cti_setimagetodisplay(img_menuopts_effects, CVEC2(origin_x + 222, origin_y + 280));
		cti_setimagetodisplay(img_menuopts_back, CVEC2(origin_x + 222, origin_y + 390));
		break;
	}
	return result;
}

