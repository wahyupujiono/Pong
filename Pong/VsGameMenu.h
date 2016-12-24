#pragma once
#include <vector>

class VsMainMenu {
public:
	enum MenuResult { Nothing, Play, Play1, Exit };

	struct MenuItem {
		sf::Rect<int> rect;
		MenuResult action;
	};

	MenuResult Show(sf::RenderWindow &renderWindow);

private:
	MenuResult GetMenuResponse(sf::RenderWindow &renderWindow);
	MenuResult HandleClick(int x, int y);
	std::vector<MenuItem> _menuItems;
};
