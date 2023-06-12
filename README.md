## Introduction
This tutorial will help you make an autoclicker, also this was mostly made for beginners who don't know on where to start. (If you are a beginner I strongly advise you to follow the tutorial rather then pasting the codes provided, so you can understand the basics and the way things work)

**Features**
 - Only click in game window
 - Smart mode (Only click when not in menus)
 - Randomization (Bypasses anticheats)
   - Spike
   - Drop
 - No Hit Delay (Bypass hit blocks by game)

**NOTE:** I'll update this and add more functions like string cleaning, but right now that's all (I'll also add just codesnippets in the future that you can use). 
**NOTE #2:** Feel free to open issues about missing explanations or lack of detail on explainations.

## Basics
### 1.0) Basics
First in your project you should set the `Character Set` to `Use Multi-Byte Character Set`. (You can do this in `Project > Property >  Advanced`)

Now your code right now probably looks like this:
```cpp
int main() {

}
```

### 1.1) Finding game window
Finding the game window of Minecraft is a necessary part of writing an autoclicker.
This is how you should do it.
```cpp
#include <Windows.h> // Importing Windows.h because it has the FindWindow function.

int main() {
	auto MinecraftWindow = FindWindow("LWJGL", nullptr); // Handle of Game Window
}
```
"LWJGL" is Minecraft's window class, so it works universally regardless of version.

### 1.2) Looping, detecting if mouse button is down
Looping is neccessary for detecting if the user holds the LMB down, because we use `GetAsyncKeyState`
Okay, now we implement it.

Code with Step 1.2 implemented should look like this:
```cpp
#include <Windows.h>

int main() {
	while(true) { // Loop
		auto MinecraftWindow = FindWindow("LWJGL", nullptr); // This is in the loop because the window handle changes when toggling fullscreen.
		if(GetAsyncKeyState(VK_LBUTTON) < 0 && MinecraftWindow) { // VK_LBUTTON means left mouse button, if returned value is less than 0 then it's pressed down. MinecraftWindow isn't a boolean but a check if it's not invalid.
			//rest goes here
		}
	}
}
```

### 1.3) Checking active window
This function serves whether the user has the game in focus or no.
It's recommended to use the `GetForegroundWindow()` function to check the window in focus (returns window handle).
Then we compare the returned value of `GetForegroundWindow()` and `MinecraftWindow`.

Code with Step 1.3 implemented should look like this:
```cpp
#include <Windows.h>

int main() {
	while(true) { // Loop
		auto MinecraftWindow = FindWindow("LWJGL", nullptr); // This is in the loop because the window handle changes when toggling fullscreen.
		if(GetAsyncKeyState(VK_LBUTTON) < 0 && MinecraftWindow) { // VK_LBUTTON means left mouse button, if returned value is less than 0 then it's pressed down.
			if(GetForegroundWindow() == MinecraftWindow) { // Checks if the minecraft window is in focus
				//rest goes here
			}
		}
	}
}
```

### 1.4) Implementing mouse clicks with No Hit Delay
The most important thing in whole Autoclickers is to actually click, so we have to implement clicking.
**Best way** to click is by **`SendMessage`** which basically fakes a click into your game window, and it will only affect your game. There are **other ways** to send a click but they're **not recommended** like **``mouse_event``**.
**CAUTION:** This code will hit a 100+ cps since it doesn't have a delay added, follow the next step.

Code with Step 1.4 implemented should look like this:
```cpp
#include <Windows.h>

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
				lmb_click(MinecraftWindow); // Sends click when LMB is down and window is in focus.
			}
		}
	}
}
```

### 1.5) Setting a CPS limit
It is important to set a CPS limit if you don't wanna get banned and to control the clicker so I'll show you.
It's super simple you just have to set a delay using `Sleep`, with the formula `1000 / cps`.

```cpp
#include <Windows.h>

int cps = 14;

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
				lmb_click(MinecraftWindow); // Sends click when LMB is down and window is in focus.
				Sleep(1000 / cps);
			}
		}
	}
}
```

### 1.6) Adding randomization
Randomization is **very important to add**, because heuristic anti-cheat checks can detect non-randomized autoclickers, because of the lack of behavior change in the clicking pattern.
This randomization I provided generates a random number between 0-100 and check if it's less than 50 or more (so **it's 50/50**), this stage **determines whether it should drop or spike**. When spike is generated it **works the following way**: Get the **sixth of the CPS Delay** and the **third of the CPS Delay**, then just generate a **random number between them** so it has a wide range of numbers. It works almost the same with  **drop**, all difference is it generates with the value **fifth of the CPS Delay** and the **half of the CPS Delay**.
**For your information:** Adding to a delay means less cps, subtracting from a delay means more cps.
```cpp
#include <Windows.h>
#include <random>

int cps = 14;

int random(int min, int max) { // Random function (https://stackoverflow.com/a/5020138)
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> x(min, max);

	return (int)x(rng);
} // I won't provide an explaination here because it doesn't matter.

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
				lmb_click(MinecraftWindow); // Sends click when LMB is down and window is in focus.
	
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
```

### 1.7) Implementing `Smart Mode`
Smart mode is an important extra to avoid accidental clicks, it's **easy to implement** and **very encouraged to add**.
This method has a simple way of working without using memory. It checks if the cursor is hidden or visible on the screen with `GetCursorInfo`, if the cursor is visible it means the player is in a menu, if it isn't then the user is not in a menu.

```cpp
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
```

**Congrats!** Now you have a simple auto-clicker! 🎊
