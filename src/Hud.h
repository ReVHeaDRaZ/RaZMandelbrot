#pragma once

sf::Text heading, currentIterations, hudFrames, colorMeth, instructions;
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
	currentIterations.setCharacterSize(10);
	currentIterations.setPosition(80, 5);
	currentIterations.setString(to_string(128));
	heading = currentIterations;
	hudFrames = currentIterations;
	instructions = currentIterations;
	colorMeth = currentIterations;

	heading.setPosition(5, 5);
	heading.setString("ITERATIONS\n\nFRAMES\n\nCOLOR METHOD");
	hudFrames.setPosition(80,23);
	hudFrames.setString("0");
	colorMeth.setString("SINGLE");
	colorMeth.setPosition(80,41);

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
	win->draw(colorMeth);

}
