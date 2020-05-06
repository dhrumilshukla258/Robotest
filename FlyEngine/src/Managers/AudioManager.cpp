/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Ivan Cazares, ivan.c, 60004418
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "AudioManager.h"

#include "../InitData.h"
#include "InputManager.h"
extern InputManager* gpInputManager;

bool isAllMute = false;
bool isBackgroundMute = false;

float RandomBetween(float, float);
float ChangeSemitone(float, float);

AudioManager::AudioManager()
{
	FMOD::System_Create(&system);
	system->init(numberOfChannels, FMOD_INIT_NORMAL, nullptr);//100 channels
}

//to maintain control over the function
AudioManager::~AudioManager()
{
	// Release every sound object and clear the map
	SoundMap::iterator iter;
	for (iter = sounds.begin(); iter != sounds.end(); ++iter)
		iter->second->release();
	sounds.clear();
	// Release the system object
	system->release();
	system = 0;
}

// Release resources
void AudioManager::Update(float elapsed)
{
	if (gpInputManager->isTriggered(M)) {
		ToggleMuteBG();
	}

	vector<ChannelMap::iterator> pStoppedChannels;
	bool bIsPlaying = false;
	for (auto it = channels.begin(), itEnd = channels.end(); it != itEnd; ++it) {
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying)
			pStoppedChannels.push_back(it);
	}
	for (auto& it : pStoppedChannels)//releases all channels that are not actually playing
		channels.erase(it);

	AudioManager::ErrorCheck(system->update());//run update and check for erors
}

void AudioManager::DefineLoop(int channelID, int times)
{
	FMOD::Channel* pChannel = nullptr;
	pChannel = channels[channelID];
	channels[channelID]->setLoopCount(times);
}

//ask system for problems
int AudioManager::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		printf("FMOD ERROR " + result);
		return 1;
	}
	//printf("FMOD all good ");
	return 0;
}

void AudioManager::Load(string sound)
{
	LoadOrStream(sound, false);
}

void AudioManager::LoadLoop(std::string path)
{
	if (sounds.find(path) != sounds.end()) return;
	FMOD::Sound* sound;
	const std::string& path2 = directory::audio + path;//relative to project folder
	system->createSound(
		path2.c_str(),
		FMOD_LOOP_NORMAL,
		nullptr,
		&sound //storing address
	);

	sounds.insert(std::make_pair(path, sound));// Store sound object in the map using the path as key
}

void AudioManager::LevelShiftChanges()
{
	if (isBackgroundMute) {
		isBackgroundMute = false;
		ToggleMuteBG();
	}
}

//decides if load to memory or stream directly from the source
void AudioManager::LoadOrStream(const std::string& name, bool stream)
{
	// Ignore call if sound is already loaded
	if (sounds.find(name) != sounds.end()) return;
	// Load (or stream) file into a sound object
	FMOD::Sound* sound;
	const std::string& path2 = directory::audio + name;//relative to project folder
	if (stream)
		system->createStream(
			path2.c_str(),//name
			FMOD_DEFAULT,//sound nature loop, hardware accelerator,2D/3D
			nullptr, // special paramethers when loading a subsound form larger, midi dls sample set. etc,,, basically dont touch now
			&sound//
		);
	else
		system->createSound(
			path2.c_str(),
			FMOD_DEFAULT,
			nullptr,
			&sound //storing address
		);
	// Store the sound object in the map using the path as key
	sounds.insert(std::make_pair(name, sound));
}

void  AudioManager::Mute(int channelID)
{
	FMOD::Channel* pChannel = nullptr;
	pChannel = channels[channelID];
	channels[channelID]->setVolume(0);
}

void AudioManager::Pause(int channelID)
{
	FMOD::Channel* pChannel = nullptr;
	pChannel = channels[channelID];
	channels[channelID]->setPaused(true);
}

void AudioManager::PauseAll()
{
	FMOD::Channel* pChannel = nullptr;
	for (int i = 0; i < numberOfChannels; i++) {
		pChannel = channels[i];
		pChannel->setPaused(true);
	}
}

int AudioManager::Play(string path)
{
	int channelID = mnNextChannelId++;
	SoundMap::iterator sound = sounds.find(path);// Search for a matching sound in the map
	if (sound == sounds.end())
		return -1;// Ignore call if no sound was found

	FMOD::Channel* pChannel = nullptr;
	system->playSound(	//  play the sound
		sound->second,// 
		nullptr,//FMOD_CHANNEL_FREE select an intrenal channel
		false, //starts in pause? false? then play right now
		&pChannel//  channel reference for further manipulation. //advanced function must return the reference channel
	);
	if (pChannel) {
		channels[channelID] = pChannel;
	}

	if (isAllMute)
		Mute(channelID);

	return channelID;
}

int AudioManager::Play(string path, int loops)//-1 infinite
{
	int channelID = mnNextChannelId++;
	SoundMap::iterator sound = sounds.find(path);// Search for a matching sound in the map
	if (sound == sounds.end())
		return -1;// Ignore call if no sound was found

	FMOD::Channel* pChannel = nullptr;
	system->playSound(	//  play the sound
		sound->second,// 
		nullptr,//FMOD_CHANNEL_FREE select an intrenal channel
		false, //starts in pause? false? then play right now
		&pChannel//  channel reference for further manipulation. //advanced function must return the reference channel
	);
	if (pChannel) {
		channels[channelID] = pChannel;
	}
	channels[channelID]->setLoopCount(loops);

	if (isBackgroundMute || isAllMute)
		Mute(channelID);

	return channelID;
}

void AudioManager::PlaySFX(const std::string& path, float minVolume, float maxVolume, float minPitch, float maxPitch)
{
	SoundMap::iterator sound = sounds.find(path);
	if (sound == sounds.end()) return;// if there is not sound, do nothing
	// Calculate random volume and pitch in selected range
	float volume = RandomBetween(minVolume, maxVolume);
	float pitch = RandomBetween(minPitch, maxPitch);
	// Play the sound effect with these initial values
	FMOD::Channel* channel;
	system->playSound(
		sound->second,
		nullptr,
		true,
		&channel
	);
	channel->setVolume(volume);
	float frequency;
	channel->getFrequency(&frequency);
	channel->setFrequency(ChangeSemitone(frequency, pitch));
	channel->setPaused(false);

	if (isAllMute)
		channel->setVolume(0.0f);
}

void AudioManager::Resume(int channelID)
{
	FMOD::Channel* pChannel = nullptr;
	pChannel = channels[channelID];
	channels[channelID]->setPaused(false);
}

void AudioManager::SetPan(int channelID, float fPan)
{
	FMOD::Channel* pChannel = nullptr;
	pChannel = channels[channelID];
	channels[channelID]->setPan(fPan);
}

void AudioManager::Stop(int channelID)
{
	FMOD::Channel* pChannel = nullptr;
	pChannel = channels[channelID];
	channels[channelID]->stop();
}

void AudioManager::StopAll()
{
	for (int i = 0; i < numberOfChannels; i++) {
		FMOD::Channel* pChannel = nullptr;
		FMOD_RESULT res = system->getChannel(i, &pChannel);
		if (res == FMOD_OK && pChannel) {
			pChannel->stop();
		}
	}
}

void AudioManager::Stream(string path)
{
	LoadOrStream(path, true);
}

void AudioManager::ToggleAll() 
{
	//Toggle all sounds in channels
	for (int i = 0; i < numberOfChannels; i++) {
		FMOD::Channel* pChannel = nullptr;
		FMOD_RESULT res = system->getChannel(i, &pChannel);
		if (res == FMOD_OK && pChannel) {
			TogglePause(i);
		}
	}
}

void AudioManager::ToggleAllPause(int channelID) {
	pauseEnabled = !pauseEnabled;
	//Toggle all sounds in channels
	for (int i = 0; i < numberOfChannels; i++) {
		FMOD::Channel* pChannel = nullptr;
		FMOD_RESULT res = system->getChannel(i, &pChannel);
		if (res == FMOD_OK && pChannel) {
			if (pauseEnabled)
				Pause(i);
			else
				Resume(i);
		}
	}
	if (!pauseEnabled)
		Pause(channelID);
	else
		Resume(channelID);
}

void AudioManager::ToggleMuteAll()
{
	if (!isAllMute) {
		for (auto channel : channels) {
			Mute(channel.first);
		}
	}
	else {
		for (auto channel : channels) {
			channels[channel.first]->setVolume(1);
		}
	}

	isAllMute = !isAllMute;
}

void AudioManager::ToggleMuteBG()
{
	if (!isBackgroundMute) {
		for (int channelID : mLoadLoopChannels) {
			Mute(channelID);
		}
	}
	else {
		for (int channelID : mLoadLoopChannels) {
			channels[channelID]->setVolume(1);
		}
	}

	isBackgroundMute = !isBackgroundMute;
}

void AudioManager::TogglePause(int channelID)
{
	bool paused;
	FMOD::Channel* pChannel = nullptr;
	pChannel = channels[channelID];
	channels[channelID]->getPaused(&paused);
	channels[channelID]->setPaused(!paused);
}

void AudioManager::Unload(string path)
{
	SoundMap::iterator sound = sounds.find(path);
	if (sound == sounds.end()) return;

	sounds.erase(sound);
}

void  AudioManager::Volume(int channelID, float value)
{
	FMOD::Channel* pChannel = nullptr;
	pChannel = channels[channelID];
	channels[channelID]->setVolume(value);
}


float RandomBetween(float min, float max) 
{
	if (min == max) return min;
	float n = (float)rand() / (float)RAND_MAX;
	return min + n * (max - min);
}

float ChangeSemitone(float frequency, float variation) 
{
	static float semitone_ratio = pow(2.0f, 1.0f / 12.0f);
	return frequency * pow(semitone_ratio, variation);
}
