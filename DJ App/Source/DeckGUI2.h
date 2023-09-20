/*
  ==============================================================================

    DeckGUI2.h
    Created: 6 Mar 2023 8:52:14pm
    Author:  Jake

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DjAudioPlayer.h"
#include "WaveformDisplay.h"
#include "MyLook&Feel.h"
#include "Sine.h"
using namespace juce;

//==============================================================================
/*
*/
class DeckGUI2  : public Component, public Button::Listener, public Slider::Listener, public Timer, public FileDragAndDropTarget
{
public:
    DeckGUI2(DJAudioPlayer& dj, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse);
    ~DeckGUI2() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(Button * button) override;
    void sliderValueChanged(Slider * slider) override;
    void timerCallback() override;
    void loadTrackMeter();
    void buildGUI(); //dynamic GUI
    //drag and drop
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    void paintOverChildren(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

private:
    juce::TextButton playButton; //start/stop
    juce::TextButton loadButton;
    juce::TextButton skipForward;
    juce::TextButton skipBackward;
    juce::TextButton speedReset;
    
    juce::Slider volumeSlider;
    juce::Slider posSlider;
    juce::Slider speedSlider;
    juce::Slider cutoffSlider;
    
   
    
    juce::Label playbackLabel;
    juce::Label volumeLabel;
    juce::Label posLabel;
    juce::Label speedLabel;
    
    Sine sine1;
    
    
    
    bool looping;
    bool isPlaying;
    bool guiActive; //dynamic GUI
    int timerHz; //timer interval
    
    DJAudioPlayer& djAudioPlayer;
    WaveformDisplay waveformDisplay;
    MyLookAndFeel myLookAndFeel; //placed before to avoid the jassert error
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI2)
};

