#ifndef TWISTED_MENU_H
#define TWISTED_MENU_H

//#include "medialayer.h"

#include <libcti/libcti.h>
#include "cluster.h"
#include "stage.h"

enum class MENUSTATE
{
	TITLE,
	MAIN,
	PLAYERS,
	COMPUTERS,
	OPTIONS,
};

class menu
{
private:
	bool m_quit;
	bool m_play;

	MENUSTATE menustate;

	AI_LEVEL m_ai1;
	AI_LEVEL m_ai2;

	int m_bgipos_x;
	int m_bgipos_y;

	// images
	CTI_IMAGE img_bgi;
	CTI_IMAGE img_title;
	CTI_IMAGE img_menuback;

	CTI_IMAGE img_menumain_play;
	CTI_IMAGE img_menumain_opts;
	CTI_IMAGE img_menumain_exit;

	CTI_IMAGE img_menuplay_jack;
	CTI_IMAGE img_menuplay_erik;
	CTI_IMAGE img_menuplay_nini;
	CTI_IMAGE img_menuplay_sven;
	CTI_IMAGE img_menuplay_judd;
	CTI_IMAGE img_menuplay_anna;
	CTI_IMAGE img_menuplay_vlad;

	CTI_IMAGE img_menuopts_music;
	CTI_IMAGE img_menuopts_effects;
	CTI_IMAGE img_menuopts_volume;
	CTI_IMAGE img_menuopts_back;

	// audio
	void*	wav_titlemusic;

public:
	menu();
	~menu();

	bool	initialize();
	bool	uninitialize();

	AI_LEVEL		getAI1();
	AI_LEVEL		getAI2();

	STAGE	update(int timepoint);
	bool	handle_event(int* response);
	bool	handle_keydown(EVENTKEY keycode);
	bool	handle_mousemove(CVEC2 mouse);
	bool	handle_mousebuttondown(CVEC2 mouse);
	bool	draw();
};

#endif
