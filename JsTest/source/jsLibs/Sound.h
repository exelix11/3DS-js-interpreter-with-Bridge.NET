#pragma once
#include <3ds.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <dukglue/dukglue.h>

extern void SoundInit();
extern void SoundDispose();

struct Wave{
	u32 SampleRate;
	u32 Length;
	u8* Samples;
	u16 bitsPerSample;
	u16 channels;
	u16 ndspFormat;
};

class WavFile
{
    private:
        Wave loadedWave;
        bool Loaded = false;
    public:
        WavFile(const char * path);
        ~WavFile();
        void PlayOnChannel(u8 channel, bool loop);
        u8 Play(bool loop);
};

extern WavFile* SoundLoadWave(const char * path);
extern void SoundFreeAll();
extern void SoundFreeWave(WavFile *filePtr);
extern void SoundStopChannel(u8 channel);
extern void SoundStopAllChannels();
u8 FreeChannel();

void createDspBlock(ndspWaveBuf* waveBuf, u16 bps, u32 size, int loop, void* data);
void SoundPlay(Wave *w, bool loop ,u8 channel);