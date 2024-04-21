	#include "pipe.h"
	#include <iostream>
	#include <SDL_image.h>
	#include <random>

	static const int spaceTopDown = 200;
	static const int birdPosition = 680 / 4;
	static const int maxHeightPipe1 = 220;
	static const int minHeightPipe1 = 80;
	static const int SCREEN_WIDTH = 680;
	static const int SCREEN_HEIGHT = 480;
	static const int PIPE_WIDTH = 50;

	static int count = 0;

	// To random number
	static std::random_device rd; // obtain a random number from hardware
	static std::mt19937 gen(rd()); // seed the generator
	

	int random(int a, int b)
	{
		std::uniform_int_distribution<> distr(a, b); // define the range
		return distr(gen);
	}

	pipe::pipe() {
		scored = false;
		plus = false;
		type = PT_Normal;
		texPipe1 = nullptr;
		texPipe2 = nullptr;
		surfacePipe1 = IMG_Load("pipe1.png");
		surfacePipe2 = IMG_Load("pipe2.png");
		rect1 = { SCREEN_WIDTH, 0, PIPE_WIDTH, random(minHeightPipe1, maxHeightPipe1) };
		rect2 = { SCREEN_WIDTH, rect1.h + spaceTopDown, PIPE_WIDTH, SCREEN_HEIGHT - rect2.y };
	}

	pipe::~pipe()
	{
		if (surfacePipe1)
		{
			SDL_FreeSurface(surfacePipe1);
		}
		if (surfacePipe2)
		{
			SDL_FreeSurface(surfacePipe2);
		}
		if (texPipe1)
		{
			SDL_DestroyTexture(texPipe1);
		}
		if (texPipe2)
		{
			SDL_DestroyTexture(texPipe2);
		}
	}

	void pipe::setRect(SDL_Rect& rec1, SDL_Rect& rec2)
	{
		rect1 = rec1;
		rect2 = rec2;
	}

	void pipe::updateRect()
	{
	if (type == PT_Normal)
		{
			rect1.x -= 5;
			rect2.x -= 5;
		}
	else
		{
			rect1.x -= 5;
			rect2.x -= 5;
			if (count == 50)
			{
				count = 0;
				
				if (plus == false) {
					rect1.h += 50;
					
					plus = true;
				}
				else {
					plus = false;
					rect1.h -= 50;
				}
				
				rect2.y = rect1.h + spaceTopDown;
				rect2.h = SCREEN_HEIGHT - rect2.y;
			}
			else
			{
				count++;
			}
		}
		// Check and go to begin position
		if (rect1.x < -PIPE_WIDTH)
		{
			// random new type, to run pipe on vertical
 			int ran = random(1,3);
			if (ran == 3) {
				type = PT_Run;
			}
			else {
				type = PT_Normal;
			}
			// Update to begin position
			scored = false;
			rect1.x = SCREEN_WIDTH;
			rect2.x = SCREEN_WIDTH;
			rect1.h = random(minHeightPipe1, maxHeightPipe1);
			rect2.y = rect1.h + spaceTopDown;
			rect2.h = SCREEN_HEIGHT - rect2.y;
		}
	}

	void pipe::renderPipe(SDL_Renderer*& renderer)
	{
		
		if (texPipe1 == NULL)
		{
			texPipe1 = SDL_CreateTextureFromSurface(renderer, surfacePipe1);
			texPipe2 = SDL_CreateTextureFromSurface(renderer, surfacePipe2);
		}

		SDL_RenderCopy(renderer, texPipe1, NULL, &rect1);
		SDL_RenderCopy(renderer, texPipe2, NULL, &rect2);	
	}

	bool pipe::checkBirdIncident(SDL_Rect& bird)
	{
		if (bird.y < 0 || bird.y > SCREEN_WIDTH)
		{
			return true;
		}
		if ((bird.x + bird.w > rect1.x && bird.x < rect1.x + rect1.w)
			&& (bird.y < rect1.h || bird.y + bird.h > rect1.h + spaceTopDown))
		{
			return true;
		}

		return false;
	}

	void pipe::checkScore(int& score)
	{
		if (!scored && rect1.x < birdPosition)
		{
			score++;
			scored = true;
			std::cout << "score: " << score << std::endl;
		}
	}
