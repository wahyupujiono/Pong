#include "stdafx.h"
#include "Game.h"
#include "GameObjectManager.h"
#include "SplashScreen.h"
#include "MainMenu.h"
#include "InGameMenu.h"
#include "EndGameMenu.h"
#include "PlayerPaddle.h"
#include "AIPaddle.h"
#include "GameBall.h"
#include "SFMLSoundProvider.h"
#include "ServiceLocator.h"
#include "ScoreBoard.h"
#include "VsGameMenu.h"
#include "PlayerPaddle2.h"

void Game::Start() {
	if (_gameState != Uninitialized)
		return;

	SFMLSoundProvider soundProvider;
	ServiceLocator::RegisterServiceLocator(&soundProvider);
	ServiceLocator::GetAudio()->PlaySong("audio/Soundtrack.ogg", true);

	//Set the background window
	_mainWindow.create(sf::VideoMode(1024, 768), "Pong!");
	_gameState = Game::ShowingSplash;

	PlayerPaddle *player1 = new PlayerPaddle();
	player1->SetDefaultPosition((SCREEN_WIDTH / 2), 700);
	_gameObjectManager.Add("Paddle1", player1);


	AIPaddle *player2 = new AIPaddle();
	player2->SetDefaultPosition((SCREEN_WIDTH / 2), 40);
	_gameObjectManager.Add("Paddle2", player2);

	PlayerPaddle2 *player3 = new PlayerPaddle2();
	player3->SetDefaultPosition((SCREEN_WIDTH / 2), 40);
	_gameObjectManager.Add("Paddle3", player3);

	GameBall *ball = new GameBall();
	ball->SetDefaultPosition((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2));
	_gameObjectManager.Add("Ball", ball);

	//The game loop
	while (!isExiting()) {
		GameLoop();
	}
}

sf::RenderWindow& Game::GetWindow() {
	return _mainWindow;
}

// Check for all the events that occured since the last frame
const sf::Event& Game::GetInput() {
	sf::Event event;
	_mainWindow.pollEvent(event);
	return event;
}

bool Game::isExiting() {
	if (_gameState == Game::Exiting)
		return true;
	else
		return false;
}

GameObjectManager& Game::GetManager() {
	return _gameObjectManager;
}

ScoreBoard& Game::GetScoreBoard() {
	return _scoreBoard;
}

void Game::GameLoop() {
	sf::Event event;
	srand(time(0));
	size_t R, G, B;

	switch (_gameState) {

	case Game::ShowingSplash:
		ShowSplashScreen();
		break;

	case Game::ShowingMenu:
		ShowMainMenu();
		break;

	case Game::Paused:
		ServiceLocator::GetAudio()->PlaySong("audio/Soundtrack.ogg", true);
		ShowInGameMenu();
		break;

	case Game::Paused1:
		ServiceLocator::GetAudio()->PlaySong("audio/Soundtrack.ogg", true);
		ShowInGameMenu1();
		break;

	case Game::VSMenu:
		ShowVsMenu();
		break;

	case Game::Playing:
		R = rand() % 255;
		G = rand() % 255;
		B = rand() % 255;
		_mainWindow.clear(sf::Color(R, G, B));
		_gameObjectManager.UpdateAll();
		_gameObjectManager.DrawAll(_mainWindow);
		ResetBackground();
		_gameObjectManager.Get("Paddle2");
		_gameObjectManager.Remove("Paddle3");
		_scoreBoard.Draw(_mainWindow);
		_mainWindow.display();

		while (_mainWindow.pollEvent(event)) {
			if (event.type == sf::Event::EventType::Closed) {
				_gameState = Game::Exiting;
			}
			if (event.type == sf::Event::EventType::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					_gameState = Game::Paused;
				}
			}
		}
		break;

	case Game::Playing1:
		R = rand() % 255;
		G = rand() % 255;
		B = rand() % 255;
		_mainWindow.clear(sf::Color(R, G, B));
		_gameObjectManager.UpdateAll();
		_gameObjectManager.DrawAll(_mainWindow);
		ResetBackground();
		_gameObjectManager.Get("Paddle3");
		_gameObjectManager.Remove("Paddle2");
		_scoreBoard.Draw(_mainWindow);
		_mainWindow.display();

		while (_mainWindow.pollEvent(event)) {
			if (event.type == sf::Event::EventType::Closed) {
				_gameState = Game::Exiting;
			}
			if (event.type == sf::Event::EventType::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					_gameState = Game::Paused1;
				}
			}
		}
		break;

	case Game::Completed:
		ShowEndGameMenu();
		ResetGame();
		ServiceLocator::GetAudio()->PlaySong("audio/Soundtrack.ogg", true);
		break;

	case Game::Exiting:
		_mainWindow.close();
		break;
	default:

		break;
	}
}

void Game::ShowSplashScreen() {
	SplashScreen splashScreen;
	splashScreen.Show(_mainWindow);
	_gameState = Game::ShowingMenu;
}

void Game::ShowMainMenu() {
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.Show(_mainWindow);

	switch (result) {
	case MainMenu::Exit:
		_gameState = Game::Exiting;
		break;
	case MainMenu::VsMenu:
		ServiceLocator::GetAudio()->PlaySound("audio/menuSelect.wav");
		_gameState = Game::VSMenu;
		break;
	default:
		break;

	}
}

void Game::ShowVsMenu() {
	VsMainMenu vsMainMenu;
	VsMainMenu::MenuResult result = vsMainMenu.Show(_mainWindow);

	switch (result) {
	case VsMainMenu::Exit:
		_gameState = Game::Exiting;
		break;
	case VsMainMenu::Play:
		ServiceLocator::GetAudio()->PlaySound("audio/menuSelect.wav");
		_gameState = Game::Playing;
		ResetGame();
		break;
	case VsMainMenu::Play1:
		ServiceLocator::GetAudio()->PlaySound("audio/menuSelect.wav");
		_gameState = Game::Playing1;
		break;
	default:
		break;
	}
}

void Game::ShowInGameMenu() {
	InGameMenu inGameMenu;
	InGameMenu::MenuResult result = inGameMenu.Show(_mainWindow);

	switch (result) {
	case InGameMenu::Exit:
		_gameState = Game::Exiting;
		break;
	case InGameMenu::Resume:
		ServiceLocator::GetAudio()->PlaySound("audio/menuSelect.wav");
		_gameState = Game::Playing;
		_gameObjectManager.RestartClock();
		break;
	case InGameMenu::Restart:
		ServiceLocator::GetAudio()->PlaySound("audio/menuSelect.wav");
		ResetGame();
		_gameState = Game::ShowingMenu;
		ServiceLocator::GetAudio()->PlaySong("audio/Soundtrack.ogg", true);
		break;
	default:
		break;
	}
}

void Game::ShowInGameMenu1() {
	InGameMenu inGameMenu;
	InGameMenu::MenuResult result = inGameMenu.Show(_mainWindow);

	switch (result) {
	case InGameMenu::Exit:
		_gameState = Game::Exiting;
		break;
	case InGameMenu::Resume:
		ServiceLocator::GetAudio()->PlaySound("audio/menuSelect.wav");
		_gameState = Game::Playing1;
		_gameObjectManager.RestartClock();
		break;
	case InGameMenu::Restart:
		ServiceLocator::GetAudio()->PlaySound("audio/menuSelect.wav");
		ResetGame();
		_gameState = Game::ShowingMenu;
		ServiceLocator::GetAudio()->PlaySong("audio/Soundtrack.ogg", true);
		break;
	default:
		break;
	}
}

void Game::ResetBackground() {

	PlayerPaddle *player1 = new PlayerPaddle();
	player1->SetDefaultPosition((SCREEN_WIDTH / 2), 700);
	_gameObjectManager.Add("Paddle1", player1);

	AIPaddle *player2 = new AIPaddle();
	player2->SetDefaultPosition((SCREEN_WIDTH / 2), 40);
	_gameObjectManager.Add("Paddle2", player2);

	PlayerPaddle2 *player3 = new PlayerPaddle2();
	player3->SetDefaultPosition((SCREEN_WIDTH / 2), 40);
	_gameObjectManager.Add("Paddle3", player3);

}

void Game::ResetGame() {
	_scoreBoard.ResetScore();
	_gameObjectManager.ResetAll();
	_gameObjectManager.UpdateAll();

}

bool Game::CheckVictory() {
	ScoreBoard::ScoreStatus status = _scoreBoard.CheckEndOfGame();
	EndGameMenu endGameMenu;

	if (status != ScoreBoard::Playing) {
		_gameState = Game::Completed;
	}

	return status != ScoreBoard::ScoreStatus::Playing;
}

void Game::ShowEndGameMenu() {
	ScoreBoard::ScoreStatus status = _scoreBoard.CheckEndOfGame();
	EndGameMenu endGameMenu;
	_mainWindow.clear();

	if (status == ScoreBoard::Won) {
		endGameMenu.GameWon(_mainWindow);
	}
	else if (status == ScoreBoard::Lost) {
		endGameMenu.GameLost(_mainWindow);
	}

	_gameState = Game::ShowingMenu;
}


Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
GameObjectManager Game::_gameObjectManager;
ScoreBoard Game::_scoreBoard;