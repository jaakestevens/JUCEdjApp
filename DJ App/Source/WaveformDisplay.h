/*
  ==============================================================================

    WaveformDisplay.h
    Created: 13 Mar 2023 3:56:06pm
    Author:  Jake

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component, public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse , AudioThumbnailCache& cacheToUse);

    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool loadURL(const URL& url);
    
    void changeListenerCallback(ChangeBroadcaster * source) override;
    
    void setPositionRelative(double pos);
    
    void fileLoadingAndDraw(juce::Graphics& graphics);
    
    double getGain();
    
   

private:
    bool fileLoaded = false;
    
    double position = 0.;
    
    bool waveformLoaded = false;
    
    AudioThumbnail audioThumbnail;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
