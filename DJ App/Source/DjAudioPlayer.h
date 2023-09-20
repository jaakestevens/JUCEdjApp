/*
  ==============================================================================

    DjAudioPlayer.h
    Created: 27 Feb 2023 6:30:32pm
    Author:  Jake

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Interval.h"
#include <atomic>
#include "onePole.h"

using namespace juce;
using namespace std;

class DJAudioPlayer : public juce::AudioSource{

public:
    DJAudioPlayer(AudioFormatManager& afm);
        
    ~DJAudioPlayer();
    
    virtual void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    virtual void releaseResources()override;

    virtual void getNextAudioBlock (const AudioSourceChannelInfo &bufferToFill) override;
        
    bool loadURL(const juce::URL& url);
    
    void play();

    void stop();
    
    void setGain(double gain);
    
    void setPosition(const double posInSecs);
    
    void setPositionRelative(const double pos);
    
    
    float getPosInTrack();
    
    float getTrackLength();
    
    String getTrackPosInMinsSecondsMs();
    
    void setMix(float v);
    
    void setGain(float v);
    
    void setCutoff(float f);
    
    
    bool isURLFilled();
    
    void setSpeed(const double speed);
    
    double getPositionRelative();
    
    double getLevel();
    
    Interval<float> loopRegion;
    
    OnePole<float> filter1, filter2;
    
    
    
    
    
private:
    
    
    AudioFormatManager& formatManager; //does not store its own copy
    
    std::unique_ptr <AudioFormatReaderSource > readerSource; // this is a pointer as we don’t want to construct it until we have a file to play
    
    AudioTransportSource transportSource; // this has been swapped with readersource as otherwise readersource will be a dangling pointer
    
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
    AudioSourceChannelInfo channelInfo;
    
    float gain = 1.f;
    float mix = 1.f;
    bool filledURL;
    bool reverse;
    bool isPlaying;
    
};
