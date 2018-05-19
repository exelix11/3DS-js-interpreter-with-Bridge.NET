#include "Sound.h"
#include <algorithm>

void SoundInit()
{
    ndspInit();
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);
}

void SoundDispose()
{
    ndspExit();
}

inline u8 FreeChannel()
{
    for (u8 i = 0; i <23; i++) if (!ndspChnIsPlaying(i)) return i;
    return 22;
}

void SoundStopChannel(u8 channel) {
    if (channel >= 23) return;
	ndspChnReset(channel);
}

void SoundStopAllChannels() {
	for (u8 i = 0; i <23; i++) ndspChnReset(i);
}


ndspWaveBuf waveBuffs[23];
inline void createDspBlock(ndspWaveBuf* waveBuf, u16 bps, u32 size, int loop, void* data){
	waveBuf->data_vaddr = data;
	waveBuf->nsamples = size / (bps/8); 
	waveBuf->looping = loop;
	waveBuf->offset = 0;	
	DSP_FlushDataCache(data, size);
}

void SoundPlay(Wave *w, bool loop,u8 channel) 
{
    u8 ch = (channel >= 23 ? FreeChannel() : channel); 
	SoundStopChannel(ch);
	ndspChnWaveBufClear(ch);
	ndspChnInitParams(ch);
	ndspChnSetInterp(ch, NDSP_INTERP_LINEAR);
	ndspChnSetRate(ch, w->SampleRate);
	ndspChnSetFormat(ch, w->ndspFormat); 
	createDspBlock(&waveBuffs[ch], w->bitsPerSample * w->channels , w->Length, loop ? 1 : 0, (void*)w->Samples);
	ndspChnWaveBufAdd(ch, &waveBuffs[ch]);
}

std::vector<WavFile*> LoadedWavs;
extern WavFile* SoundLoadWave(const char * path)
{
	WavFile* res = new WavFile(path);
	LoadedWavs.push_back(res);
	return res;
}

extern duk_context *mainCtx; //from main.cpp
void SoundFreeWave(WavFile *filePtr)
{
	auto it = std::find(LoadedWavs.begin(), LoadedWavs.end(), filePtr);
	if (it != LoadedWavs.end())
	{ 
    	dukglue_invalidate_object(mainCtx, filePtr);  
		LoadedWavs.erase(it);
	} 
    delete filePtr;
}

extern void SoundFreeAll()
{
	SoundStopAllChannels();
	for (std::vector<WavFile*>::iterator it = LoadedWavs.begin() ; it != LoadedWavs.end(); ++it)
	{
    	dukglue_invalidate_object(mainCtx, *it);  
    	delete *it;
	}
	LoadedWavs.clear();
}

WavFile::WavFile(const char * path) 
{
    FILE *file = fopen(path, "r");
	if(!file) 
	{
		printf("File error\n");
	}	
	//check signature
	char sig[4];
	fread(sig, 1, 4, file);
	if(!(sig[0] == 'R' && sig[1] == 'I' && sig[2] == 'F' && sig[3] == 'F')){
		printf("no Sign %d %d %d %d \n",sig[0],sig[1],sig[2],sig[3] );
		fclose(file);
	}
	//file sizes and metadata 
	fseek(file, 40, SEEK_SET);
	fread(&(loadedWave.Length), 4, 1, file);
	fseek(file, 22, SEEK_SET);
	fread(&(loadedWave.channels), 2, 1, file);
	fseek(file, 24, SEEK_SET);
	fread(&(loadedWave.SampleRate), 4, 1, file);
	fseek(file, 34, SEEK_SET);
	fread(&(loadedWave.bitsPerSample), 2, 1, file);
	loadedWave.Samples = (u8*)linearAlloc(loadedWave.Length);
    
	fseek(file, 44, SEEK_SET);
	fread(loadedWave.Samples, 1, loadedWave.Length, file);
	fclose(file);

	if(loadedWave.bitsPerSample == 8) {
		loadedWave.ndspFormat = (loadedWave.channels == 1) ? NDSP_FORMAT_MONO_PCM8  : NDSP_FORMAT_STEREO_PCM8 ;
	} else {
		loadedWave.ndspFormat = (loadedWave.channels == 1) ? NDSP_FORMAT_MONO_PCM16 : NDSP_FORMAT_STEREO_PCM16;
	}
	Loaded = true;
}

WavFile::~WavFile()
{
    if (!Loaded) return;
    linearFree(loadedWave.Samples);
    Loaded = false; //useless
    printf("Wave destroyed\n");
}

void WavFile::PlayOnChannel(u8 channel, bool loop)
{
    if (!Loaded) return;
    SoundPlay(&loadedWave, loop, channel);
}

u8 WavFile::Play(bool loop)
{
    if (!Loaded) return 0xFF;
    u8 freech = FreeChannel();
    SoundPlay(&loadedWave, loop, freech);
    return freech;
}