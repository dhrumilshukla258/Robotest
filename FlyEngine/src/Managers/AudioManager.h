/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Ivan Cazares, ivan.c, 60004418
- End Header --------------------------------------------------------*/

#pragma once
#include "fmod/fmod.hpp"
using namespace std;
typedef std::map<std::string, FMOD::Sound*> SoundMap;
typedef map<int, FMOD::Channel*> ChannelMap;

class AudioManager {
public:
	AudioManager();
	~AudioManager();
	void Update(float elapsed);// clears all unused channels
	//void Update(unsigned int id);//  just to play a sound fx, should be deleted after
	void DefineLoop(int channelID, int times);//a sound that already is playing,  modify repetitions
	int ErrorCheck(FMOD_RESULT result); // ask FMOD actual status, to see possible errors
	void LevelShiftChanges();
	void Load(string sound); // load a sound
	void LoadLoop(string path); // load sound and make it loopable
	void Mute(int channelID);
	void Pause(int channelID);
	void PauseAll();
	int Play(string path);//one shoot play
	int Play(string path, int loops);//possible many loops -1 =infinite,(for this , the audio must be previously Loaded as "LoadLoop")
	void PlaySFX(const std::string& path, float minVolume, float maxVolume, float minPitch, float maxPitch);// same sound. varies volume and pitch
	void Resume(int channelID);
	void SetPan(int channelID, float fPan);//locate 2Dsound left or right  (-1,1)
	void Stop(int channelID);
	void StopAll();
	void Stream(string path); //uses original location of sound, less memory, more process
	void ToggleAllPause(int channelID);//Pause/resume a sound 
	void ToggleAll();//Pause/resume every sound 
	void ToggleMuteAll();
	void ToggleMuteBG();
	void TogglePause(int channelID);//Pause/resume a sound 
	void Unload(string path); // release sound from memory 
	void Volume(int channelID, float value); //(0 to 1)

	SoundMap* GetSoundMap() { return &sounds; }

public:
	std::vector<int> mLoadLoopChannels;

private:
	void LoadOrStream(const std::string& path, bool stream);
	FMOD::System* system;
	SoundMap sounds;
	ChannelMap channels;
	int numberOfChannels = 64;
	int mnNextChannelId = 0;
	bool pauseEnabled = false;//use for toggling all sounds to only a background sound
};