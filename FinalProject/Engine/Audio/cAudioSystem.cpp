#include "cAudioSystem.h"

#include <Engine/Logging/Logging.h>

#include <vector>

//std::string filename;

// Static Data
//============

// Interface
//==========

void eae6320::AudioSystem::cAudio::AudioConstructor(std::string i_filePath, std::string i_audioName, size_t i_volume, bool i_isLoop)
{
	if (i_filePath != "" && i_audioName != "" && i_volume)
	{
		filePath = i_filePath;
		audioName = i_audioName;
		volume = i_volume;
		isLoop = i_isLoop;
		inIndex = 0;
	}
	else
	{
		Logging::OutputMessage("Audio path or name is illegal!");
	}
}

void eae6320::AudioSystem::cAudio::SubmitAudioToBePlayed()
{
	MCIERROR mciError;

	std::string mciCommendString = "open " + filePath + " type mpegvideo alias " + audioName;
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();;
	mciError = mciSendString(mciCommend, NULL, 0, 0);

	WCHAR errorMessage[128];
	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		Logging::OutputMessage("Submit audio failed : %s:", errorMessage);
	}
}

void eae6320::AudioSystem::cAudio::Play()
{
	MCIERROR mciError;
	WCHAR errorMessage[128];
	std::string mciCommendString = "play " + audioName;
	LPCWSTR mciCommend;
	if (isLoop)
	{
		mciCommendString += " repeat";
		std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
		mciCommend = temp.c_str();
		mciError = mciSendString(mciCommend, NULL, 0, 0);
	}
	else
	{
		std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
		mciCommend = temp.c_str();
		mciError = mciSendString(mciCommend, NULL, 0, 0);
	}

	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		if (errorMessage)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			Logging::OutputMessage("Play audio failed : %s:", errorMessage);
		}
	}
}

void eae6320::AudioSystem::cAudio::PlayIndependent()
{
	// Check independent list
	// If there is no audio being played, turn off the audio and free the device for others to use
	std::string t_audioName;
	for (size_t i = inIndex; i > 0; i--)
	{
		t_audioName = audioName + "_" + std::to_string(i);
		if (IsPlaying_WithName(t_audioName))
		{
			break;
		}
		else
		{
			// Close this audio
			if (CloseAudio_WithName(t_audioName) == Results::Success)
				inIndex--;
		}
	}

	// Init a new device for new audio
	inIndex++;
	t_audioName = audioName + "_" + std::to_string(inIndex);

	// Open new device
	MCIERROR mciError;
	std::string mciCommendString = "open " + filePath + " type mpegvideo alias " + t_audioName;
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();
	mciError = mciSendString(mciCommend, NULL, 0, 0);

	WCHAR errorMessage[128];
	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		Logging::OutputMessage("Submit audio %s failed : %s:", t_audioName, errorMessage);
	}

	// Play new device
	mciCommendString = "play " + t_audioName;
	temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	mciCommend = temp.c_str();
	mciError = mciSendString(mciCommend, NULL, 0, 0);

	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		Logging::OutputMessage("Play audio %s failed : %s:", t_audioName, errorMessage);
	}

	SetVolume(volume);
}

void eae6320::AudioSystem::cAudio::PauseAudio()
{
	MCIERROR mciError;
	WCHAR errorMessage[128];
	std::string mciCommendString = "pause " + audioName;
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();

	mciError = mciSendString(mciCommend, NULL, 0, 0);

	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		if (errorMessage)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			Logging::OutputMessage("Pause audio failed : %s:", errorMessage);
		}
	}

	// Pause all independent audio
	std::string t_audioName;
	for (size_t i = inIndex; i > 0; i--)
	{
		t_audioName = audioName + "_" + std::to_string(i);
		mciCommendString = "pause " + t_audioName;
		temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
		mciCommend = temp.c_str();

		mciError = mciSendString(mciCommend, NULL, 0, 0);
		if (mciError)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			if (errorMessage)
			{
				mciGetErrorString(mciError, errorMessage, 128);
				Logging::OutputMessage("Pause audio %s failed : %s:", t_audioName, errorMessage);
			}
		}
	}
}

void eae6320::AudioSystem::cAudio::ResumeAudio()
{
	MCIERROR mciError;
	WCHAR errorMessage[128];
	std::string mciCommendString = "resume " + audioName;
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();

	mciError = mciSendString(mciCommend, NULL, 0, 0);

	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		if (errorMessage)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			Logging::OutputMessage("Resume audio failed : %s:", errorMessage);
		}
	}

	// Resume all independent audio
	std::string t_audioName;
	for (size_t i = inIndex; i > 0; i--)
	{
		t_audioName = audioName + "_" + std::to_string(i);
		mciCommendString = "resume " + t_audioName;
		temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
		mciCommend = temp.c_str();

		mciError = mciSendString(mciCommend, NULL, 0, 0);
		if (mciError)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			if (errorMessage)
			{
				mciGetErrorString(mciError, errorMessage, 128);
				Logging::OutputMessage("Resume audio %s failed : %s:", t_audioName, errorMessage);
			}
		}
	}
}

void eae6320::AudioSystem::cAudio::SetVolume(size_t i_volume)
{
	// Check the i_volume legality, 
	// if i_volume < 1, then i_volume = 1
	// if i_volume > 1000, then i_volume = 1000
	if (i_volume < 1 || i_volume > 1000)
	{
		volume = i_volume < 1 ? 1 : 1000;
	}
	else
	{
		volume = i_volume;
	}

	MCIERROR mciError;
	WCHAR errorMessage[128];
	std::string mciCommendString = "setaudio " + audioName + " volume to " + std::to_string(volume);
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();

	mciError = mciSendString(mciCommend, NULL, 0, 0);

	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		if (errorMessage)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			Logging::OutputMessage("Set audio volume failed : %s:", errorMessage);
		}
	}

	// Set all independent audio to the same volume
	if (inIndex > 0)
	{
		for (size_t i = inIndex; i > 0; i--)
		{
			std::string t_audioName = audioName + "_" + std::to_string(i);
			mciCommendString = "setaudio " + t_audioName + " volume to " + std::to_string(volume);
			temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
			mciCommend = temp.c_str();

			mciError = mciSendString(mciCommend, NULL, 0, 0);

			if (mciError)
			{
				mciGetErrorString(mciError, errorMessage, 128);
				if (errorMessage)
				{
					mciGetErrorString(mciError, errorMessage, 128);
					Logging::OutputMessage("Set audio %s volume failed : %s:", t_audioName, errorMessage);
				}
			}
		}
	}
}

bool eae6320::AudioSystem::cAudio::IsPlaying()
{
	return IsPlaying_WithName(audioName);
}

bool eae6320::AudioSystem::cAudio::IsPlaying_WithName(std::string i_audioName)
{
	bool retValue = false;

	MCIERROR mciError;
	WCHAR errorMessage[128];
	std::string mciCommendString = "status " + i_audioName + " mode";
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();
	WCHAR status[128];

	mciError = mciSendString(mciCommend, status, 128, 0);

	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		if (errorMessage)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			Logging::OutputMessage("Call IsPlaying() failed : %s:", errorMessage);
		}
		return retValue;
	}
	const char myChar[8] = { (char)status[0], (char)status[1], (char)status[2], (char)status[3], (char)status[4], (char)status[5], (char)status[6], (char)status[7] };

	retValue = strcmp(myChar, "playing") == 0;

	return retValue;
}

eae6320::cResult eae6320::AudioSystem::cAudio::CloseAudio()
{
	eae6320::cResult retValue = Results::Success;

	// Check independent audio, close them all
	for (size_t i = inIndex; i > 0; i--)
	{
		std::string t_audioName = audioName + "_" + std::to_string(i);
		retValue = CloseAudio_WithName(t_audioName);

		if (retValue == Results::Failure)
			return retValue;
	}

	// Close audio
	retValue = CloseAudio_WithName(audioName);

	return retValue;
}

eae6320::cResult eae6320::AudioSystem::cAudio::CloseAudio_WithName(std::string i_audioName)
{
	eae6320::cResult retValue = Results::Success;

	MCIERROR mciError;
	WCHAR errorMessage[128];
	std::string mciCommendString = "close " + i_audioName;
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();

	mciError = mciSendString(mciCommend, NULL, 64, 0);

	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		if (errorMessage)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			Logging::OutputMessage("Close audio %s failed : %s:", i_audioName, errorMessage);
		}

		retValue = Results::Failure;
	}

	return retValue;
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::AudioSystem::Initialize()
{
	auto result = Results::Success;

	return result;
}

eae6320::cResult eae6320::AudioSystem::CleanUp()
{
	auto result = Results::Success;

	return result;
}