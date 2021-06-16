 #include "main.h"
#include "stage.h"

#define TWISTED_VERSION		"0.5.0"

bool	stage_init(STAGE stage);
STAGE	stage_update(STAGE stage, int timepoint);
int		stage_event(STAGE stage);
bool	stage_draw(STAGE stage);
bool	stage_exit(STAGE stage);

bool stage_init(STAGE stage)
{
	bool result = false;

	switch(stage)
	{
	case STAGE::MENU:
		result = mymenu.initialize();
		break;
	case STAGE::GAME:
		result = mygame.initialize(AI_LEVEL::HUMAN, mymenu.getAI2());
//		result = mygame.initialize(AI_LEVEL::HUMAN, AI_LEVEL::ERIK);
		break;
	}

	return result;
}

STAGE stage_update(STAGE stage, int timepoint)
{
	STAGE result = STAGE::QUIT;

	switch(stage)
	{
	case STAGE::MENU:
		result = mymenu.update(timepoint);
		break;
	case STAGE::GAME:
		result = mygame.update(timepoint);
		break;
	}

	return result;
}

int stage_event(STAGE stage)
{
	bool result = false;
	int response = 0;

	switch(stage)
	{
	case STAGE::MENU:
		result = mymenu.handle_event(&response);
		break;
	case STAGE::GAME:
		result = mygame.handle_event(&response);
		break;
	}

	return response;
}

int stage_keydown(STAGE stage)
{
	bool result = false;
	EVENTKEY keycode;
	cti_geteventkey(&keycode);


	switch (stage)
	{
	case STAGE::MENU:
		result = mymenu.handle_keydown(keycode);
		break;
	case STAGE::GAME:
		result = mygame.handle_keydown(keycode);
		break;
	}

	return result;
}

int stage_mouseposition(STAGE stage)
{
	bool result = false;
	CVEC2 mousepos;
	cti_getmouseposition(&mousepos);

	switch (stage)
	{
	case STAGE::MENU:
		result = mymenu.handle_mousemove(mousepos);
		break;
	case STAGE::GAME:
		result = mygame.handle_mousemove(mousepos);
		break;
	}

	return result;
}

int stage_mousebuttondown(STAGE stage)
{
	bool result = false;
	CVEC2 mousepos;
	cti_getmouseposition(&mousepos);

	switch (stage)
	{
	case STAGE::MENU:
		result = mymenu.handle_mousebuttondown(mousepos);
		break;
	case STAGE::GAME:
		result = mygame.handle_mousebuttondown(mousepos);
		break;
	}

	return result;
}

bool stage_draw(STAGE stage)
{
	bool result = false;
	cti_cleardisplay();

	CVEC2 size;
	cti_getscreensize(&size);
	switch(stage)
	{
	case STAGE::MENU:
		result = mymenu.draw();

		char versiontext[32];
		sprintf_s(versiontext, 32, "Version: %s", TWISTED_VERSION);
		cti_settexttodisplay(versiontext, CVEC2(size.cx / 2, size.cy - 100), CCOLOR(255,255,255));
		break;
	case STAGE::GAME:
		result = mygame.draw();
		break;
	}

	return cti_displayimages();
}

bool stage_exit(STAGE stage)
{
	bool result = false;

	switch(stage)
	{
	case STAGE::MENU:
		result = mymenu.uninitialize();
		break;
	case STAGE::GAME:
		result = mygame.uninitialize();
		break;
	}

	return result;
}



bool initialize()
{
	return cti_initialize("Twisted", 1280, 800);
}

bool uninitialize()
{
	return cti_uninitialize();
}

int main( int argc, char* args[] )
{
	STAGE stage 		= STAGE::MENU;
	STAGE stage_next	= STAGE::MENU;

	EVENTTYPE et;
	int response=0;

	g_quit = false;

	int time_current = 0;
	int time_next = 0;

	if(initialize())
	{
		if(stage_init(stage))
		{
			while(!g_quit)
			{
				while (cti_pollevent(&et))
				{
					switch (et)
					{
					case EVENTTYPE::E_KEYDOWN:
						stage_keydown(stage);
						break;
					case EVENTTYPE::E_MOUSEMOTION:
						stage_mouseposition(stage);
						break;
					case EVENTTYPE::E_MOUSEBUTTONDOWN:
						stage_mousebuttondown(stage);
						break;
					}
				}

				time_current = cti_getticks();
				if (time_current >= time_next)
				{
					time_next = time_current + 30;
					stage_next = stage_update(stage, cti_getticks());
					stage_draw(stage);

					if (stage != stage_next)
					{
						stage_exit(stage);
						if (stage_next == STAGE::QUIT)
						{
							g_quit = true;
						}
						else if (stage_init(stage_next))
						{
							stage = stage_next;
						}
					}
				}
			}
		}
		uninitialize();
	}

	return 0;
}

