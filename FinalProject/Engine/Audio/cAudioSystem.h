#pragma once
/*
    This file declares the external interface for the audio system
*/

#ifndef EAE6320_AUDIOSYSTEM_H
#define EAE6320_AUDIOSYSTEM_H

// Includes
//=========

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include<Engine/Results/cResult.h>

#include <iostream>
#include <string>

namespace eae6320
{
    namespace AudioSystem
    {
        // Interface
        //==========

        cResult Initialize();
        cResult CleanUp();


        // Audio Class
        //============
        class cAudio
        {
        private:
            // Audio file path
            std::string filePath = "";

            // Audio nick name
            std::string audioName = "";

            // Audio's index in s_audioDataReadyToPlay
            // The reason I do this is because one audio file maybe play at the same time but with a different audio volume
            // So the user needs to create a new cAudio instance
            // This new instance's filePath is the same but with different indexCount, refCount and volume
            //size_t indexCount = 0;

            // If the user want to use the same audio and same volume to play multiple times at the same time
            // Then the user does not need to create a new instance but just add 1 in this refCount
            //size_t refCount = 0;

            // Audio's volume
            size_t volume = 1000;

            // Is loop
            bool isLoop;

            // Independent play index
            size_t inIndex;

        public:
            // Interface
            //==========

            // This function needs to call at Initialize function at MyGame.
            // Create all the audio assets at once and then submit them at submit function.
            // Params:
            // i_filePath: Audio file path
            // i_audioName: Audio nickname
            // i_volume: Default audio volume
            // i_isLoop: The play state is looping or not
            void AudioConstructor(std::string i_filePath = "", std::string i_audioName = "", size_t i_volume = 1000, bool i_isLoop = false);

            // Before call play, has to call SubmitAudioToBePlayed first, This interface can not call in Initialize function but anywhere else.
            void SubmitAudioToBePlayed();

            // Play the audio: Need to call SubmitAudioToBePlayed() first.
            // param:
            // * isLoop: If this is true, will loop play this audio, if this is false, just play once.
            void Play();

            // If you want to play the same audio multiple times simultaneously.
            // You need to call this function instead of Play()
            // ***********************************IMPORTANT******************************************
            // Use this function to play audio. It will generate a new audio instance for the audio system
            // But it will use the same audio volume to play it.
            // And if you call PauseAudio() function, it will pause all instance.
            // ***********************************IMPORTANT******************************************
            void PlayIndependent();

            // Pause audio(include all independent audio)
            void PauseAudio();

            // Resume audio(include all independent audio)
            void ResumeAudio();

            // i_volume has to between (1 ~ 1000)
            void SetVolume(size_t i_volume);

            // Check current audio state, if is playing, return true
            bool IsPlaying();

            // Close audio(include all independent audio)
            eae6320::cResult CloseAudio();

        private:
            bool IsPlaying_WithName(std::string i_audioName);

            eae6320::cResult CloseAudio_WithName(std::string i_audioName);

        };
    }
}




#endif // !EAE6320_AUDIOSYSTEM_H