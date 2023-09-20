/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 13 Mar 2023 3:56:06pm
    Author:  Jake

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

using namespace juce;

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse ,AudioThumbnailCache& cacheToUse)
: audioThumbnail(1000, formatManagerToUse , cacheToUse) //also calling the audioThumbnail constructor that takes samplesperThumbnail
                                                        //audioFormatManager that the thumbnail uses to read, and the cache stores multiple thumbnail
{
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    if(fileLoaded) //loading colours, only runs
    {
        fileLoadingAndDraw(g);
    }
    else {
        //no sample loaded(will not happen)
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
        g.setColour (juce::Colours::black);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
        g.setColour (juce::Colours::green);
        g.setFont (24.0f);
        g.drawText ("Waveform Here!", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
    }
    resized();
}

void WaveformDisplay::resized()
{
  

}
bool WaveformDisplay::loadURL(const URL& url)
{
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new URLInputSource(url)); //new means it is a dynamically allocated object
    return fileLoaded;
}
void WaveformDisplay::changeListenerCallback(ChangeBroadcaster * source)
{
    //DBG("wfd: change received!");
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if(pos >= 0. && pos != position) //only updates if it has changed
    {
        position = pos;
        repaint();
    }
}

void WaveformDisplay::fileLoadingAndDraw(Graphics& graphics)
{
    float percentageComplete = audioThumbnail.getProportionComplete();
    
    graphics.setColour(Colour::fromRGB(255 - (percentageComplete * 255), 0 + (percentageComplete * 255), 0)); //Loading colour that changes according to percentage of loading
    
    audioThumbnail.drawChannel(graphics , getLocalBounds(),0.,audioThumbnail.getTotalLength(),0,1.); //zoom factor
    
    
    
    
    if(audioThumbnail.isFullyLoaded()) //White when fully loaded
    {

        int w = 8; //width
        
        graphics.fillAll(juce::Colours::silver);
        
        graphics.setColour(Colour::fromRGB(255,255,255));
        
        
        audioThumbnail.drawChannel(graphics , getLocalBounds(),
                                   0., //start time
                                   audioThumbnail.getTotalLength(), // end time
                                   0, // channel number
                                   1.); //zoom factor
        
        
        
        
        graphics.setColour(Colours::royalblue);
        
        graphics.drawRect(position * getWidth() - w/2, 0, w, getHeight());
        
        
    }
    
}
