#pragma once

#include <JuceHeader.h> //searches first in the system paths
#include "DjAudioPlayer.h" //quotation include looks for local files (in the same file as the source)
#include "DeckGUI2.h"

using namespace juce;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void sliderValueChanged(Slider * slider);
    
    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{20}; //we need to tell it how large the cache is
    
    bool isPlaying;
    bool looping;
        
    DJAudioPlayer player1{formatManager};
    DJAudioPlayer player2{formatManager};
    DeckGUI2 deck1{player1, formatManager, thumbCache}; //feeding objects into the gui constructor
    DeckGUI2 deck2{player2, formatManager, thumbCache}; //My own GUI
    
    juce::Slider xfadeSlider;
    
    MixerAudioSource mixerSource;

    MyLookAndFeel myLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    };

