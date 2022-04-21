#include "Hud.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define MAX_NUM_PARTICLES (WIN_WIDTH * WIN_HEIGHT)

sf::VertexArray vertexarrayPoints(sf::Points, MAX_NUM_PARTICLES);

constexpr int maxThreads = 8;	// Max amount of threads to use for Fractal function

float rAmount = 1.0f; 			// RGB Multipliers to change colors
float bAmount = 0.2f;
float gAmount = 0.2f;
bool singleColor = true;		// Use single color or Colorize formula

float frames;	  				// To store FramesPerSecond
int hudCount = 0; 				// To count loops for hud

double offsetX = 0.0; 			// Used for Panning
double offsetY = 0.0;
double zmx1 = WIN_WIDTH / 4; 	// Used for Zooming
double zmx2 = 2;
double zmy1 = WIN_HEIGHT / 4;
double zmy2 = 2;
double zmAmount = 1.1;
bool zoomIn = false;
bool zoomOut = false;
int maxiterations = 128;

double ReMap(double value, double istart, double istop, double ostart, double ostop);
void InitVertexArray();
void CalculateFractal(int start, int end);
void CreateFractalThreads();

double ReMap(double value, double istart, double istop, double ostart, double ostop)
{
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void InitVertexArray()
{
	for (int y = 0; y < WIN_HEIGHT; y++)
	{
		for (int x = 0; x < WIN_WIDTH; x++)
		{
			vertexarrayPoints[x + y * WIN_WIDTH].position.x = x;
			vertexarrayPoints[x + y * WIN_WIDTH].position.y = y;
		}
	}
}

void CalculateFractal(int start, int end)
{
	for (int y = 0; y < WIN_HEIGHT; y++)
	{
		for (int x = start; x < end; x++)
		{
			double a = (x + offsetX) / zmx1 - zmx2; // X with Pan and Zoom;
			double b = zmy2 - (y + offsetY) / zmy1; // Y with Pan and Zoom;
			double ca = a;
			double cb = b;
			double n = 0;
			double absOld = 0.0;
			double convergeNumber = maxiterations; // Changes if the while loop breaks due to non-convergence

			while (n < maxiterations)
			{
				double aa = a * a - b * b;
				double bb = 2 * a * b;
				double abs = sqrt(a * a + b * b);
				a = aa + ca;
				b = bb + cb;

				if (abs > 4)
				{
					// Measure how much we exceeded the maximum
					double diffToLast = abs - absOld;
					double diffToMax = 4.0 - absOld;
					convergeNumber = n + diffToMax / diffToLast;
					break;
				}
				n++;
				absOld = abs;
			}

			// We color each pixel based on how long it takes to get to infinity
			// If we never got there, pick the color black
			if (n == maxiterations)
			{
				vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color::Black;
			}
			else
			{
				double brightness = ReMap(convergeNumber, 0, maxiterations, 0, 1);
				brightness = ReMap(sqrt(brightness), 0, 1, 0, 255);
				if(singleColor)
					vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(brightness * rAmount, brightness * gAmount, brightness * bAmount, 255);
				else
					vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(	(sf::Uint8)(sin(0.016 * brightness + 4) * 230 + 25),
																			(sf::Uint8)(sin(0.013 * brightness + 2) * 230 + 25),
																			(sf::Uint8)(sin(0.01 * brightness + 1) * 230 + 25), 255);
			}
		}
	}
}

void CreateFractalThreads(){
	std::thread t[maxThreads];
	int sectionWidth = WIN_WIDTH/maxThreads;
	int start, end;

	for(size_t i=0; i<maxThreads; i++){
		start 	= i * sectionWidth;
		end		= (i+1) * sectionWidth;

		t[i] = std::thread(CalculateFractal, start, end);
	}

	for(size_t i=0; i<maxThreads; i++)
		t[i].join();
}

int main()
{
	// Create a non resizable window
	sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "RaZ Mandelbrot", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

	sf::Vector2i mousePos;
	InitHud();
	InitVertexArray();

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
				if (event.key.code == sf::Keyboard::Key::C)
					singleColor = !singleColor;
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
					offsetX = 0.0; // Used for Panning
					offsetY = 0.0;
					zmx1 = WIN_WIDTH / 4; // Used for Zooming
					zmx2 = 2;
					zmy1 = WIN_HEIGHT / 4;
					zmy2 = 2;
					zmAmount = 1.1;
					zoomIn = false;
					zoomOut = false;
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
		{
			mousePos = sf::Mouse::getPosition(window); // Get Mouse pos
			offsetX = offsetX + (double)mousePos.x - (WIN_WIDTH / 2);
			offsetY = offsetY + (double)mousePos.y - (WIN_HEIGHT / 2);
			zmx1 = zmx1 * zmAmount;
			zmx2 = zmx2 * (1 / zmAmount);
			zmy1 = zmy1 * zmAmount;
			zmy2 = zmy2 * (1 / zmAmount);
			offsetX = offsetX * zmAmount;
			offsetY = offsetY * zmAmount;
		}
		if (zoomOut && !zoomIn)
		{
			mousePos = sf::Mouse::getPosition(window); // Get Mouse pos
			offsetX = offsetX + (double)mousePos.x - (WIN_WIDTH / 2);
			offsetY = offsetY + (double)mousePos.y - (WIN_HEIGHT / 2);
			zmx1 = zmx1 / zmAmount;
			zmx2 = zmx2 / (1 / zmAmount);
			zmy1 = zmy1 / zmAmount;
			zmy2 = zmy2 / (1 / zmAmount);
			offsetX = offsetX / zmAmount;
			offsetY = offsetY / zmAmount;
		}

		// Draw
		window.clear();

		CreateFractalThreads();				// Using threads
		//CalculateFractal(0, WIN_WIDTH); 	// No threads

		window.draw(vertexarrayPoints, sf::BlendAdd);
		DrawHud(&window);

		window.display();

		// Calculate Frames per Second
		frames = 1.f / clk.getElapsedTime().asSeconds();
		clk.restart();

		// Update Hud values
		hudCount++;
		if (hudCount >= 50)
		{
			currentIterations.setString(to_string(maxiterations));
			hudFrames.setString(to_string(frames));
			hudCount = 0;
		}
	}

	return 0;
}
