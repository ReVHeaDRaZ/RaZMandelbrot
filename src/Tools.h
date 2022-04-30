#pragma once

// Screenshot Variables
sf::Texture screenshotTexture;
std::string filename = "MandelScreen_";
int screenshotNumber = 0;
bool takeScreenshot = false;
bool makeVideoFrames = false;

std::vector<sf::Color> palette, palette2;	// For Palette method of Coloring mandelbrot

double ReMap(double value, double istart, double istop, double ostart, double ostop);
void CreatePalettes();
void TakeScreenshot(sf::Window &window);


double ReMap(double value, double istart, double istop, double ostart, double ostop)
{
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void CreatePalettes()
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

	palette2.push_back(sf::Color(20, 0, 0));	 	// Fire
	palette2.push_back(sf::Color(40, 0, 0));	 	// Dark Red
	palette2.push_back(sf::Color(60, 0, 0));
	palette2.push_back(sf::Color(80, 0, 0));
	palette2.push_back(sf::Color(100, 0, 0));
	palette2.push_back(sf::Color(120, 0, 0));
	palette2.push_back(sf::Color(140, 0, 0));
	palette2.push_back(sf::Color(160, 0, 0));
	palette2.push_back(sf::Color(180, 0, 0));
	palette2.push_back(sf::Color(200, 0, 0));
	palette2.push_back(sf::Color(220, 0, 0));
	palette2.push_back(sf::Color(240, 0, 0));
	palette2.push_back(sf::Color(255, 20, 0));		// Red
	palette2.push_back(sf::Color(255, 40, 0));
	palette2.push_back(sf::Color(255, 60, 0));
	palette2.push_back(sf::Color(255, 80, 0));
	palette2.push_back(sf::Color(255, 100, 0));
	palette2.push_back(sf::Color(255, 120, 0));	 	// Orange
	palette2.push_back(sf::Color(255, 140, 0));
	palette2.push_back(sf::Color(255, 160, 0));
	palette2.push_back(sf::Color(255, 180, 0));
	palette2.push_back(sf::Color(255, 200, 0));
	palette2.push_back(sf::Color(255, 220, 0));
	palette2.push_back(sf::Color(255, 255, 20));	 // Yellow
	palette2.push_back(sf::Color(255, 255, 40));
	palette2.push_back(sf::Color(255, 255, 60));
	palette2.push_back(sf::Color(255, 255, 80));
	palette2.push_back(sf::Color(255, 255, 100));
	palette2.push_back(sf::Color(255, 255, 120));
	palette2.push_back(sf::Color(255, 255, 140));
	palette2.push_back(sf::Color(255, 255, 160));
	palette2.push_back(sf::Color(255, 255, 180));
	palette2.push_back(sf::Color(255, 255, 200));
	palette2.push_back(sf::Color(255, 255, 220));
	palette2.push_back(sf::Color(255, 255, 240));
	palette2.push_back(sf::Color(255, 255, 255)); 	// White
	palette2.push_back(sf::Color(255, 255, 240));
	palette2.push_back(sf::Color(255, 255, 220));
	palette2.push_back(sf::Color(255, 255, 200));
	palette2.push_back(sf::Color(255, 255, 180));
	palette2.push_back(sf::Color(255, 255, 160));
	palette2.push_back(sf::Color(255, 255, 140));
	palette2.push_back(sf::Color(255, 255, 120));
	palette2.push_back(sf::Color(255, 255, 100));
	palette2.push_back(sf::Color(255, 255, 80));
	palette2.push_back(sf::Color(255, 255, 60));
	palette2.push_back(sf::Color(255, 255, 40));
	palette2.push_back(sf::Color(255, 255, 20));	 // Yellow
	palette2.push_back(sf::Color(255, 220, 0));
	palette2.push_back(sf::Color(255, 200, 0));
	palette2.push_back(sf::Color(255, 180, 0));
	palette2.push_back(sf::Color(255, 160, 0));
	palette2.push_back(sf::Color(255, 140, 0));
	palette2.push_back(sf::Color(255, 120, 0));	 	// Orange
	palette2.push_back(sf::Color(255, 100, 0));
	palette2.push_back(sf::Color(255, 80, 0));
	palette2.push_back(sf::Color(255, 60, 0));
	palette2.push_back(sf::Color(255, 40, 0));
	palette2.push_back(sf::Color(255, 20, 0));		// Red
	palette2.push_back(sf::Color(240, 0, 0));
	palette2.push_back(sf::Color(220, 0, 0));
	palette2.push_back(sf::Color(200, 0, 0));
	palette2.push_back(sf::Color(180, 0, 0));
	palette2.push_back(sf::Color(160, 0, 0));
	palette2.push_back(sf::Color(140, 0, 0));
	palette2.push_back(sf::Color(120, 0, 0));
	palette2.push_back(sf::Color(100, 0, 0));
	palette2.push_back(sf::Color(80, 0, 0));
	palette2.push_back(sf::Color(60, 0, 0));
	palette2.push_back(sf::Color(40, 0, 0));	 	// Dark Red
}

void TakeScreenshot(sf::Window &window){
	screenshotTexture.update(window);
	screenshotTexture.copyToImage().saveToFile(filename + to_string(screenshotNumber) + ".png");
	screenshotNumber++;
}