#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define MAX_NUM_PARTICLES (WIN_WIDTH * WIN_HEIGHT)
#include "Hud.h"
#include "Mandelbrot.h"

int main()
{
	// Create a non resizable window
	sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "RaZ Mandelbrot", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

	// Create Texture for screenshots
	screenshotTexture.create(window.getSize().x, window.getSize().y);

	InitHud();
	InitVertexArray();
	CreatePalette16();

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
				if (event.key.code == sf::Keyboard::Key::R)
					rAmount -= 0.1f;
				if (event.key.code == sf::Keyboard::Key::T)
					rAmount += 0.1f;
				if (event.key.code == sf::Keyboard::Key::G)
					gAmount -= 0.1f;
				if (event.key.code == sf::Keyboard::Key::H)
					gAmount += 0.1f;
				if (event.key.code == sf::Keyboard::Key::B)
					bAmount -= 0.1f;
				if (event.key.code == sf::Keyboard::Key::N)
					bAmount += 0.1f;
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
					if (colorMethod > 3)
						colorMethod = 0;
				}
				if (event.key.code == sf::Keyboard::Key::PageUp)
				{
					maxiterations += 32;
				}
				if (event.key.code == sf::Keyboard::Key::PageDown)
				{
					maxiterations -= 32;
					if (maxiterations < 32)
						maxiterations = 32;
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
		else if(!makeVideoFrames)
			DrawHud(&window);

		window.display();

		// Calculate Frames per Second
		frames = 1.f / clk.getElapsedTime().asSeconds();
		clk.restart();

		// Update Hud values
		hudCount++;
		if (hudCount >= 30)
		{
			currentIterations.setString(to_string(maxiterations));
			hudFrames.setString(to_string(frames));
			hudCount = 0;
		}
	}

	return 0;
}
