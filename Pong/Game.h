#pragma once
#include "GameObjectManager.h"
#include "ScoreBoard.h"

class Game {
public:
	static void Start();
	static sf::RenderWindow& GetWindow();
	const static sf::Event& GetInput();
	static GameObjectManager& GetManager();
	static ScoreBoard& GetScoreBoard();
	static bool CheckVictory();

	const static int SCREEN_WIDTH = 1024;
	const static int SCREEN_HEIGHT = 768;
private:
	static bool isExiting();
	static void GameLoop();
	static void ShowSplashScreen();
	static void ShowMainMenu();
	static void ShowInGameMenu();
	static void ShowInGameMenu1();
	static void ShowEndGameMenu();
	static void ResetGame();
	static void ResetGame1();
	static void ShowVsMenu();
	static void ResetBackground();

	enum GameState { Uninitialized, ShowingSplash, Paused, Paused1, ShowingMenu, Playing, Playing1, Exiting, Completed, VSMenu };

	static GameState _gameState;
	static sf::RenderWindow _mainWindow;

	static GameObjectManager _gameObjectManager;
	static ScoreBoard _scoreBoard;

};