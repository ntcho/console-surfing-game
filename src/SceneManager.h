#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "ObjectManager.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <cmath>

class SceneManager {
private:
	int size_x; // width of the console window
	int size_y; // height of the console window
	float center_x; // x-axis position of the camera
	float center_y; // y-axis position of the camera
	HANDLE hBuffer[2]; // buffer handle
	int nScreenIndex; // save current buffer
	void CreateBuffer(); // create buffer
	void WriteBuffer(int x, int y, std::string str); // write str at (x,y)
	void FlipBuffer(); // Flip buffer
	void ClearBuffer(); // Clear buffer
	void DeleteBuffer(); // Delete buffer
	void Draw(float center_x, float center_y, int width, int height, std::string texture); // Draw texture
public:
	SceneManager(); // Instantiates the SceneManager object
	SceneManager(int x, int y); // Instantiates with given width and height
	void Init(); // Initialization
	void Ready(); // Draw ready scene
	void Render(ObjectManager& manager); // Renders all object
	void Pause(); // Draw pause scene
	void Release(); // Finish the game
	void SetColor(unsigned char bg_color, unsigned char txt_color); // Set color
};

#endif