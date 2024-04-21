#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <mutex>
#include <vector>
#include <sstream>
#include <string>
#include "pipe.h"

const int SCREEN_WIDTH = 680;
const int SCREEN_HEIGHT = 480;
const int PIPE_WIDTH = 50;
const int TIMER_INTERVAL = 20;


SDL_TimerID timerID = 0;

// Tạo cửa sổ
SDL_Window* window = NULL;
// Tạo renderer
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;

SDL_Rect birdRect = { SCREEN_WIDTH / 4 , SCREEN_HEIGHT / 4 , 50, 50 };
const SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
const SDL_Rect scoreRect = { 0, 0, 200, 100 };

SDL_Surface* birdSurface = NULL;
SDL_Texture* birdTexture = NULL;
SDL_Surface* backgroundSurface = NULL;
SDL_Texture* backgroundTexture = NULL;

std::vector<pipe*> listPipe;
std::mutex mutex;
int score = 0;

// Button
struct Button {
	SDL_Rect rect; // Position
	SDL_Color color;
	std::string text;
};

void drawButton(SDL_Renderer* renderer, const Button& button, TTF_Font* font)
{
	SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, button.color.a);
	SDL_RenderFillRect(renderer, &button.rect);

	SDL_Surface* surface = TTF_RenderText_Solid(font, button.text.c_str(), { 255, 255, 255, 255 });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect textRect;
	textRect.x = button.rect.x + (button.rect.w - surface->w) / 2;
	textRect.y = button.rect.y + (button.rect.h - surface->h) / 2;
	textRect.w = surface->w;
	textRect.h = surface->h;

	SDL_RenderCopy(renderer, texture, NULL, &textRect);
	SDL_DestroyTexture(texture);
}
//void drawScore(SDL_Renderer* renderer, TTF_Font* font, const std::string& score)
//{
//	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//	SDL_RenderFillRect(renderer, &scoreRect);
//
//	SDL_Surface* surface = TTF_RenderText_Solid(font, score.c_str(), { 255, 255, 255, 255 });
//	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
//	SDL_FreeSurface(surface);
//
//	SDL_Color foregroundColor = { 255, 255, 255 };
//	SDL_Color backgroundColor = { 0, 0, 255 };
//
//	SDL_Surface* textSurface
//		= TTF_RenderText_Shaded
//		(font, score.c_str(), foregroundColor, backgroundColor);
//
//	SDL_Rect textLocation = { 100, 100, 0, 0 };
//
//	SDL_FillRect(backgroundSurface, NULL, SDL_MapRGB(backgroundSurface->format, 0, 0, 0));
//	 
//	SDL_BlitSurface(textSurface, NULL, backgroundSurface, &textLocation);
//
//	SDL_FreeSurface(textSurface);
//
//	SDL_Rect textRect;
//	textRect.x = (scoreRect.w - surface->w) / 2;
//	textRect.y = (scoreRect.h - surface->h) / 2;
//	textRect.w = 2;
//	textRect.h = 4;
//
//
//	SDL_RenderCopy(renderer, texture, NULL, &textRect);
//	SDL_DestroyTexture(texture);
//}


void sendCustomEvent() {
	SDL_Event event;
	event.type = SDL_USEREVENT; // Đặt loại sự kiện là tùy chỉnh
	event.user.code = 0; // Mã sự kiện tùy chỉnh
	event.user.data1 = nullptr;
	event.user.data2 = nullptr;
	SDL_PushEvent(&event); // Đẩy sự kiện vào hàng đợi
}


void handleEventSpace(SDL_Renderer* renderer, SDL_Rect& rectangle)
{
	mutex.lock();
	rectangle.y -= 40;
	SDL_RenderCopy(renderer, birdTexture, NULL, &rectangle);
	mutex.unlock();
}


//void viewPausePopup()
//{
//
//}

//bool showRestartPopup() {
//	// Render a simple pop-up asking to restart
//	SDL_RemoveTimer(timerID);
//	Button button;
//	button.rect = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 50 }; // Vị trí và kích thước của button
//	button.color = { 255, 0, 0, 255 }; // Màu sắc của button
//	button.text = "RESTART"; // Text của button
//	drawButton(renderer, button, font);
//	Button button2;
//	button2.rect = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 150, 200, 50 }; // Vị trí và kích thước của button
//	button2.color = { 255, 0, 0, 255 }; // Màu sắc của button
//	button2.text = "QUIT"; // Text của button
//	drawButton(renderer, button, font);
//	drawButton(renderer, button2, font);
//	/*SDL_Rect popupRect = { 200, 200, 400, 200 };
//	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
//	SDL_RenderFillRect(gRenderer, &popupRect);
//
//	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
//	SDL_RenderDrawRect(gRenderer, &popupRect);*/
//	
//
//	// Render text
//	// You can use SDL_ttf library to render text here
//
//	// Update screen
//	SDL_RenderPresent(gRenderer);
//
//	// Wait for player's response
//	SDL_Event e;
//	//while (SDL_WaitEvent(&e)) {
//	//	if (e.type == SDL_KEYDOWN) {
//	//		if (e.key.keysym.sym == SDLK_r) {
//	//			// Player chose to restart
//	//			std::cout << "TRUE" << std::endl;
//	//			return true;
//	//		}
//	//		else if (e.key.keysym.sym == SDLK_ESCAPE) {
//	//			// Player chose to quit
//	//			std::cout << "FALSE" << std::endl;
//	//			return false;
//	//		}
//	//	}
//	//	else if (e.type == SDL_QUIT) {
//	//		// Player closed the window
//	//		return false;
//	//	}
//	//}
//
//	return false;
//}



//
//SDL_Window* gWindow = NULL;
//SDL_Renderer* gRenderer = NULL;
//TTF_Font* gFont = NULL;



//void renderScore(int score) {
//	// Tạo surface từ số điểm
//	SDL_Color textColor = { 255, 255, 255 }; // Màu trắng
//	std::string scoreText = "Score: " + std::to_string(score);
//	gFont = TTF_OpenFont("Arian.ttf", 28);
//	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, scoreText.c_str(), textColor);
//	if (textSurface == nullptr) {
//		std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
//		return;
//	}
//
//	// Tạo texture từ surface
//	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
//	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
//	if (textTexture == nullptr) {
//		std::cerr << "Unable to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;
//		SDL_FreeSurface(textSurface);
//		return;
//	}
//
//	// Lấy kích thước của texture
//	int textWidth = 0, textHeight = 0;
//	SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
//
//	// Vị trí để vẽ văn bản trên màn hình (ở giữa trên cùng)
//	int x = (800 - textWidth) / 2;
//	int y = 20;
//
//	// Vẽ văn bản lên renderer
//	SDL_Rect dstRect = { x, y, textWidth, textHeight };
//	SDL_RenderCopy(gRenderer, textTexture, nullptr, &dstRect);
//
//	// Giải phóng bộ nhớ
//	SDL_FreeSurface(textSurface);
//	SDL_DestroyTexture(textTexture);
//}


Uint32 update(Uint32 interval, void* param)
{
	mutex.lock();
	SDL_RenderClear(renderer);

	// Update BACKGROUND
	SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

	// Bird go down
	birdRect.y += 2;
	SDL_RenderCopy(renderer, birdTexture, NULL, &birdRect);

	// Update listPipe
	for (int i = 0; i < listPipe.size(); i++)
	{
		
		listPipe[i]->updateRect();
		listPipe[i]->renderPipe(renderer);
		if (listPipe[i]->checkBirdIncident(birdRect))
		{
			
			sendCustomEvent();
			break;
		}
		listPipe[i]->checkScore(score);
	}

	SDL_RenderPresent(renderer);

	mutex.unlock();
	return interval; // Trả về thời gian tiếp theo muốn kích hoạt timer (miligiây)
}



int main(int argc, char* args[])
{
	// Khởi tạo SDL và kiểm tra nếu có lỗi
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("SDL Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	birdSurface = IMG_Load("bird.png");
	if (birdSurface == nullptr) {
		std::cerr << "Cannot load image for bird! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	birdTexture = SDL_CreateTextureFromSurface(renderer, birdSurface);

	backgroundSurface = IMG_Load("background.jpg");
	if (backgroundSurface == nullptr) {
		std::cerr << "Cannot load background! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);

	SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

	// Prepare pipes
	pipe* p1 = new pipe();
	pipe* p2 = new pipe();
	pipe* p3 = new pipe();
	
	listPipe.push_back(p1);
	listPipe.push_back(p2);
	listPipe.push_back(p3);

	

	for (int i = 0; i < listPipe.size(); i++)
	{
		SDL_Rect rect1 = { SCREEN_WIDTH + i*253, 0 , 50,  random(80, 220) }, rect2 = { SCREEN_WIDTH + i*253, rect1.h + 200, 50, SCREEN_HEIGHT - rect2.y};
		listPipe[i]->setRect(rect1, rect2);
		listPipe[i]->renderPipe(renderer);
	}

	font = TTF_OpenFont("Arian.ttf", 24); // Load font
	Button button;
	button.rect = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 100 }; // Vị trí và kích thước của button
	button.color = { 255, 0, 0, 255 }; // Màu sắc của button
	button.text = "START"; // Text của button

	drawButton(renderer, button, font);
	SDL_RenderPresent(renderer);
	
	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				handleEventSpace(renderer, birdRect);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			if (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w &&
				mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h)
			{
				timerID = SDL_AddTimer(TIMER_INTERVAL, update, NULL);
			}
		}
			break;
		case SDL_USEREVENT:
		case SDL_QUIT:
			quit = true;
			break;
		}
		SDL_Delay(20);
	}

	// Giải phóng bộ nhớ và thoát SDL
	if (timerID)
	{
		SDL_RemoveTimer(timerID);
	}
	if (birdSurface)
	{
		SDL_FreeSurface(birdSurface);
	}
	if (backgroundSurface)
	{
		SDL_FreeSurface(backgroundSurface);
	}
	if (birdTexture)
	{
		SDL_DestroyTexture(birdTexture);
	}
	if (backgroundTexture)
	{
		SDL_DestroyTexture(backgroundTexture);
	}
	if (font)
	{
		TTF_CloseFont(font);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	delete p1;
	delete p2;
	delete p3;
	IMG_Quit();
	SDL_Quit();
	return 0;
}
