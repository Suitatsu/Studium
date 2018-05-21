#include "Game.h"
#include "GameState.h"


Game *game = nullptr;
Menue *menue = nullptr;
Splashscreen * splash = nullptr;

GameState currentGameState = GameState::Splashscreen;



int main(int argc, char* argv[])
{

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;
	int nextState;

	game = new Game();
	menue = new Menue();
	splash = new Splashscreen();

	//Creating Game Window
	game->init("GameWindow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);



	while (game->running())
	{
		
		frameStart = SDL_GetTicks();
		switch (currentGameState)
		{
		case GameState::Game:
			game->handleEvents();
			nextState = game->update();
			game->render();
			
			switch (nextState)
			{
			case 1: currentGameState = GameState::Game; break;
			case 2: currentGameState = GameState::Menue; break;
			case 3: currentGameState = GameState::Splashscreen; break;
			case 4: currentGameState = GameState::End; break;
			default:
			break;
			}
			
			break;
		case GameState::Menue:
			nextState = menue->update();
			
			switch (nextState)
			{
			case 1: currentGameState = GameState::Game; break;
			case 2: currentGameState = GameState::Menue; break;
			case 3: currentGameState = GameState::Splashscreen; break;
			case 4: currentGameState = GameState::End; break;
			default:
				break;
			}
			
			break;
		case GameState::Splashscreen:
			
			nextState = splash->update();
			

			if (nextState == 1)
				currentGameState = GameState::Game;
			break;
		case GameState::End:
			game->clean();
			break;
		default:
			break;
			
		}
		

		frameTime = SDL_GetTicks() - frameStart; // How long does Events, update and render take

		// Limit frames
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	
	return 0;
}