#include <xtl.h>
#include "GraphicsDevice.h"

// Define deadzone constant
#define XINPUT_DEADZONE 7849

// Game state
struct GameState
{
	HANDLE gamepad;
	float posX;
	float posY;
	float speed;
	LARGE_INTEGER frequency;
	LARGE_INTEGER lastTime;
	float deltaTime;
};

void Initialize(GameState* game)
{
	game->gamepad = NULL;
	game->posX = 350.0f;
	game->posY = 200.0f;
	game->speed = 200.0f;  // pixels per second
	game->deltaTime = 0.0f;

	// Initialize high-resolution timer
	QueryPerformanceFrequency(&game->frequency);
	QueryPerformanceCounter(&game->lastTime);

	// Initialize input devices
	XDEVICE_PREALLOC_TYPE devices[] = {
		{XDEVICE_TYPE_GAMEPAD, 4}
	};
	XInitDevices(sizeof(devices) / sizeof(XDEVICE_PREALLOC_TYPE), devices);
}

void CalculateDeltaTime(GameState* game)
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	game->deltaTime = (float)(currentTime.QuadPart - game->lastTime.QuadPart) / (float)game->frequency.QuadPart;

	game->lastTime = currentTime;

	// Cap delta time to prevent huge jumps (e.g., when debugging)
	if (game->deltaTime > 0.1f)
		game->deltaTime = 0.1f;
}

void ReadInput(GameState* game)
{
	// Open controller 0 if not already open
	if (game->gamepad == NULL)
	{
		if (XGetDevices(XDEVICE_TYPE_GAMEPAD) & XDEVICE_PORT0_MASK)
		{
			game->gamepad = XInputOpen(XDEVICE_TYPE_GAMEPAD, XDEVICE_PORT0,
				XDEVICE_NO_SLOT, NULL);
		}
	}

	// Read input
	if (game->gamepad)
	{
		XINPUT_STATE state;
		if (XInputGetState(game->gamepad, &state) == ERROR_SUCCESS)
		{
			// D-Pad movement
			if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
				game->posY -= game->speed * game->deltaTime;
			if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				game->posY += game->speed * game->deltaTime;
			if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
				game->posX -= game->speed * game->deltaTime;
			if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
				game->posX += game->speed * game->deltaTime;

			// Analog stick movement
			SHORT thumbX = state.Gamepad.sThumbLX;
			SHORT thumbY = state.Gamepad.sThumbLY;

			// Apply deadzone
			if (thumbX > XINPUT_DEADZONE || thumbX < -XINPUT_DEADZONE)
			{
				game->posX += (thumbX / 32768.0f) * game->speed * game->deltaTime;
			}
			if (thumbY > XINPUT_DEADZONE || thumbY < -XINPUT_DEADZONE)
			{
				game->posY -= (thumbY / 32768.0f) * game->speed * game->deltaTime;
			}
		}
		else
		{
			// Controller disconnected
			XInputClose(game->gamepad);
			game->gamepad = NULL;
		}
	}
}

void Update(GameState* game)
{
	// Keep box on screen
	if (game->posX < 0)
		game->posX = 0;

	if (game->posY < 0) 
		game->posY = 0;

	if (game->posX > 640 - 80) 
		game->posX = 640 - 80;

	if (game->posY > 480 - 150) 
		game->posY = 480 - 150;
}

void Draw(GraphicsDevice* graphics, GameState* game)
{
	graphics->Clear(D3DCOLOR_XRGB(0, 64, 128));
	graphics->Begin();

	// Draw yellow box
	graphics->DrawQuad(100, 100, 200, 100, D3DCOLOR_XRGB(255, 255, 0));

	// Draw green box
	graphics->DrawQuad((int)game->posX, (int)game->posY, 80, 150,
		D3DCOLOR_XRGB(0, 255, 0));

	graphics->End();
	graphics->Present();
}

void Shutdown(GameState* game)
{
	if (game->gamepad)
	{
		XInputClose(game->gamepad);
		game->gamepad = NULL;
	}
}

void __cdecl main()
{
	GraphicsDevice graphics;
	if (!graphics.Initialize())
		return;

	GameState game;
	Initialize(&game);

	// Game loop
	while (TRUE)
	{
		CalculateDeltaTime(&game);
		ReadInput(&game);
		Update(&game);
		Draw(&graphics, &game);
	}

	Shutdown(&game);
}