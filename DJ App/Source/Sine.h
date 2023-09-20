/*
  ==============================================================================

    Sine.h
    Created: 17 Apr 2023 8:46:12pm
    Author:  Jake

  ==============================================================================
*/

#pragma once
class Sine
{
public:
    Sine() : phase(0.f), frequency(880.f), sampleRate(44100){}
    
    void setFrequency(float f)
    {
        frequency = f;
    }
    
    float getNextSample()
    {
        float value = std::sin(phase);
        phase += (frequency / sampleRate) * 2.0f * juce::MathConstants<float>::pi;
        if (phase >= 2.0f * juce::MathConstants<float>::pi)
        {
            phase -= 2.0f * juce::MathConstants<float>::pi;
        }
               if(value > 0)
               {
                   return value;
               }else{
                   return 0;
               }
    }
    
private:
    float phase;
    float frequency;
    float sampleRate;
};
