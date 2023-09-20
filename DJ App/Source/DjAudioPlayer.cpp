/*
  ==============================================================================

    DjAudioPlayer.cpp
    Created: 27 Feb 2023 6:30:32pm
    Author:  Jake

  ==============================================================================
*/

#include "DjAudioPlayer.h"
#include <JuceHeader.h>

using namespace std;
using namespace juce;

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& afm) : formatManager(afm)
{
//    formatManager.registerBasicFormats();
    filledURL = false;
}
    
DJAudioPlayer::~DJAudioPlayer()
{
    
}
        

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
}
    
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo &bufferToFill)
{
    auto l = loopRegion;
    
    if(l.proper())
    {
        auto p = getPositionRelative();
            if(p < l.start())
            {
                setPositionRelative(l.start());
            }
            else if(p >= l.end())
            {
                setPositionRelative(l.start());
            }
    }
    
    if(!readerSource.get())
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    resampleSource.getNextAudioBlock(bufferToFill);
    
    int numChannels = bufferToFill.buffer->getNumChannels();
    auto * chan0 = numChannels>0 ? bufferToFill.buffer->getWritePointer(0,bufferToFill.startSample) : nullptr;
    auto * chan1 = numChannels>1 ? bufferToFill.buffer->getWritePointer(1,bufferToFill.startSample) : nullptr;
    
    for(int i = 0; i < bufferToFill.numSamples; i++)
    {
        if(chan0) chan0[i] = filter1(chan0[i]);
        if(chan0) chan0[i] = filter1(chan0[i]);
        
    }
}

bool DJAudioPlayer::loadURL(const URL& url) //URL is the path to the sound file, returns whether the load was successfully
{
    
    auto * reader = formatManager.createReaderFor(url.getLocalFile());
    
    if(reader)
        {
        
        std::unique_ptr <AudioFormatReaderSource > newSource(new AudioFormatReaderSource(reader , true));
            
        transportSource.setSource(newSource.get(), 0, nullptr , reader ->sampleRate);
        readerSource.reset(newSource.release());
            filter1.reset();
            filter2.reset();
            filledURL = true;
            return true;

        }
    return false;
}

void DJAudioPlayer::play()
{
    transportSource.start(); //then starts the track
    
    std::cout << "playing" << std::endl;
    
    isPlaying = true;
    
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
    isPlaying = false;
}

void DJAudioPlayer::setGain(double gain)
{
    transportSource.setGain(gain);
}

float DJAudioPlayer::getPosInTrack()
{
    float trackPercentage = (transportSource.getCurrentPosition() / transportSource.getLengthInSeconds());
    return trackPercentage;
}

float DJAudioPlayer::getTrackLength()
{
    return transportSource.getLengthInSeconds();
}

String DJAudioPlayer::getTrackPosInMinsSecondsMs()
{
    int minutes = (int)transportSource.getCurrentPosition() / 60;
    int seconds = (int)transportSource.getCurrentPosition() % 60;
    int ms = (int)(transportSource.getCurrentPosition() * 1000) % 1000;
    
    String timeString = (String)minutes + "m" + "   " + (String)seconds + "s" + "  " + (String)ms + "ms";
    
    
    return timeString; //converted to a string outside the function

}



bool DJAudioPlayer::isURLFilled()
{
    return filledURL;
}



void DJAudioPlayer::setPositionRelative(const double pos)
{
    auto posInSecs = pos * transportSource.getLengthInSeconds(); //scales the legnth of the track between 0 and 1
    
    setPosition(posInSecs);
}

void DJAudioPlayer::setPosition(const double posInSecs)
{
    if(posInSecs < 0. || posInSecs > transportSource.getLengthInSeconds())
    {
        DBG("DjAudioPlayer::setPosition: warning: set position " << posInSecs << "greater than length" << transportSource.getLengthInSeconds());
        return;
    }
    transportSource.setPosition(posInSecs);
    
    if(isPlaying == true)
    {
        transportSource.start();
    }
    //transportSource.start();
};

void DJAudioPlayer::setSpeed(const double speed)
{
    if(speed <= 0.)
    {
        std::cout<<"speed must be greater than 0"<<std::endl;
    }else
    {
        resampleSource.setResamplingRatio(speed);
    }
}
double DJAudioPlayer::getPositionRelative()
{
    auto length = transportSource.getLengthInSeconds();
    if(length > 0.) return transportSource.getCurrentPosition() / length;
    return 0.;
    
}


void DJAudioPlayer::setGain(float v)
{
    gain = v;
    transportSource.setGain(mix * gain);
    
}
void DJAudioPlayer::setMix(float v)
{
    mix = v;
    transportSource.setGain(mix * gain);
    
}

void DJAudioPlayer::setCutoff(float f)
{
    filter1.cutoff(f);
    filter2.cutoff(f);
}


