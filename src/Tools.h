#pragma once

// Screenshot Variables
sf::Texture screenshotTexture;
std::string filename = "MandelScreen_";
int screenshotNumber = 0;
bool takeScreenshot = false;
bool makeVideoFrames = false;

std::vector<sf::Color> palette;	// For Palette method of Coloring mandelbrot

double ReMap(double value, double istart, double istop, double ostart, double ostop);
void CreatePalette16();
void TakeScreenshot(sf::Window &window);


double ReMap(double value, double istart, double istop, double ostart, double ostop)
{
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void CreatePalette16()
{
	palette.push_back(sf::Color(66, 30, 15));	 // Brown
	palette.push_back(sf::Color(25, 7, 26));	 // Dark Violet
	palette.push_back(sf::Color(9, 1, 47));		 // Dark Blue
	palette.push_back(sf::Color(4, 4, 73));		 // Blue 5
	palette.push_back(sf::Color(0, 7, 73));		 // Blue 4
	palette.push_back(sf::Color(12, 44, 138));	 // Blue 3
	palette.push_back(sf::Color(24, 82, 177));	 // Blue 2
	palette.push_back(sf::Color(57, 125, 209));	 // Blue 1
	palette.push_back(sf::Color(134, 181, 229)); // Blue 0
	palette.push_back(sf::Color(211, 236, 248)); // Lightest Blue
	palette.push_back(sf::Color(241, 233, 191)); // Lightest Yellow
	palette.push_back(sf::Color(248, 201, 95));	 // Light Yellow
	palette.push_back(sf::Color(255, 170, 0));	 // Dirty Yellow
	palette.push_back(sf::Color(204, 128, 0));	 // Brown 0
	palette.push_back(sf::Color(153, 87, 0));	 // Brown 1
	palette.push_back(sf::Color(106, 52, 3));	 // Brown 2
}

void TakeScreenshot(sf::Window &window){
	screenshotTexture.update(window);
	screenshotTexture.copyToImage().saveToFile(filename + to_string(screenshotNumber) + ".png");
	screenshotNumber++;
}