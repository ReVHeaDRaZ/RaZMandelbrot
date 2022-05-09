uint WIN_WIDTH = 800; // Window Resolution
uint WIN_HEIGHT = 600;
uint MAX_NUM_PARTICLES = (WIN_WIDTH * WIN_HEIGHT);

#include "Hud.h"
#include "Mandelbrot.h"

static void show_usage(std::string name)
{
	std::cout << "Usage: " << name << " <option(s)> "
			  << "Options:\n"
			  << "\t-h\t\tShow this help message\n"
			  << "\t-r Resolution WIDTH HEIGHT\t"
			  << std::endl;
}

int main(int argc, char* argv[])
{
	//-------------Command-Line Arguments-----------------
	// No Arguments
	if (argc == 1)
	{
		WIN_WIDTH = 800;
		WIN_HEIGHT = 600;
	}
	else
	{ // Not enough arguments
		if (argc < 3)
		{
			std::cout << "Usage: " << argv[0] << "-r Resolution WIDTH HEIGHT" << std::endl;
			show_usage(argv[0]);
			return 0;
		}

		std::string widthArg;
		std::string heightArg;
		for (int i = 1; i < argc; ++i)
		{
			std::string arg = argv[i];
			if (arg == "-h")
			{
				show_usage(argv[0]);
				return 0;
			}
			else if (arg == "-r")
			{
				if (i + 2 < argc)
				{							// Make sure we aren't at the end of argv!
					widthArg = argv[i + 1]; // Increment 'i' so we don't get the argument as the next argv[i].
					heightArg = argv[i + 2];
				}
				else
				{ // Uh-oh, there was not enough argument to the option.
					std::cout << "-r option requires two arguments." << std::endl;
					return 0;
				}
			}
		}
		WIN_WIDTH = stoi(widthArg);
		WIN_HEIGHT = stoi(heightArg);
		MAX_NUM_PARTICLES = (WIN_WIDTH * WIN_HEIGHT);
		vertexarrayPoints.resize(MAX_NUM_PARTICLES);
	}

	// Create a non resizable window
	sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "RaZ Mandelbrot", sf::Style::Fullscreen); //sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

	// Create Texture for screenshots
	screenshotTexture.create(window.getSize().x, window.getSize().y);

	InitHud();
	InitVertexArray();
	CreatePalettes();
	ResetView();

	// Use a timer to obtain the time elapsed
	sf::Clock clk;
	clk.restart();


	// Main loop
	while (window.isOpen())
	{
		// Event handling
		sf::Event event;

		// Event loop (keyboard, mouse ...
		while (window.pollEvent(event))
		{
			// Close the window if a key is pressed or if requested
			if (event.type == sf::Event::Closed)
				window.close();

			// Key pressed
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Key::Escape)
					window.close();
				if (event.key.code == sf::Keyboard::Key::Up)
					offsetY -= 10;
				if (event.key.code == sf::Keyboard::Key::Down)
					offsetY += 10;
				if (event.key.code == sf::Keyboard::Key::Left)
					offsetX -= 10;
				if (event.key.code == sf::Keyboard::Key::Right)
					offsetX += 10;
				if (event.key.code == sf::Keyboard::Key::R){
					rAmount -= 0.05f;
					if(rAmount<0.f) rAmount=0.f;
				}
				if (event.key.code == sf::Keyboard::Key::T){
					rAmount += 0.05f;
					if(rAmount>1.0f) rAmount=1.0f;
				}
				if (event.key.code == sf::Keyboard::Key::G){
					gAmount -= 0.05f;
					if(gAmount<0.f) gAmount=0.f;
				}
				if (event.key.code == sf::Keyboard::Key::H){
					gAmount += 0.05f;
					if(gAmount>1.0f) gAmount=1.0f;
				}
				if (event.key.code == sf::Keyboard::Key::B){
					bAmount -= 0.05f;
					if(bAmount<0.f) bAmount=0.f;
				}
				if (event.key.code == sf::Keyboard::Key::N){
					bAmount += 0.05f;
					if(bAmount>1.0f) bAmount=1.0f;
				}
				if (event.key.code == sf::Keyboard::Key::Comma)
				{
					interiorHue -= 2;
					bhsv.h = interiorHue;
					brgb = HsvToRgb(bhsv);
					interiorColor = sf::Color(brgb.r, brgb.g, brgb.b, 255);
				}
				if (event.key.code == sf::Keyboard::Key::Period)
				{
					interiorHue += 2;
					bhsv.h = interiorHue;
					brgb = HsvToRgb(bhsv);
					interiorColor = sf::Color(brgb.r, brgb.g, brgb.b, 255);
				}
				if (event.key.code == sf::Keyboard::Key::S)
					takeScreenshot = true;
				if (event.key.code == sf::Keyboard::Key::V)
					makeVideoFrames = !makeVideoFrames;
				if (event.key.code == sf::Keyboard::Key::C)
				{
					colorMethod++;
					switch(colorMethod){
						case 0:
							colorMeth.setString("SINGLE");
							break;
						case 1:
							colorMeth.setString("PALETTE");
							break;
						case 2:
							colorMeth.setString("SMOOTH");
							break;
						case 3:
							colorMeth.setString("HUE");
							break;
						case 4:
							colorMeth.setString("FIRE PALETTE");
							break;;
						default:
							colorMeth.setString("SINGLE");
					}
					if (colorMethod > 4)
						colorMethod = 0;
				}
				if (event.key.code == sf::Keyboard::Key::PageUp)
				{
					maxiterations += 32;
					currentIterations.setString(to_string(maxiterations));
				}
				if (event.key.code == sf::Keyboard::Key::PageDown)
				{
					maxiterations -= 32;
					if (maxiterations < 32)
						maxiterations = 32;
					currentIterations.setString(to_string(maxiterations));
				}
				if (event.key.code == sf::Keyboard::Key::I)
					showControls = !showControls;
				if (event.key.code == sf::Keyboard::Key::A)
					animated = !animated;
				if (event.key.code == sf::Keyboard::Key::X)
					normalMap = !normalMap;
				if (event.key.code == sf::Keyboard::Key::End){
					lightHeight -= 0.1;
					if(lightHeight<0.2) lightHeight=0.2;
				}
				if (event.key.code == sf::Keyboard::Key::Home){
					lightHeight += 0.1;
					if(lightHeight>4.0) lightHeight=4.0;
				}
				if (event.key.code == sf::Keyboard::Key::Insert){
					angle--;
					if(angle<0.0) angle=360.0;
					v = exp(complexi*angle*2.0*doublepi/360.0);
				}
				if (event.key.code == sf::Keyboard::Key::Delete){
					angle++;
					if(angle>360.0) angle=0.0;
					v = exp(complexi*angle*2.0*doublepi/360.0);
				}

			}
			// Mouse Pressed
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
					zoomIn = true;
				if (event.mouseButton.button == sf::Mouse::Button::Right)
					zoomOut = true;
				if (event.mouseButton.button == sf::Mouse::Button::Middle) // Reset View
				{
					ResetView();
				}
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
					zoomIn = false;
				if (event.mouseButton.button == sf::Mouse::Button::Right)
					zoomOut = false;
			}
		}

		if (zoomIn && !zoomOut)
			ZoomIn(window);
		if (zoomOut && !zoomIn)
			ZoomOut(window);

		// Draw
		window.clear();

		CreateFractalThreads(); // Using threads
		//CalculateFractal(0, WIN_WIDTH); 	// No threads

		window.draw(vertexarrayPoints, sf::BlendAdd);
		if (takeScreenshot)
		{
			TakeScreenshot(window);
			takeScreenshot = false;
		}
		else if (!makeVideoFrames)
			DrawHud(&window);

		window.display();

		// Calculate Frames per Second
		frames = 1.f / clk.getElapsedTime().asSeconds();
		clk.restart();

		// Update Hud values
		hudCount++;
		if (hudCount >= 30)
		{
			hudFrames.setString(to_string(frames));
			hudCount = 0;
		}
	}

	return 0;
}
