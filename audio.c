#include "audio.h"
#include <stdio.h>

// Audio state variables
int gbps_AudioEnabled = 0;
float gbps_CurrentEngineSpeed = 0.0f;

// Audio file paths - MODIFY THESE PATHS TO YOUR AUDIO FILES
// Audio file paths - Updated to handle MP3s added by user
static const char* AUDIO_ENGINE = "res\\sounds\\engine.mp3";
static const char* AUDIO_CRASH = "res\\sounds\\crash.mp3";
static const char* AUDIO_GAMEOVER = "res\\sounds\\gameover.mp3";
static const char* AUDIO_MENU_MOVE = "res\\sounds\\menu_move.mp3";
static const char* AUDIO_MENU_SELECT = "res\\sounds\\menu_select.mp3";

// Track if sounds are currently playing
static int enginePlaying = 0;
static int crashPlayed = 0;

/*
 * Initialize the audio system
 * NOTE: Place your .wav files in res/sounds/ directory:
 *   - engine.wav (loopable engine sound)
 *   - crash.wav (crash sound effect)
 *   - gameover.wav (game over sound)
 */
void gbps_InitAudio()
{
    // Load setting first
    gbps_LoadAudioSettings();

    // Check if critical audio files exist
    int filesFound = 0;
    const char* criticalFiles[] = { AUDIO_ENGINE, AUDIO_CRASH, AUDIO_GAMEOVER };
    for (int i = 0; i < 3; i++) {
        FILE* f = fopen(criticalFiles[i], "rb");
        if (f) {
            fclose(f);
            filesFound++;
        }
    }

    if (filesFound == 3) {
        printf("Audio System: Ready (using MCI for MP3)\n");
    } else {
        gbps_AudioEnabled = 0;
        printf("Audio System: Limited or Disabled (%d/3 critical files found)\n", filesFound);
        printf("Please ensure engine.mp3, crash.mp3, and gameover.mp3 are in res/sounds/\n");
    }
    
    // Close any previous instances just in case
    mciSendString("close all", NULL, 0, NULL);
    
    gbps_CurrentEngineSpeed = 0.0f;
    enginePlaying = 0;
    crashPlayed = 0;
}

/*
 * Cleanup audio resources
 */
void gbps_CleanupAudio()
{
    gbps_StopAllSounds();
    gbps_AudioEnabled = 0;
}

/*
 * Play engine sound (looping)
 */
void gbps_PlayEngineSound()
{
    if (!gbps_AudioEnabled) return;
    
    if (!enginePlaying) {
        char cmd[256];
        mciSendString("close engine", NULL, 0, NULL); // Safety: close if already open
        sprintf(cmd, "open \"%s\" type mpegvideo alias engine", AUDIO_ENGINE);
        mciSendString(cmd, NULL, 0, NULL);
        mciSendString("play engine repeat", NULL, 0, NULL);
        enginePlaying = 1;
    }
}

/*
 * Update engine sound based on car speed
 * This adjusts the volume and can be extended for pitch control
 */
void gbps_UpdateEngineSound(float speed)
{
    if (!gbps_AudioEnabled) return;
    
    // Smooth speed transitions
    if (gbps_CurrentEngineSpeed < speed) {
        gbps_CurrentEngineSpeed += 0.1f;
        if (gbps_CurrentEngineSpeed > speed) 
            gbps_CurrentEngineSpeed = speed;
    } else if (gbps_CurrentEngineSpeed > speed) {
        gbps_CurrentEngineSpeed -= 0.1f;
        if (gbps_CurrentEngineSpeed < speed) 
            gbps_CurrentEngineSpeed = speed;
    }
    
    // Start engine sound if not already playing
    if (speed > 0.1f && !enginePlaying) {
        gbps_PlayEngineSound();
    }
    
    // Stop engine if speed is very low
    if (speed < 0.1f && enginePlaying) {
        gbps_StopEngineSound();
    }
    
    // Note: Windows Multimedia API has limited pitch control
    // For dynamic pitch, consider using OpenAL or FMOD in future
    // Current implementation provides basic engine sound feedback
}

/*
 * Play crash sound effect (one-shot)
 */
void gbps_PlayCrashSound()
{
    if (!gbps_AudioEnabled) return;
    
    if (!crashPlayed) {
        // Stop engine sound
        gbps_StopEngineSound();
        mciSendString("close crash", NULL, 0, NULL); // Safety
        
        char cmd[256];
        sprintf(cmd, "open \"%s\" type mpegvideo alias crash", AUDIO_CRASH);
        mciSendString(cmd, NULL, 0, NULL);
        mciSendString("play crash", NULL, 0, NULL);
        crashPlayed = 1;
    }
}

/*
 * Play game over sound (one-shot)
 */
void gbps_PlayGameOverSound()
{
    if (!gbps_AudioEnabled) return;
    
    char cmd[256];
    mciSendString("close gameover", NULL, 0, NULL); // Safety
    sprintf(cmd, "open \"%s\" type mpegvideo alias gameover", AUDIO_GAMEOVER);
    mciSendString(cmd, NULL, 0, NULL); // Actually open the file
    mciSendString("play gameover", NULL, 0, NULL);
}

/*
 * Play menu move sound (one-shot)
 */
void gbps_PlayMenuMoveSound()
{
    if (!gbps_AudioEnabled) return;
    
    char cmd[256];
    mciSendString("close menumove", NULL, 0, NULL); // Safety
    sprintf(cmd, "open \"%s\" type mpegvideo alias menumove", AUDIO_MENU_MOVE);
    mciSendString(cmd, NULL, 0, NULL);
    mciSendString("play menumove", NULL, 0, NULL);
}

/*
 * Play menu select sound (one-shot)
 */
void gbps_PlayMenuSelectSound()
{
    if (!gbps_AudioEnabled) return;
    
    char cmd[256];
    mciSendString("close menuselect", NULL, 0, NULL); // Safety
    sprintf(cmd, "open \"%s\" type mpegvideo alias menuselect", AUDIO_MENU_SELECT);
    mciSendString(cmd, NULL, 0, NULL);
    mciSendString("play menuselect", NULL, 0, NULL);
}

/*
 * Stop engine sound
 */
void gbps_StopEngineSound()
{
    if (!gbps_AudioEnabled) return;
    
    if (enginePlaying) {
        mciSendString("stop engine", NULL, 0, NULL);
        mciSendString("close engine", NULL, 0, NULL);
        enginePlaying = 0;
    }
}

/*
 * Stop all sounds
 */
void gbps_StopAllSounds()
{
    mciSendString("stop engine", NULL, 0, NULL);
    mciSendString("close engine", NULL, 0, NULL);
    mciSendString("stop crash", NULL, 0, NULL);
    mciSendString("close crash", NULL, 0, NULL);
    mciSendString("stop gameover", NULL, 0, NULL);
    mciSendString("close gameover", NULL, 0, NULL);
    mciSendString("stop menumove", NULL, 0, NULL);
    mciSendString("close menumove", NULL, 0, NULL);
    mciSendString("stop menuselect", NULL, 0, NULL);
    mciSendString("close menuselect", NULL, 0, NULL);
    
    enginePlaying = 0;
    crashPlayed = 0;
    gbps_CurrentEngineSpeed = 0.0f;
}

void gbps_PauseAllSounds()
{
    if (!gbps_AudioEnabled) return;
    mciSendString("pause engine", NULL, 0, NULL);
    mciSendString("pause crash", NULL, 0, NULL);
    mciSendString("pause gameover", NULL, 0, NULL);
}

void gbps_ResumeAllSounds()
{
    if (!gbps_AudioEnabled) return;
    if (enginePlaying) mciSendString("resume engine", NULL, 0, NULL);
    mciSendString("resume crash", NULL, 0, NULL);
    mciSendString("resume gameover", NULL, 0, NULL);
}

void gbps_ToggleAudio()
{
    gbps_AudioEnabled = !gbps_AudioEnabled;
    if (!gbps_AudioEnabled) {
        gbps_StopAllSounds();
    }
    gbps_SaveAudioSettings();
}

void gbps_SaveAudioSettings()
{
    FILE* f = fopen("settings.txt", "w");
    if (f) {
        fprintf(f, "%d", gbps_AudioEnabled);
        fclose(f);
    }
}

void gbps_LoadAudioSettings()
{
    FILE* f = fopen("settings.txt", "r");
    if (f) {
        if (fscanf(f, "%d", &gbps_AudioEnabled) != 1) {
            gbps_AudioEnabled = 1; // Default ON
        }
        fclose(f);
    } else {
        gbps_AudioEnabled = 1; // Default ON if no file
    }
}
