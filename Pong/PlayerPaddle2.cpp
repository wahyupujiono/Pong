#include "stdafx.h"
#include "PlayerPaddle2.h"
#include "Game.h"



PlayerPaddle2::PlayerPaddle2() :
	_velocity(0),
	_maxVelocity(700.0f),
	_restitutionCoef(0.7f),
	_elapsedTimeSinceStart(0.0f)
{
	Load("images/paddle.png");
	assert(IsLoaded());
	GetSprite().setOrigin(GetWidth() / 2, GetHeight() / 2);
}

PlayerPaddle2::~PlayerPaddle2() {}

void PlayerPaddle2::Draw(sf::RenderWindow &window) {
	VisibleGameObject::Draw(window);
}

void PlayerPaddle2::Reset() {
	_velocity = 0.0f;
	_elapsedTimeSinceStart = 0.0f;
	VisibleGameObject::Reset();
}

float PlayerPaddle2::GetVelocity() const {
	return _velocity;
}

void PlayerPaddle2::Update(float elapsedTime) {

	_elapsedTimeSinceStart += elapsedTime;

	if (_elapsedTimeSinceStart > 0.001f) {
		if (_velocity > 0.0f)
			_velocity -= 1.5f;
		else
			_velocity += 1.5f;
		_elapsedTimeSinceStart = 0.0f;
	}


	// input handling by polling
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		_velocity -= 4.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		_velocity += 4.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		_velocity = 0;
	}

	if (_velocity > _maxVelocity) _velocity = _maxVelocity;

	if (_velocity < -_maxVelocity) _velocity = -_maxVelocity;

	sf::Vector2f pos = this->GetPosition();

	// If paddle goes out of screen, invert velocity
	// width / 2 is added because origin was changed in constructor
	if ((pos.x < GetWidth() / 2 && _velocity < 0) ||
		(pos.x > Game::SCREEN_WIDTH - GetWidth() / 2 && _velocity > 0)) {
		_velocity = -_restitutionCoef * _velocity;
	}

	GetSprite().move(_velocity * elapsedTime, 0);
}
