#pragma once

#include "global/types.h"

#include <stdbool.h>
#include <stdint.h>

bool Sound_Init();
bool Sound_Shutdown();
bool Sound_Effect(int32_t sfx_num, PHD_3DPOS *pos, uint32_t flags);
bool Sound_StopEffect(int32_t sfx_num, PHD_3DPOS *pos);
void Sound_UpdateEffects();
void Sound_ResetEffects();
void Sound_ResetAmbientLoudness();
void Sound_StopAmbientSounds();
void Sound_StopAllSamples();
void Sound_SetMasterVolume(int8_t volume);
void Sound_LoadSamples(
    size_t num_samples, const char **sample_pointers, size_t *sizes);
