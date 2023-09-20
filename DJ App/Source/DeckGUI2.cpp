/*
  ==============================================================================

    DeckGUI2.cpp
    Created: 6 Mar 2023 8:52:04pm
    Author:  Jake

  ==============================================================================
*/
#include <JuceHeader.h>
#include "DeckGUI2.h"
#include <strings.h>

//==============================================================================
DeckGUI2::DeckGUI2(DJAudioPlayer& dj, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse)  :
djAudioPlayer(dj), waveformDisplay(formatManagerToUse, cacheToUse) //calls the constructor on waveformdisplay
{
    
    
        playButton.setLookAndFeel(&myLookAndFeel);
        playButton.addListener(this);
        playButton.setButtonText("Start/Stop");
        
        loadButton.setLookAndFeel(&myLookAndFeel);
        addAndMakeVisible(loadButton); //This is the only button made visible until a file is loaded
        loadButton.addListener(this);
        loadButton.setButtonText("Load Audio File");
        
        skipForward.setLookAndFeel(&myLookAndFeel);
        skipForward.addListener(this);
        skipForward.setButtonText(">>");
        
        skipBackward.setLookAndFeel(&myLookAndFeel);
        skipBackward.addListener(this);
        skipBackward.setButtonText("<<");
    
        speedReset.setLookAndFeel(&myLookAndFeel);
        speedReset.addListener(this);
        speedReset.setButtonText("reset");
    
    
        
    
        String trackPos = djAudioPlayer.getTrackPosInMinsSecondsMs();
    
        posLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
        posLabel.setText(trackPos, juce::dontSendNotification);
        posLabel.attachToComponent(&posSlider, false); //attaches label to another object(this attaches to fader)
        posLabel.setJustificationType(juce::Justification::centredTop);//Center of object

        
       
        //Track position slider
        posSlider.setLookAndFeel(&myLookAndFeel);
        posSlider.addListener(this);
        posSlider.setRange(0,1);
        posSlider.setValue(0);
        posSlider.setNumDecimalPlacesToDisplay(3);
        posSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        posSlider.setSize(getWidth() * 0.4, getHeight() * 0.1);
        posSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 40, 40); //No text box
        
    
        //playback slider
        
        playbackLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
        playbackLabel.setText("Speed", juce::dontSendNotification);
        playbackLabel.attachToComponent(&speedSlider, false); //attaches label to another object(this attaches to fader)
        playbackLabel.setJustificationType(juce::Justification::centredTop);//Center of object
    
        speedSlider.setLookAndFeel(&myLookAndFeel);
        speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        speedSlider.addListener(this);
        speedSlider.setRange(0.1, 3.f);
        speedSlider.setValue(1.f);
        speedSlider.setNumDecimalPlacesToDisplay(2);
        speedSlider.setSize(getWidth()*0.2,getHeight()/20);
        speedSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 30, 20);
    
        speedLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
        speedLabel.setText("Playback Speed", juce::dontSendNotification);
        speedLabel.attachToComponent(&speedSlider, true);
        

    
        //volume slider
        volumeLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
        volumeLabel.setText("Volume", juce::dontSendNotification);
        volumeLabel.attachToComponent(&volumeSlider, false); //attaches label to another object(this attaches to fader)
        volumeLabel.setJustificationType(juce::Justification::centredTop);//Center of object
            
        volumeSlider.setLookAndFeel(&myLookAndFeel);
        volumeSlider.addListener(this);
        volumeSlider.setRange(0, 1);
        volumeSlider.setNumDecimalPlacesToDisplay(2);
        volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical); //horizontal across
        volumeSlider.setValue(5);
        volumeSlider.setSize(getWidth()*0.2,getHeight()/20);
        volumeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 30, 20);
    
        cutoffSlider.setRange(0,1);
        cutoffSlider.setValue(1);
    cutoffSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    cutoffSlider.setSize(getWidth()*0.2,getHeight()/20);
    cutoffSlider.setLookAndFeel(&myLookAndFeel);
        
    cutoffSlider.onValueChange = [this]()
    {
        float v = cutoffSlider.getValue();
        djAudioPlayer.setCutoff(v * v * v * 0.5);
    };
      
    
    waveformDisplay.addMouseListener(this, false); //deckGui is a listner for the waveform
        
    
        //track percentage
        
        //runs timercallback function 24 times a second
        timerHz = 50;
        startTimerHz(timerHz);
        
    
        isPlaying = false;
        looping = false;
   
}

DeckGUI2::~DeckGUI2()
{
    playButton.setLookAndFeel(nullptr);
    loadButton.setLookAndFeel(nullptr);
    skipForward.setLookAndFeel(nullptr);
    skipBackward.setLookAndFeel(nullptr);
    speedReset.setLookAndFeel(nullptr);
    volumeSlider.setLookAndFeel(nullptr); //Important
    posSlider.setLookAndFeel(nullptr);
    speedSlider.setLookAndFeel(nullptr);
    cutoffSlider.setLookAndFeel(nullptr);
    stopTimer();
}

void DeckGUI2::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setFont(20.f);
    
    
    juce::Rectangle<int> background = getLocalBounds();
    
    auto gradient = juce::ColourGradient::vertical(juce::Colour::fromFloatRGBA(0.28f, 0.3f, 0.34f, 1.0f),0.0f, juce::Colour::fromFloatRGBA(0.4f,0.3f,0.6f,1.0f), getHeight());
    //juce::ColourGradientgradient(juce::Colours::blue, 0.0f, 0.0f, juce::Colours::black, getWidth(), getHeight(), false);
    
    gradient.addColour(0.5, juce::Colour::fromFloatRGBA(0.34f, 0.3f, 0.5f, 1.0f)); //this is how to add a colour stop
    
    g.setGradientFill(gradient);
    
    g.fillRect(background);

}

void DeckGUI2::paintOverChildren(juce::Graphics &g)
{
    if(guiActive) // a previous booleon I have instated instead of creating a new member function
    {
        auto loopRegion = djAudioPlayer.loopRegion;
        if(loopRegion.proper())
        {
            
            sine1.setFrequency(900 * speedSlider.getValue());
            float loopOpacity = sine1.getNextSample();
            float maxOpacity = 0.4;
            if(loopOpacity > maxOpacity)
            {
                loopOpacity = maxOpacity;
            }
            
            
            g.setColour(Colours::cyan);
            g.setOpacity(loopOpacity + 0.2);
            auto start = loopRegion.start();
            auto length = loopRegion.length();
            auto b = waveformDisplay.getBoundsInParent();
            b.setBounds(start * b.getWidth() + b.getX(), b.getY(),length * b.getWidth(), b.getHeight());
            g.fillRect(b);
            
        }
    }
}

void DeckGUI2::timerCallback() //runs 24 times a second, dynamic GUI
{
    bool fileLoaded = djAudioPlayer.isURLFilled();

    if(fileLoaded == true && guiActive == false) //only runs if gui hasn't been built
    {
        guiActive = true;
        
        buildGUI();
        
    }
    waveformDisplay.setPositionRelative(djAudioPlayer.getPositionRelative());
    
    posSlider.setValue(djAudioPlayer.getPositionRelative());
    
    posLabel.setText(djAudioPlayer.getTrackPosInMinsSecondsMs(), juce::dontSendNotification);
    
    
}
    

void DeckGUI2::resized()
{
    auto rowH = getHeight() / 8;
    auto columnW = getWidth() / 8;
    
    volumeSlider.setBounds(columnW * 0.3, getHeight() / 3, columnW, rowH * 3.8);
    speedSlider.setBounds(columnW * 5, getHeight() / 12, columnW, rowH * 3.8);
//    cutoffSlider.setBounds(columnW * 4.5, getHeight() / 12, columnW, rowH * 3.8);
    speedReset.setBounds(columnW * 6 + getWidth() * 0.05, getHeight() / 12 * 3.4, columnW, rowH * 0.3);
    posSlider.setBounds(columnW * 2, (getHeight() / 3 )* 2, getWidth() - columnW * 2.5, rowH);
    waveformDisplay.setBounds(columnW * 2, getHeight() * 0.35, getWidth() - columnW * 2.5, rowH * 2);
    
    playButton.setBounds(0,0,getWidth(),rowH);
    skipBackward.setBounds(0,rowH ,getWidth() / 2, rowH);
    skipForward.setBounds(getWidth() / 2,rowH,getWidth() / 2, rowH);
    
    if(guiActive == true) //used to change the position and text of the load button
    {
        rowH = getHeight() / 6; //To match GUI 1
        loadButton.setButtonText("Load other Audio File");
        loadButton.setBounds(0, 5 * rowH, getWidth(), rowH); //matching the alternative gui's load button
    }else
    {
        loadButton.setBounds(getWidth() / 4,getHeight() / 4, getWidth() / 2, getHeight() / 2); //Starting load position
    }

}

void DeckGUI2::buttonClicked(Button * button)
{
    
if(&speedReset == button)
    {
        djAudioPlayer.setSpeed(1.0);
        speedSlider.setValue(1.0f);
        std::cout<<"speedreset"<<std::endl;
    }
    
if(&playButton == button && isPlaying == false)
{
    isPlaying = true; //start/stop logic
    DBG("MainComponent::buttonClicked: playButton");
    djAudioPlayer.play();
}
else if(&playButton == button && isPlaying == true)
{
    djAudioPlayer.stop();
    isPlaying = false;
}
else if (&loadButton == button)
{
    FileChooser chooser{"Select a file..."};
    if(chooser.browseForFileToOpen())
    {
        auto url = URL(chooser.getResult()); //chooses result
        djAudioPlayer.loadURL(url); //calls the function with the result of the chooser as an argument
        waveformDisplay.loadURL(url); //loads this url into the waveform function
        std::cout<<"load completed"<<std::endl;
    }
}

else if(&skipBackward == button)
{
    posSlider.setValue(posSlider.getValue() - 0.25);
   
}

else if(&skipForward == button)
{
    posSlider.setValue(posSlider.getValue() + 0.25);
}

    
}

void DeckGUI2::sliderValueChanged(Slider * slider)
{
    if(&volumeSlider == slider)
    {
        
        djAudioPlayer.setGain(slider -> getValue());
    }
    else if(&posSlider == slider)
    {
        float posInSecs = posSlider.getValue();
        djAudioPlayer.setPositionRelative(posInSecs);
        
    }
    else if(&speedSlider == slider)
    {
        djAudioPlayer.setSpeed(slider -> getValue());
        std::cout<<slider -> getValue()<<std::endl;
    }
}

void DeckGUI2::mouseDown(const juce::MouseEvent &event)
{
    if(guiActive && &waveformDisplay == event.eventComponent)
    {
        mouseDrag(event);
    }
}

void DeckGUI2::mouseDrag(const juce::MouseEvent &event)
{
        if(guiActive && &waveformDisplay == event.eventComponent)
        {
            float invw = 1./waveformDisplay.getWidth(); //noramlising bounds
            auto start = event.getMouseDownX() * invw;
            auto end = event.getPosition().getX() * invw;
            djAudioPlayer.loopRegion.start(start).end(end);
            waveformDisplay.repaint();
        }
}

void DeckGUI2::buildGUI() //After an audio file is loaded(gives the user clearer instructions
{
   
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(volumeLabel);
    
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(playbackLabel);
    
    addAndMakeVisible(speedReset);
    
    addAndMakeVisible(posSlider);
    
    addAndMakeVisible(playButton);
    addAndMakeVisible(skipBackward);
    addAndMakeVisible(skipForward);
    
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(cutoffSlider);
    
    
    resized(); //Sets bounds
    
    std::cout << "Gui built" << std::endl;
}

bool DeckGUI2::isInterestedInFileDrag(const StringArray& files)
{
DBG("DeckGUI2::isInterestedInFileDrag"); return true;
}

void DeckGUI2::filesDropped(const StringArray& files, int x, int y)
{
    for(const auto& filename : files)
{ DBG("DeckGUI::filesDropped " << filename);
        auto url = URL(File(filename));
        djAudioPlayer.loadURL(url);
        waveformDisplay.loadURL(url);
        

    }
    return;
}


