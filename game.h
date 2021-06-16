#ifndef TWISTED_GAME_H
#define TWISTED_GAME_H


#include "cluster.h"
#include "stage.h"

#define MATCHMULTIPLIER		3

enum class GAMESTATE
{
	GAME_START = 0,
	GAME_MAIN,
	GAME_PAUSE,
	GAME_WINNER,
};

class game
{
private:
	GAMESTATE	gamestate;
	int			timestart;
	int			winner;

	bool		m_quit;

	AI_LEVEL	ai_west;
	AI_LEVEL	ai_east;

	cluster		clust_west;
	cluster		clust_east;

	// images
	void*		img_marble[NUM_COLORS];
	void*		img_cursor;
	void*		img_back;
	void*		img_bannerwin;
	void*		img_bannerlose;

	void*		img_versus;
	void*		img_ai1name;
	void*		img_ai2name;

	// audio
	void*		wav_break01;

public:
	game();
	game(AI_LEVEL ai1, AI_LEVEL ai2);
	~game();

	bool	initialize(AI_LEVEL ai1, AI_LEVEL ai2);
	bool	uninitialize();

	STAGE	update(int timepoint);
	bool	handle_event(int* response);
	bool	handle_keydown(EVENTKEY keycode);
	bool	handle_mousemove(CVEC2 mouse);
	bool	handle_mousebuttondown(CVEC2 mouse);
	bool	draw();
};

#endif // GAME_H
