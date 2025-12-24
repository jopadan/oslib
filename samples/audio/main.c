#include <pspkernel.h>
#include <oslib/oslib.h>

PSP_MODULE_INFO("Audio Sample", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(12 * 1024);

// Sound formats
#define FORMAT_WAV 0
#define FORMAT_BGM 1

// Load modes
#define MODE_RAM    0
#define MODE_STREAM 1

// Audio files
#define WAV_FILE "Resources/jump.wav"
#define BGM_FILE "Resources/music.bgm"

// Current settings
int currentFormat = FORMAT_BGM;
int currentMode = MODE_STREAM;
int inMenu = 1;

OSL_SOUND *sound = NULL;
OSL_IMAGE *bkg = NULL;

// Function prototypes
void DrawMenu();
void DrawPlayer();
void HandleMenuKeys();
void HandlePlayerKeys();
void LoadSound();
void UnloadSound();

const char* GetFormatName() {
	return currentFormat == FORMAT_WAV ? "WAV" : "BGM";
}

const char* GetModeName() {
	return currentMode == MODE_RAM ? "RAM" : "Stream";
}

int main(int argc, char* argv[])
{
	// Initialization
	oslInit(0);
	oslInitGfx(OSL_PF_8888, 1);
	oslInitConsole();
	oslInitAudio();
	oslSetQuitOnLoadFailure(1);

	// Configure joypad
	oslSetKeyAutorepeatInit(40);
	oslSetKeyAutorepeatInterval(10);

	// Load background
	bkg = oslLoadImageFile("bkg.png", OSL_IN_RAM, OSL_PF_8888);

	while (!osl_quit)
	{
		oslStartDrawing();

		// Draw background or gradient
		if (bkg) {
			oslDrawImageXY(bkg, 0, 0);
		} else {
			oslDrawGradientRect(0, 0, 480, 272,
			                    RGB(0, 0, 64), RGB(0, 0, 64),
			                    RGB(0, 64, 128), RGB(0, 64, 128));
		}

		if (inMenu) {
			DrawMenu();
			HandleMenuKeys();
		} else {
			DrawPlayer();
			HandlePlayerKeys();
		}

		oslEndDrawing();
		oslSyncFrame();
		oslAudioVSync();
	}

	// Cleanup
	UnloadSound();
	if (bkg) oslDeleteImage(bkg);
	oslEndGfx();
	oslQuit();

	return 0;
}

void DrawMenu()
{
	oslSetTextColor(RGBA(255, 255, 255, 255));
	oslSetBkColor(RGBA(0, 0, 0, 128));

	oslPrintf_xy(10, 10, "=== Audio Sample - Select Options ===");

	// Format selection
	oslPrintf_xy(10, 50, "Format:  < %s >", GetFormatName());
	oslPrintf_xy(10, 70, "         (Left/Right to change)");

	// Mode selection
	oslPrintf_xy(10, 100, "Mode:    < %s >", GetModeName());
	oslPrintf_xy(10, 120, "         (Up/Down to change)");

	// File that will be loaded
	oslPrintf_xy(10, 160, "File: %s",
	             currentFormat == FORMAT_WAV ? WAV_FILE : BGM_FILE);

	// Instructions
	oslPrintf_xy(10, 200, "Press X to start playing");
	oslPrintf_xy(10, 220, "Press START to quit");
}

void DrawPlayer()
{
	oslSetTextColor(RGBA(255, 255, 255, 255));
	oslSetBkColor(RGBA(0, 0, 0, 128));

	oslPrintf_xy(10, 10, "=== Audio Player ===");
	oslPrintf_xy(10, 30, "Format: %s | Mode: %s", GetFormatName(), GetModeName());

	// Status
	if (sound) {
		int channel = oslGetSoundChannel(sound);
		if (channel >= 0) {
			if (osl_audioActive[channel] == 2) {
				oslPrintf_xy(10, 60, "Status: PAUSED (Channel %d)", channel);
			} else if (osl_audioActive[channel] == 1) {
				oslPrintf_xy(10, 60, "Status: PLAYING (Channel %d)", channel);
			} else {
				oslPrintf_xy(10, 60, "Status: STOPPED");
			}
		} else {
			oslPrintf_xy(10, 60, "Status: STOPPED");
		}
	} else {
		oslPrintf_xy(10, 60, "Status: NOT LOADED");
	}

	// Controls
	oslPrintf_xy(10, 100, "Controls:");
	oslPrintf_xy(10, 120, "  O = Play/Resume");
	oslPrintf_xy(10, 140, "  [] = Pause/Unpause");
	oslPrintf_xy(10, 160, "  /\\ = Stop");
	oslPrintf_xy(10, 180, "  X = Back to menu");
	oslPrintf_xy(10, 220, "  START = Quit");
}

void HandleMenuKeys()
{
	oslReadKeys();

	// Change format
	if (osl_keys->pressed.left || osl_keys->pressed.right) {
		currentFormat = (currentFormat == FORMAT_WAV) ? FORMAT_BGM : FORMAT_WAV;
	}

	// Change mode
	if (osl_keys->pressed.up || osl_keys->pressed.down) {
		currentMode = (currentMode == MODE_RAM) ? MODE_STREAM : MODE_RAM;
	}

	// Start playing
	if (osl_keys->pressed.cross) {
		LoadSound();
		if (sound) {
			oslSetSoundLoop(sound, 1);
			oslPlaySound(sound, 0);
		}
		inMenu = 0;
	}

	// Quit
	if (osl_keys->pressed.start) {
		oslQuit();
	}
}

void HandlePlayerKeys()
{
	oslReadKeys();

	if (sound) {
		// Play/Resume
		if (osl_keys->pressed.circle) {
			int channel = oslGetSoundChannel(sound);
			if (channel < 0) {
				oslPlaySound(sound, 0);
			} else if (osl_audioActive[channel] == 2) {
				oslPauseSound(sound, 0); // Unpause
			}
		}

		// Pause/Unpause
		if (osl_keys->pressed.square) {
			oslPauseSound(sound, -1); // Toggle
		}

		// Stop
		if (osl_keys->pressed.triangle) {
			oslStopSound(sound);
		}
	}

	// Back to menu
	if (osl_keys->pressed.cross) {
		UnloadSound();
		inMenu = 1;
	}

	// Quit
	if (osl_keys->pressed.start) {
		oslQuit();
	}
}

void LoadSound()
{
	UnloadSound();

	const char* filename;
	int streamFlag;

	if (currentFormat == FORMAT_WAV) {
		filename = WAV_FILE;
	} else {
		filename = BGM_FILE;
	}

	streamFlag = (currentMode == MODE_STREAM) ? OSL_FMT_STREAM : OSL_FMT_NONE;

	sound = oslLoadSoundFile(filename, streamFlag);
}

void UnloadSound()
{
	if (sound) {
		oslStopSound(sound);
		oslDeleteSound(sound);
		sound = NULL;
	}
}
