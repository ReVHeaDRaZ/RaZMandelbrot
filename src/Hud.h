#pragma once

sf::Text heading, currentIterations, hudFrames;
sf::Font font;
float frames;	  // To store FramesPerSecond
int hudCount = 0; // To count loops for hud


template <typename T>
//function to convert a non-string variable to a string.
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}

void InitHud();
void DrawHud(sf::RenderWindow* win);

void InitHud()
{
	currentIterations.setFont(font);
	currentIterations.setFillColor(sf::Color::White);
	currentIterations.setCharacterSize(20);
	currentIterations.setPosition(100, 5);
	currentIterations.setString(to_string(128));
	heading = currentIterations;
	hudFrames = currentIterations;

	heading.setPosition(5, 5);
	heading.setString("ITERATIONS\nFRAMES");
	hudFrames.setPosition(100,25);
	hudFrames.setString("0");

	//load the font
	//note that this path is relative to the workspace
	if (!font.loadFromFile("content/shredded.ttf"))
	{
		std::cout << "Failed to load font" << std::endl;
	}
}

void DrawHud(sf::RenderWindow* win)
{
	win->draw(heading);
	win->draw(currentIterations);
	win->draw(hudFrames);
}
