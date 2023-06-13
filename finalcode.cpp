#include <Windows.h>
#include <random>

int cps = 14;
bool smart_mode = true;

bool in_menu() { // Returns if user is in menu
	CURSORINFO info = { info.cbSize = sizeof(CURSORINFO) }; //Setting up output
	GetCursorInfo(&info); // Writing to variable 'info'
	return !(bool)((unsigned __int32)info.hCursor > 0x1FFFF); // Checking if the cursor handle has a value bigger than 0x1FFFF, usually if the cursor is invisible it will return a high value.
}

int random(int min, int max) { // Random function (https://stackoverflow.com/a/5020138)
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> x(min, max);

	return (int)x(rng);
}

void lmb_click(HWND window) { //Left mouse button click
	SendMessage(window, WM_LBUTTONDOWN, 0, 0);
	// No delay here to bypass click blocks on newer versions
	SendMessage(window, WM_LBUTTONUP, 0, 0);
}

int main() {
	while(true) { // Loop
		auto MinecraftWindow = FindWindow("LWJGL", nullptr); // This is in the loop because the window handle changes when toggling fullscreen.
		if(GetAsyncKeyState(VK_LBUTTON) < 0 && MinecraftWindow) { // VK_LBUTTON means left mouse button, if returned value is less than 0 then it's pressed down.
			if(GetForegroundWindow() == MinecraftWindow) { // Checks if the minecraft window is in focus
				if(smart_mode) { // Check if smart mode is enabled.
					if(!in_menu()) { // If it is check if the user isn't in a menu
						lmb_click(MinecraftWindow); // If not then just send the click.
					}
				}
				else { // If not just click.
					lmb_click(MinecraftWindow);
				}
				
				// Randomization part
				if (random(0, 100) < 50) { // 50%/50% to either drop or spike. 
					int delay = random((1000 / cps) / 6, (1000 / cps) / 3); // Lower spikes because of the 6/3 split.
					Sleep(1000.0 / cps - delay);
				}
				else {
					int delay = random((1000 / cps) / 5, (1000 / cps) / 2); // Higher drops because of the 5/2 split.
					Sleep(1000.0 / cps + delay);
				}
			}
		}
	}
}
