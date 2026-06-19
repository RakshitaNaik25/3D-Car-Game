#ifndef GBPS_AUDIO_H
#define GBPS_AUDIO_H
/*
audio.h
Handles audio playback for the game using Windows Multimedia API
copyright (c) 2024 GBPS Team
*/

#include <windows.h>
#include <mmsystem.h>

// Sound types
typedef enum {
    SOUND_ENGINE = 0,
    SOUND_CRASH,
    SOUND_GAMEOVER,
    SOUND_COUNT
} SoundType;

// Audio state
extern int gbps_AudioEnabled;
extern float gbps_CurrentEngineSpeed; // For smooth engine sound transitions

// Initialize audio system
void gbps_InitAudio();

// Cleanup audio resources
void gbps_CleanupAudio();

// Play sounds
void gbps_PlayEngineSound();
void gbps_PlayCrashSound();
void gbps_PlayGameOverSound();
void gbps_PlayMenuMoveSound();
void gbps_PlayMenuSelectSound();

// Update engine sound based on car speed
void gbps_UpdateEngineSound(float speed);

// Stop all sounds
void gbps_StopAllSounds();

// Pause/Resume for Pause menu
void gbps_PauseAllSounds();
void gbps_ResumeAllSounds();

// Stop specific sound
void gbps_StopEngineSound();

// Toggle audio ON/OFF
void gbps_ToggleAudio();

// Persistence
void gbps_SaveAudioSettings();
void gbps_LoadAudioSettings();

#endif
