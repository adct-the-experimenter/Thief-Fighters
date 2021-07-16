#include "SoundSystem.h"

#define MINIAUDIO_IMPLEMENTATION
#define MA_ENABLE_ALSA 
#define MA_ENABLE_WINMM
#define MA_ENABLE_SNDIO 

#include "core/miniaudio.h"

#include <stdio.h>

#define SAMPLE_FORMAT   ma_format_f32
#define CHANNEL_COUNT   2
#define SAMPLE_RATE     48000



//Audio Device
static ma_device main_audio_device;


static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);

    (void)pInput;
}



bool SoundSystem::Init()
{
	sound_system_initialized = false;
	//initialize audio device here
	
	//set up device configuration
    ma_device_config deviceConfig;
	
	deviceConfig = ma_device_config_init(ma_device_type_playback);
	
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = SAMPLE_FORMAT;
    deviceConfig.playback.channels = CHANNEL_COUNT;
    deviceConfig.sampleRate        = SAMPLE_RATE;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = NULL;
	
	//intialize main audio device
    if (ma_device_init(NULL, &deviceConfig, &main_audio_device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        return false;
    }
	
	//start main audio device
    if (ma_device_start(&main_audio_device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&main_audio_device);
        return false;
    }
	
	sound_system_initialized = true;
	return true;
}

void SoundSystem::Update_VersusMode()
{
	//play sound based on sound id received
}
	
void SoundSystem::Update_MetroidVaniaMode()
{
	//play sound based on sound id received
	
}

void SoundSystem::Close()
{
	if(sound_system_initialized)
	{
		ma_device_uninit(&main_audio_device);
	}
}
