﻿#include "SceneManager.h"
#include "ObjectManager.h"

extern int life_count;
extern int score;
extern int boost_count;
extern int high_score;

// Instantiates the SceneManager object
SceneManager::SceneManager() {
	size_x = 160;
	size_y = 45;
	center_x = 0.0f;
	center_y = 0.0f;
	nScreenIndex = 0;
}
// Instantiates with given width and height
SceneManager::SceneManager(int x, int y) {
	size_x = x;
	size_y = y;
	center_x = 0.0f;
	center_y = 0.0f;
	nScreenIndex = 0;
}
// create buffer
void SceneManager::CreateBuffer() {
	COORD size = { size_x, size_y };
	CONSOLE_CURSOR_INFO cci;
	SMALL_RECT rect;

	rect.Bottom = 0;
	rect.Left = 0;
	rect.Right = size_x - 1;
	rect.Top = size_y - 1;

	hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(hBuffer[0], size);
	SetConsoleWindowInfo(hBuffer[0], TRUE, &rect);

	hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(hBuffer[1], size);
	SetConsoleWindowInfo(hBuffer[1], TRUE, &rect);

	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hBuffer[0], &cci);
	SetConsoleCursorInfo(hBuffer[1], &cci);
}
// write str at (x,y)
void SceneManager::WriteBuffer(int x, int y, std::string str) {
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(hBuffer[nScreenIndex], CursorPosition);

	// string to const char*
	std::vector<char> temp_arr(str.begin(), str.end());
	temp_arr.push_back('\0');
	char* temp_arr_ptr = &temp_arr[0];

	WriteFile(hBuffer[nScreenIndex], temp_arr_ptr, strlen(temp_arr_ptr), &dw, NULL);
}
// Flip buffer
void SceneManager::FlipBuffer() {
	SetConsoleActiveScreenBuffer(hBuffer[nScreenIndex]);
	nScreenIndex = !nScreenIndex;
}
// Clear buffer
void SceneManager::ClearBuffer() {
	COORD Coor = { 0,0 };
	DWORD dw;
	FillConsoleOutputCharacter(hBuffer[nScreenIndex], ' ', size_x * size_y, Coor, &dw);
}
// Delete buffer
void SceneManager::DeleteBuffer() {
	CloseHandle(hBuffer[0]);
	CloseHandle(hBuffer[1]);
}
// Draw texture
void SceneManager::Draw(float center_x, float center_y, int width, int height, std::string texture) {
	int x = (int)(round(center_x - (float)(width)));
	int y = (int)(round(center_y - (float)(height / 2)));
	for (int i = 0; i < height; i++) {
		if (y + i < 0 || y + i >= size_y) continue;
		else if (x < 0) {
			for (int j = 0; j < 2 * width; j++) {
				if (x + j >= 0) {
					for (int k = 0; k < 2 * width - j; k++) {
						map[y + i][k + 2 * (x + j)] = texture[i * 2 * width + j + k];
					}
					//WriteBuffer(2 * (x + j), y + i, texture.substr(i * 2 * width + j, 2 * width - j));
					break;
				}
			}
		}
		else {
			int check = -1;
			for (int j = 0; j < 2 * width; j++) {
				if (x + j >= size_x / 2) {
					check = j;
					break;
				}
			}
			if (check == -1) {
				for (int k = 0; k < 2 * width; k++) {
					map[y + i][k + 2 * x] = texture[i * 2 * width + k];
				}
				//WriteBuffer(2 * x, y + i, texture.substr(i * 2 * width, 2 * width));
			}
			else {
				for (int k = 0; k < check; k++) {
					map[y + i][k + 2 * x] = texture[i * 2 * width + k];
				}
				//WriteBuffer(2 * x, y + i, texture.substr(i * 2 * width, check));
			}
		}
	}
}
// Initialize scene manager
void SceneManager::Init() {
	// Set console_size
	char setText[100];
	sprintf_s(setText, "mode con cols=%d lines=%d", size_x, size_y);
	system(setText);

	// Set console_title
	//SetConsoleTitle("Console surfing game");

	// Set cursor unvisible
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);

	isinvincible = false;

	// Create buffer
	CreateBuffer();
}
// Print ready scene at console screen
void SceneManager::Ready() {
	// Clear buffer
	ClearBuffer();

	SetColor(1, 15);

	// Print UI
	std::string t;
	t = "";
	for (int i = 0; i < size_x; i++) {
		t += " ";
	}
	for (int i = 0; i < size_y; i++) {
		WriteBuffer(0, i, t);
	}

	extern int score;
	t = "┌─────────────────────────────────────────┐";
	WriteBuffer(size_x / 2 - 22, 0, t);
	t = "│";
	WriteBuffer(size_x / 2 - 22, 1, t);
	t = "♥";
	SetColor(1, 12);
	for (int i = 0; i < 3; i++) {
		WriteBuffer(size_x / 2 - 20 + 2 * i, 1, t);
	}
	SetColor(1, 15);
	t = "High score : " + std::to_string(high_score) + "m";
	WriteBuffer(size_x / 2 - 8, 1, t);
	t = "ⓑ";
	SetColor(1, 10);
	for (int i = 0; i < 3; i++) {
		WriteBuffer(size_x / 2 + 14 + 2 * i, 1, t);
	}
	SetColor(1, 15);
	t = "│";
	WriteBuffer(size_x / 2 + 20, 1, t);
	t = "└─────────────────────────────────────────┘";
	WriteBuffer(size_x / 2 - 22, 2, t);

	// Print description
	t = "서핑을 시작하려면 space bar를 누르세요.";
	WriteBuffer(size_x / 2 - 19, size_y / 5, t);
	t = "┌────────────────────────────────────────────────────────────────────────────────────────────┐";
	WriteBuffer(size_x / 2 - 46, size_y / 2 - 4, t);
	t = "│";
	for (int i = 0; i < 20; i++) {
		WriteBuffer(size_x / 2 - 46, size_y / 2 - 3 + i, t);
		WriteBuffer(size_x / 2 + 47, size_y / 2 - 3 + i, t);
	}
	t = "└────────────────────────────────────────────────────────────────────────────────────────────┘";
	WriteBuffer(size_x / 2 - 46, size_y / 2 + 17, t);
	t = "▒";
	WriteBuffer(size_x * 3 / 11, size_y / 2, t);
	t = "해류";
	WriteBuffer(size_x * 3 / 11 - 1, size_y / 2 + 2, t);
	t = "ψ";
	WriteBuffer(size_x * 4 / 11, size_y / 2, t);
	t = "해초";
	WriteBuffer(size_x * 4 / 11 - 1, size_y / 2 + 2, t);
	t = "♥";
	WriteBuffer(size_x * 5 / 11, size_y / 2, t);
	t = "체력";
	WriteBuffer(size_x * 5 / 11 - 1, size_y / 2 + 2, t);
	t = " ⓑ";
	WriteBuffer(size_x * 6 / 11, size_y / 2, t);
	t = "부스트";
	WriteBuffer(size_x * 6 / 11 - 1, size_y / 2 + 2, t);
	t = "▨▨";
	WriteBuffer(size_x * 7 / 11, size_y / 2 - 1, t);
	t = "|  |";
	WriteBuffer(size_x * 7 / 11, size_y / 2, t);
	t = "갑판";
	WriteBuffer(size_x * 7 / 11, size_y / 2 + 2, t);
	t = " |> ";
	WriteBuffer(size_x * 8 / 11, size_y / 2 - 1, t);
	t = "-==-";
	WriteBuffer(size_x * 8 / 11, size_y / 2, t);
	t = "부표";
	WriteBuffer(size_x * 8 / 11, size_y / 2 + 2, t);
	t = " _ \\__/ _ ";
	WriteBuffer(size_x * 4 / 11 - 5, size_y / 2 + 9, t);
	t = " /\\(--)/\\ ";
	WriteBuffer(size_x * 4 / 11 - 5, size_y / 2 + 10, t);
	t = "~oO0oO0Oo~";
	WriteBuffer(size_x * 4 / 11 - 5, size_y / 2 + 11, t);
	t = "크라켄";
	WriteBuffer(size_x * 4 / 11 - 3, size_y / 2 + 13, t);
	t = "/\\";
	WriteBuffer(size_x * 5 / 11, size_y / 2 + 10, t);
	t = "\\/";
	WriteBuffer(size_x * 5 / 11, size_y / 2 + 11, t);
	t = "서퍼";
	WriteBuffer(size_x * 5 / 11 - 1, size_y / 2 + 13, t);
	t = "게임 조작 : 방향키, F (부스터)";
	WriteBuffer(size_x * 6 / 11, size_y / 2 + 9, t);
	t = "일시 정지 : SPACE BAR";
	WriteBuffer(size_x * 6 / 11, size_y / 2 + 11, t);
	t = "게임 종료 : ESC";
	WriteBuffer(size_x * 6 / 11, size_y / 2 + 13, t);

	// Flip buffer
	FlipBuffer();
}
// Print game scene at console screen
void SceneManager::Render(ObjectManager& manager) {
	// Clear buffer
	ClearBuffer();

	SetColor(1, 15);

	// Get object from manager
	Player& player = manager.GetPlayer();
	std::vector<MovableObject*>& movable_obj = manager.GetMovable();
	std::vector<GameObject*>& immovable_obj = manager.GetImmovable();

	// Remove out of sight objects
	float top_visible_y = manager.GetPlayer().GetCenterY() - GetHeight();

	// check movable objects out of sight
	for (int id = 0; id < movable_obj.size(); id++)
	{
		if (movable_obj[id]->GetCenterY() <= top_visible_y)
			manager.RemoveMovable(movable_obj[id]);
	}

	// check immovable objects out of sight
	for (int id = 0; id < immovable_obj.size(); id++)
	{
		if (immovable_obj[id]->GetCenterY() <= top_visible_y)
			manager.RemoveImmovable(immovable_obj[id]);
	}

	std::string t; // temp string
	float dx, dy; // variables for coordinate calculation
	float x, y; // centerX, centerY
	int w, h; // width, height

	dx = size_x / 4 - player.GetCenterX();
	dy = size_y * 2 / 5 - player.GetCenterY();

	// Initialize map
	map[0] = "";
	for (int i = 0; i < size_x; i++) {
		map[0] += " ";
	}
	for (int i = 0; i < size_y; i++) {
		map[i + 1] = map[i];
	}

	// Draw immovable object
	for (int i = 0; i < immovable_obj.size(); i++) {
		if (immovable_obj[i]->GameObject::IsVisible() == false)
			continue; // don't render objects flagged as invisible

		x = immovable_obj[i]->GetCenterX() + dx;
		y = immovable_obj[i]->GetCenterY() + dy;
		w = immovable_obj[i]->GetWidth();
		h = immovable_obj[i]->GetHeight();
		t = immovable_obj[i]->GetTexture();
		Draw(x, y, w, h, t);
	}

	// Draw movable object
	for (int i = 0; i < movable_obj.size(); i++) {
		if (movable_obj[i]->GameObject::IsVisible() == false)
			continue; // don't render objects flagged as invisible

		x = movable_obj[i]->GetCenterX() + dx;
		y = movable_obj[i]->GetCenterY() + dy;
		w = movable_obj[i]->GetWidth();
		h = movable_obj[i]->GetHeight();
		t = movable_obj[i]->GetTexture();
		Draw(x, y, w, h, t);
	}

	// Draw player
	if (player.CheckInvincible()) {
		if (isinvincible) {
			Draw(size_x / 4, size_y * 2 / 5, player.GetWidth(), player.GetHeight(), player.GetTexture());
		}
		else {
			Draw(size_x / 4, size_y * 2 / 5, player.GetWidth(), player.GetHeight(), "        ");
		}
		isinvincible = !isinvincible;
	}
	else {
		Draw(size_x / 4, size_y * 2 / 5, player.GetWidth(), player.GetHeight(), player.GetTexture());
	}

	// Print map
	for (int i = 0; i < size_y; i++) {
		WriteBuffer(0, i, map[i]);
	}

	// Print UI
	t = "┌─────────────────────────────────────────┐";
	WriteBuffer(size_x / 2 - 22, 0, t);
	t = "                                           ";
	WriteBuffer(size_x / 2 - 22, 1, t);
	t = "│";
	WriteBuffer(size_x / 2 - 22, 1, t);
	SetColor(1, 12);
	for (int i = 0; i < 3; i++) {
		if (i < life_count) {
			t = "♥";
		}
		else {
			t = "♡";
		}
		WriteBuffer(size_x / 2 - 20 + 2 * i, 1, t);
	}
	SetColor(1, 15);
	t = "Score : " + std::to_string(score) + "m";
	WriteBuffer(size_x / 2 - 6, 1, t);
	t = "ⓑ";
	SetColor(1, 10);
	for (int i = 0; i < boost_count; i++) {
		WriteBuffer(size_x / 2 + 14 + 2 * i, 1, t);
	}
	SetColor(1, 15);
	t = "│";
	WriteBuffer(size_x / 2 + 20, 1, t);
	t = "└─────────────────────────────────────────┘";
	WriteBuffer(size_x / 2 - 22, 2, t);

	// Flip buffer
	FlipBuffer();
}
// Print pause scene at console screen
void SceneManager::Pause(ObjectManager& manager) {
	// Clear buffer
	ClearBuffer();

	SetColor(1, 15);

	// Get object from manager
	Player player = manager.GetPlayer();
	std::vector<MovableObject*> movable_obj = manager.GetMovable();
	std::vector<GameObject*> immovable_obj = manager.GetImmovable();

	std::string t; // temp string
	float dx, dy; // variables for coordinate calculation
	float x, y; // centerX, centerY
	int w, h; // width, height

	dx = size_x / 4 - player.GetCenterX();
	dy = size_y * 2 / 5 - player.GetCenterY();

	// Initialize map
	map[0] = "";
	for (int i = 0; i < size_x; i++) {
		map[0] += " ";
	}
	for (int i = 0; i < size_y; i++) {
		map[i + 1] = map[i];
	}

	// Draw immovable object
	for (int i = 0; i < immovable_obj.size(); i++) {
		x = immovable_obj[i]->GetCenterX() + dx;
		y = immovable_obj[i]->GetCenterY() + dy;
		w = immovable_obj[i]->GetWidth();
		h = immovable_obj[i]->GetHeight();
		t = immovable_obj[i]->GetTexture();
		Draw(x, y, w, h, t);
	}

	// Draw movable object
	for (int i = 0; i < movable_obj.size(); i++) {
		x = movable_obj[i]->GetCenterX() + dx;
		y = movable_obj[i]->GetCenterY() + dy;
		w = movable_obj[i]->GetWidth();
		h = movable_obj[i]->GetHeight();
		t = movable_obj[i]->GetTexture();
		Draw(x, y, w, h, t);
	}

	// Draw player
	Draw(size_x / 4, size_y * 2 / 5, player.GetWidth(), player.GetHeight(), player.GetTexture());

	// Print map
	for (int i = 0; i < size_y; i++) {
		WriteBuffer(0, i, map[i]);
	}

	// Print UI
	t = "┌─────────────────────────────────────────┐";
	WriteBuffer(size_x / 2 - 22, 0, t);
	t = "                                           ";
	WriteBuffer(size_x / 2 - 22, 1, t);
	t = "│";
	WriteBuffer(size_x / 2 - 22, 1, t);
	SetColor(1, 12);
	for (int i = 0; i < 3; i++) {
		if (i < life_count) {
			t = "♥";
		}
		else {
			t = "♡";
		}
		WriteBuffer(size_x / 2 - 20 + 2 * i, 1, t);
	}
	SetColor(1, 15);
	t = "Score : " + std::to_string(score) + "m";
	WriteBuffer(size_x / 2 - 6, 1, t);
	t = "ⓑ";
	SetColor(1, 10);
	for (int i = 0; i < boost_count; i++) {
		WriteBuffer(size_x / 2 + 14 + 2 * i, 1, t);
	}
	SetColor(1, 15);
	t = "│";
	WriteBuffer(size_x / 2 + 20, 1, t);
	t = "└─────────────────────────────────────────┘";
	WriteBuffer(size_x / 2 - 22, 2, t);

	// Print pause message
	t = "┌─────────────────────────┐";
	WriteBuffer(size_x / 2 - 14, size_y / 5, t);
	t = "│    게임이 일시 중지됨   │";
	WriteBuffer(size_x / 2 - 14, size_y / 5 + 1, t);
	t = "│                         │";
	WriteBuffer(size_x / 2 - 14, size_y / 5 + 2, t);
	t = "│   다시 시작하려면 space │";
	WriteBuffer(size_x / 2 - 14, size_y / 5 + 3, t);
	t = "└─────────────────────────┘";
	WriteBuffer(size_x / 2 - 14, size_y / 5 + 4, t);

	// Flip buffer
	FlipBuffer();
}
// Called when life_count is 0
void SceneManager::GameOver(ObjectManager& manager) {
	// Clear buffer
	ClearBuffer();

	SetColor(1, 15);

	// Get object from manager
	Player player = manager.GetPlayer();
	std::vector<MovableObject*> movable_obj = manager.GetMovable();
	std::vector<GameObject*> immovable_obj = manager.GetImmovable();

	std::string t; // temp string
	float dx, dy; // variables for coordinate calculation
	float x, y; // centerX, centerY
	int w, h; // width, height

	dx = size_x / 4 - player.GetCenterX();
	dy = size_y * 2 / 5 - player.GetCenterY();

	// Initialize map
	map[0] = "";
	for (int i = 0; i < size_x; i++) {
		map[0] += " ";
	}
	for (int i = 0; i < size_y; i++) {
		map[i + 1] = map[i];
	}

	// Draw immovable object
	for (int i = 0; i < immovable_obj.size(); i++) {
		x = immovable_obj[i]->GetCenterX() + dx;
		y = immovable_obj[i]->GetCenterY() + dy;
		w = immovable_obj[i]->GetWidth();
		h = immovable_obj[i]->GetHeight();
		t = immovable_obj[i]->GetTexture();
		Draw(x, y, w, h, t);
	}

	// Draw movable object
	for (int i = 0; i < movable_obj.size(); i++) {
		x = movable_obj[i]->GetCenterX() + dx;
		y = movable_obj[i]->GetCenterY() + dy;
		w = movable_obj[i]->GetWidth();
		h = movable_obj[i]->GetHeight();
		t = movable_obj[i]->GetTexture();
		Draw(x, y, w, h, t);
	}

	// Draw player
	Draw(size_x / 4, size_y * 2 / 5, player.GetWidth(), player.GetHeight(), player.GetTexture());

	// Print map
	for (int i = 0; i < size_y; i++) {
		WriteBuffer(0, i, map[i]);
	}

	// Print UI
	t = "┌─────────────────────────────────────────┐";
	WriteBuffer(size_x / 2 - 22, 0, t);
	t = "                                           ";
	WriteBuffer(size_x / 2 - 22, 1, t);
	t = "│";
	WriteBuffer(size_x / 2 - 22, 1, t);
	SetColor(1, 12);
	for (int i = 0; i < 3; i++) {
		if (i < life_count) {
			t = "♥";
		}
		else {
			t = "♡";
		}
		WriteBuffer(size_x / 2 - 20 + 2 * i, 1, t);
	}
	SetColor(1, 15);
	t = "Score : " + std::to_string(score) + "m";
	WriteBuffer(size_x / 2 - 6, 1, t);
	t = "ⓑ";
	SetColor(1, 10);
	for (int i = 0; i < boost_count; i++) {
		WriteBuffer(size_x / 2 + 14 + 2 * i, 1, t);
	}
	SetColor(1, 15);
	t = "│";
	WriteBuffer(size_x / 2 + 20, 1, t);
	t = "└─────────────────────────────────────────┘";
	WriteBuffer(size_x / 2 - 22, 2, t);

	// Print pause message
	t = "┌─────────────────────────┐";
	WriteBuffer(size_x / 2 - 14, size_y / 5, t);
	t = "│        Game  Over       │";
	WriteBuffer(size_x / 2 - 14, size_y / 5 + 1, t);
	t = "│                         │";
	WriteBuffer(size_x / 2 - 14, size_y / 5 + 2, t);
	t = "│   다시 시작하려면 space │";
	WriteBuffer(size_x / 2 - 14, size_y / 5 + 3, t);
	t = "└─────────────────────────┘";
	WriteBuffer(size_x / 2 - 14, size_y / 5 + 4, t);

	// Flip buffer
	FlipBuffer();
}
// Release buffers
void SceneManager::Release() {
	this->DeleteBuffer();
}
// Set background, texture color
void SceneManager::SetColor(unsigned char bg_color, unsigned char txt_color){
	if (bg_color > 15 || txt_color > 15) return;

	unsigned short ColorNum = (bg_color << 4) | txt_color;
	SetConsoleTextAttribute(hBuffer[nScreenIndex], ColorNum);
}
// Get width of the console
int SceneManager::GetWidth() {
	return size_x;
}
// Get height of the console
int SceneManager::GetHeight() {
	return size_y;
}