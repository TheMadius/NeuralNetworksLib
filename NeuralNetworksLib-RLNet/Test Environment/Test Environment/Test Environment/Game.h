#pragma once
class Game
{
public:

	Game(Game& other) = delete;
	Game(Game&& other) = delete;
	Game()
	{
		gaming = initialised = false;
	}

	virtual void Init() = 0;

protected:



	//							********************		VARIABLES		********************

	bool gaming;
	bool initialised;

};
