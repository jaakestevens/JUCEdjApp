#include "MainComponent.h"
//==============================================================================
MainComponent::MainComponent()
{
    
    // Make sure you set the size of the component after
    // you add any child components
    setSize(800, 600);
   
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    formatManager.registerBasicFormats();
    
    
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    
    xfadeSlider.onValueChange = [this]()
    {
        DBG("xfade: " << xfadeSlider.getValue());
        float v = xfadeSlider.getValue();
        float gl = std::min(1.f,2.f*(1.f-v));
        float gr = std::min(1.f,2.f*v);
        player1.setMix(gl);
        player2.setMix(gr);
    };
    
    xfadeSlider.setRange(0,1);
    xfadeSlider.setValue(0.5);
    xfadeSlider.setLookAndFeel(&myLookAndFeel);
    xfadeSlider.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(xfadeSlider);
    
    isPlaying = true;
    looping = false;
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    xfadeSlider.setLookAndFeel(nullptr); //no longer used when mainComponent is destroyed
    xfadeSlider.setColour(3, juce::Colours::royalblue);
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    mixerSource.addInputSource(&player1, false); //adding the players together
    mixerSource.addInputSource(&player2, false);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::royalblue);
    
}

void MainComponent::resized()
{
    
    int xfadeH = 80;
    
    deck1.setBounds(0, 0, getWidth()/2, getHeight() - xfadeH);
    deck2.setBounds(getWidth()/2, 0, getWidth()/2, getHeight() - xfadeH);
    xfadeSlider.setBounds(0, getHeight() - xfadeH, getWidth(), xfadeH);
    
}
void MainComponent::sliderValueChanged(juce::Slider * slider)
{
    
}
