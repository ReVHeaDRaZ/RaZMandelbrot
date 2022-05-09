#pragma once
#include "ColorConvert.h"
#include "Tools.h"
#include <complex>
#include <algorithm>

sf::VertexArray vertexarrayPoints(sf::Points, MAX_NUM_PARTICLES); // To store calculated pixels
sf::Vector2i mousePos;											  // To store mouse position

constexpr int maxThreads = 8; // Max amount of threads to use for Fractal function

sf::Color interiorColor = sf::Color::Black; // Used for interior Coloring of mandelbrot
unsigned char interiorHue = 0;
HsvColor bhsv = { 255, 255, 100 };
RgbColor brgb;

sf::Clock colortimer;			// Use a timer for animated color method
bool animated = false;

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

const double escapeRadius = 4.0;
const double eps = 0.001;

// Normal Mapping variables
const double doublepi = 3.141592653589793238;
double lightHeight = 1.6; 		// Height Factor of incoming light
double angle = 45.0; 	// Angle of incoming light
const std::complex<double> complexi(0, 1);
std::complex<double> u;
std::complex<double> v = exp(complexi*angle*2.0*doublepi/360.0);
double t = 0.0;				// To store normal map lerp value
bool normalMap = false;

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
	// Store colortimer value to animate color methods
	float colortime = 0.f;
	if(animated)
		colortime = colortimer.getElapsedTime().asSeconds();

	for (uint y = 0; y < WIN_HEIGHT; y++)
	{
		for (uint x = start; x < end; x++)
		{
			double a = (x + offsetX) / zmx1 - zmx2; // X with Pan and Zoom;
			double b = zmy2 - (y + offsetY) / zmy1; // Y with Pan and Zoom;
			double ca = a;							// Store Constant a and b
			double cb = b;
			double n = 0;							// To storing Iterations
			double absOld = 0.0;
			double convergeNumber = maxiterations; 	// Changes if the while loop breaks due to non-convergence
			std::complex<double> der(1.0,1.0); 		// To store derivative
			std::complex<double> z(ca,cb);			// To store complex z
			bool inside=false;

			while (n < maxiterations)
			{
				// z2 + c
				double aa = a * a - b * b;
				double bb = 2 * a * b;
				double abs = sqrt(a * a + b * b);
				a = aa + ca;
				b = bb + cb;

				// Interior Detection
				der = der*2.0*z;
				z = std::complex<double>(a,b);
				if(sqrt(der.real()*der.real()+der.imag()*der.imag()) < eps*eps){
					n = maxiterations;
					inside=true;
					break;
				}

				// Outside of set
				if (abs > escapeRadius)
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
				if(inside)
					vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(interiorColor.r*0.5,interiorColor.g*0.5,interiorColor.b*0.5,255);
				else
				vertexarrayPoints[x + y * WIN_WIDTH].color = interiorColor;
			}
			else
			{
				// Map convergence to smooth brightness
				double brightness = ReMap(convergeNumber, 0, maxiterations, 0, 1);
				brightness = ReMap(sqrt(brightness), 0, 1, 0, 255);
				//double smooth = (n + 2 - log(log(absOld))/log(2.0));
				//double smooth = n+2 - log(log(z.real()*z.real()+z.imag()*z.imag()))/log(2.0);

				switch (colorMethod)
				{
					case 0: // Single Color
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(brightness * rAmount, brightness * gAmount, brightness * bAmount, 255);
						break;
					case 1: // Palette
						vertexarrayPoints[x + y * WIN_WIDTH].color = palette[(int)(brightness + colortime) % 16];
						break;
					case 2: // Smooth Color with sine
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color((sf::Uint8)(sin(0.3 * brightness + 0 + colortime) * 127 + 127),
																				(sf::Uint8)(sin(0.3 * brightness + 2 + colortime) * 127 + 127),
																				(sf::Uint8)(sin(0.3 * brightness + 4 + colortime) * 127 + 127),
																				255);
						break;
					case 3: // HSV
						HsvColor hsv;
						RgbColor rgb;
						hsv.h = int(brightness + colortime) % 255;
						hsv.s = 255;
						hsv.v = 255;
						rgb = HsvToRgb(hsv);
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(rgb.r, rgb.g, rgb.b, 255);
						break;
					case 4: // Palette 2 Fire
						vertexarrayPoints[x + y * WIN_WIDTH].color = palette2[(int)(brightness + colortime) % 69];
						break;
					default: // Default to Single color method
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(brightness * rAmount, brightness * gAmount, brightness * bAmount, 255);
						break;
				}

				// Normal Map
				if(normalMap){
					u = z/der;
					u = u/abs(u);
					t = u.real()*v.real() + u.imag()*v.imag() + lightHeight;
					t = t/(1+lightHeight);
					if(t<0) t=0;
					if(colorMethod==0)	// Single Color NormalMap
						vertexarrayPoints[x + y * WIN_WIDTH].color = LerpColor(sf::Color::Black,sf::Color(rAmount*255,gAmount*255,bAmount*255,255),t);
					else
						vertexarrayPoints[x + y * WIN_WIDTH].color = LerpColor(sf::Color::Black,vertexarrayPoints[x + y * WIN_WIDTH].color,t);
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
	colortimer.restart();
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