#pragma once
#include <SDL.h>

class pipe
{
	enum PipeType
	{
		PT_Normal,
		PT_Run
	};

public:
	pipe();
	~pipe();
	void renderPipe(SDL_Renderer*& renderer);
	void setRect(SDL_Rect& rect1, SDL_Rect& rect2);
	void updateRect();
	bool checkBirdIncident(SDL_Rect& bird);
	void checkScore(int& score);
private:
	SDL_Rect rect1, rect2;
	SDL_Surface* surfacePipe1, * surfacePipe2;
	SDL_Texture* texPipe1, * texPipe2;

	bool plus;
	
	bool scored;
	PipeType type;
};
int random(int a, int b);

