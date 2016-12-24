#pragma once
#include "VisibleGameObject.h"


class PlayerPaddle2 :
	public VisibleGameObject {
public:
	PlayerPaddle2();
	~PlayerPaddle2();

	void Update(float elapsedTime);
	void Draw(sf::RenderWindow &window);
	void Reset();

	float GetVelocity() const;
private:
	float _velocity;
	float _maxVelocity;
	float _restitutionCoef;
	float _elapsedTimeSinceStart;

};
