#pragma once
#include "ColorConvert.h"
#include "Tools.h"

sf::VertexArray vertexarrayPoints(sf::Points, MAX_NUM_PARTICLES); // To store calculated pixels
sf::Vector2i mousePos;											  // To store mouse position

constexpr int maxThreads = 8; // Max amount of threads to use for Fractal function

sf::Color interiorColor = sf::Color::Black; // Used for interior Coloring of mandelbrot
unsigned char interiorHue = 0;
HsvColor bhsv = { 255, 255, 100 };
RgbColor brgb;

float rAmount = 1.0f; // RGB Multipliers to change colors
float gAmount = 0.2f;
float bAmount = 0.2f;
int colorMethod = 0; // For Selecting method of colorization

double offsetX = 0.0; // Used for Panning
double offsetY = 0.0;
double zmx1 = WIN_WIDTH / 4; // Used for Zooming
double zmx2 = 2;
double zmy1 = WIN_HEIGHT / 4;
double zmy2 = 2;
double zmAmount = 1.1;
bool zoomIn = false;
bool zoomOut = false;
int maxiterations = 128;

void InitVertexArray();
void CalculateFractal(uint start, uint end);
void CreateFractalThreads();
void ResetView();
void ZoomIn(sf::Window& window);
void ZoomOut(sf::Window& window);

void InitVertexArray()
{
	for (uint y = 0; y < WIN_HEIGHT; y++)
	{
		for (uint x = 0; x < WIN_WIDTH; x++)
		{
			vertexarrayPoints[x + y * WIN_WIDTH].position.x = x;
			vertexarrayPoints[x + y * WIN_WIDTH].position.y = y;
		}
	}
}

void CalculateFractal(uint start, uint end)
{
	for (uint y = 0; y < WIN_HEIGHT; y++)
	{
		for (uint x = start; x < end; x++)
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
			// If we never got there, pick the interiorColor
			if (n == maxiterations)
			{
				vertexarrayPoints[x + y * WIN_WIDTH].color = interiorColor;
			}
			else
			{
				double brightness = ReMap(convergeNumber, 0, maxiterations, 0, 1);
				brightness = ReMap(sqrt(brightness), 0, 1, 0, 255);
				//double smooth = (n + 1 - log(log(absOld))/log(2))/maxiterations;
				switch (colorMethod)
				{
					case 0: // Single Color
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(brightness * rAmount, brightness * gAmount, brightness * bAmount, 255);
						break;
					case 1: // Palette
						vertexarrayPoints[x + y * WIN_WIDTH].color = palette[(int)brightness % 16];
						break;
					case 2: // Smooth Color
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color((sf::Uint8)(sin(0.016 * brightness + 4) * 230 + 25),
							(sf::Uint8)(sin(0.013 * brightness + 2) * 230 + 25),
							(sf::Uint8)(sin(0.01 * brightness + 1) * 230 + 25),
							255);
						break;
					case 3: // HSV
						HsvColor hsv;
						RgbColor rgb;
						hsv.h = brightness;
						hsv.s = 255;
						hsv.v = 255;
						rgb = HsvToRgb(hsv);
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(rgb.r, rgb.g, rgb.b, 255);
						break;
					case 4: // Palette 2
						vertexarrayPoints[x + y * WIN_WIDTH].color = palette2[(int)brightness % 36];
						break;
					/*case 5: // Palette 2 Attempt to smooth
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color( (palette2[(int)(brightness+1) % 36].r - palette2[(int)brightness % 36].r) * smooth + palette2[(int)brightness % 36].r,
																			(palette2[(int)(brightness+1) % 36].g - palette2[(int)brightness % 36].g) * smooth + palette2[(int)brightness % 36].g,
																			(palette2[(int)(brightness+1) % 36].b - palette2[(int)brightness % 36].b) * smooth + palette2[(int)brightness % 36].b,255);
						break;
					*/
					default: // Default to Single color method
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(brightness * rAmount, brightness * gAmount, brightness * bAmount, 255);
						break;
				}
			}
		}
	}
}

void CreateFractalThreads()
{
	std::thread t[maxThreads];
	int sectionWidth = WIN_WIDTH / maxThreads;
	int start, end;

	for (size_t i = 0; i < maxThreads; i++)
	{
		start = i * sectionWidth;
		end = (i + 1) * sectionWidth;

		t[i] = std::thread(CalculateFractal, start, end);
	}

	for (size_t i = 0; i < maxThreads; i++)
		t[i].join();
}

void ResetView()
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

void ZoomIn(sf::Window& window)
{
	mousePos = sf::Mouse::getPosition(window); // Get Mouse pos
	offsetX = offsetX + ((double)mousePos.x - (WIN_WIDTH / 2)) * 0.1;
	offsetY = offsetY + ((double)mousePos.y - (WIN_HEIGHT / 2)) * 0.1;
	zmx1 = zmx1 * zmAmount;
	zmx2 = zmx2 * (1 / zmAmount);
	zmy1 = zmy1 * zmAmount;
	zmy2 = zmy2 * (1 / zmAmount);
	offsetX = offsetX * zmAmount;
	offsetY = offsetY * zmAmount;
	if (makeVideoFrames)
		TakeScreenshot(window);
}

void ZoomOut(sf::Window& window)
{
	mousePos = sf::Mouse::getPosition(window); // Get Mouse pos
	offsetX = offsetX + ((double)mousePos.x - (WIN_WIDTH / 2)) * 0.1;
	offsetY = offsetY + ((double)mousePos.y - (WIN_HEIGHT / 2)) * 0.1;
	zmx1 = zmx1 / zmAmount;
	zmx2 = zmx2 / (1 / zmAmount);
	zmy1 = zmy1 / zmAmount;
	zmy2 = zmy2 / (1 / zmAmount);
	offsetX = offsetX / zmAmount;
	offsetY = offsetY / zmAmount;
}