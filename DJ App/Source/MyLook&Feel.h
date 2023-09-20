/*
  ==============================================================================

    MyLook&Feel.h
    Created: 22 Mar 2023 4:27:24pm
    Author:  Jake

  ==============================================================================
*/

#include "Sine.h"

#pragma once
class MyLookAndFeel : public juce::LookAndFeel_V4
{

    void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        
        sliderGlow.setFrequency(900);
        float sliderOpacity = juce::jmap(sliderGlow.getNextSample(),-2.f, 2.f, 0.25f, 1.0f);


        
        if(slider.isVertical())
        {
            
            auto trackWidth = fmin (6.0f,(float) width * 0.25f);
            
            juce::Point<float> startPoint ((float) x + (float) width * 0.5f, (float) (height + y));
            
            juce::Point<float> endPoint (startPoint.x,(float) y);
            
            
            
            juce::Path backgroundTrack;
            backgroundTrack.startNewSubPath (startPoint);
            backgroundTrack.lineTo(endPoint);
            g.setColour (juce::Colours::black); //background colour
            g.strokePath (backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
            
            
            
            
            juce::Path valueTrack;
            juce::Point<float> minPoint, maxPoint;
            
            auto kx = ((float) x + (float) width * 0.5f);
            
            auto ky = sliderPos;
            
            minPoint = startPoint;
            maxPoint = { kx, ky };
            
            auto thumbWidth = getSliderThumbRadius (slider);
            
            valueTrack.startNewSubPath (minPoint);
            valueTrack.lineTo(maxPoint);
            g.setColour (juce::Colours::violet); //background
            g.setOpacity(sliderOpacity);
            g.strokePath (valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
            
            
            auto pointerColour = juce::Colours::grey; //pointer
            
            float lowerSlider = maxSliderPos * 0.8;
            float scaledValue = 1.0f;
            
            if(sliderPos >= lowerSlider)
            {
                scaledValue = juce::jmap(sliderPos, lowerSlider, maxSliderPos, 1.f, 0.f);
            }
            g.setColour(pointerColour);
            g.fillEllipse (juce::Rectangle<float> (static_cast<float> (thumbWidth * 5 * scaledValue), static_cast<float> (thumbWidth * 0.6)).withCentre (maxPoint));
            
        }
        
        if(slider.isHorizontal())
        {
                auto trackWidth = fmin(6.0f, (float) height * 0.25f);

                juce::Point<float> startPoint ((float) x,(float) y + (float) height * 0.5f);

                juce::Point<float> endPoint ((float) (width + x),startPoint.y);

                juce::Path backgroundTrack;
            
                backgroundTrack.startNewSubPath (startPoint);
                backgroundTrack.lineTo (endPoint);
                g.setColour(juce::Colours::black);
                g.strokePath (backgroundTrack, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});

                juce::Path valueTrack;
                juce::Point<float> minPoint, maxPoint;
            
                auto kx = sliderPos;
                auto ky = ((float) y + (float) height * 0.5f);

                minPoint = startPoint;
                maxPoint = { kx, ky };

                auto thumbWidth = getSliderThumbRadius(slider);
                

                valueTrack.startNewSubPath (minPoint);
                valueTrack.lineTo(maxPoint);
                g.setColour(juce::Colours::violet);
                g.setOpacity(sliderOpacity);
                g.strokePath (valueTrack,{trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
            

                auto pointerColour = (juce::Colours::grey);
            
                float scaledValue = 1.0f;
            
                float newStartPoint = x + width * 0.5;
            
            
                if(slider.getValue() <= slider.getMaximum() * 0.2)
                {
                
                    scaledValue = juce::jmap(sliderPos,newStartPoint,(float)x, 1.f, 0.f);
                    scaledValue = scaledValue * 2.5;
                }

                g.setColour(pointerColour);
                g.fillEllipse (juce::Rectangle<float>(static_cast<float> (thumbWidth), static_cast<float> (thumbWidth * 5 * scaledValue)).withCentre(maxPoint));


                }
    };
        
    
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {
        auto radius = (float) juce::jmin(width / 2, height / 2) - 4.0f; //jmin returns the smaller of 2 values
        auto centreX = (float) x + (float) width * 0.5f; //brackets are for typecasting
        auto centreY = (float) y + (float) height * 0.5f;
        auto rx = centreX - radius; //left
        auto ry = centreY - radius; //top
        auto rw = radius * 2.0f; //width
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle); //angle at which dial will point
        
        g.setColour(juce::Colours::white);
        g.fillEllipse(rx, ry, rw, rw);
        
        g.setColour(juce::Colours::lightblue);
        g.drawEllipse(rx, ry, rw, rw, 1.0f);
        
        juce::Path p;
        
        auto pointerLength = radius * 0.7f; //pointer as in point on the dial
        auto pointerThickness = 3.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        
        g.setColour(juce::Colours::black);
        g.fillPath(p);
    }
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds();
        auto edge = 4;
        
        buttonArea.removeFromLeft(edge);
        buttonArea.removeFromTop(edge);
        
        //shadow
        g.setColour(juce::Colours::darkgrey.withAlpha(0.6f));
        g.fillRect(buttonArea);
        auto offset = isButtonDown ? -edge / 2 : -edge; //removes more if button is dowm
        buttonArea.translate(offset, offset);
        
        g.setColour(juce::Colours::slategrey);
        if(isButtonDown)
        {
            g.setColour(juce::Colours::yellow);
            g.setOpacity(1.f);
        }

        g.fillRect(buttonArea);
        
    }
    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown) override
    {
        auto font = getTextButtonFont(button, button.getHeight());
        g.setFont(font);

        g.setColour(juce::Colours::black);
        
        auto yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
        auto cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;
        
        auto fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
        auto leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
        auto rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
        auto textWidth = button.getWidth() - leftIndent - rightIndent;
        
        auto edge = 4;
        auto offset = isButtonDown ? edge / 2 : 0;
        
        if(textWidth > 0)
        {
            g.drawFittedText(button.getButtonText(), leftIndent + offset, yIndent + offset, textWidth, button.getHeight() - yIndent * 2,juce::Justification::centred, 2);
            
            g.setColour(juce::Colours::darkgrey.withAlpha(0.2f));
            
            g.drawFittedText(button.getButtonText(), leftIndent + offset * 3 + 4, yIndent + offset * 3 + 4, textWidth, button.getHeight() - yIndent * 2,juce::Justification::centred, 2);
            
        }
    }
    
private:
    
    Sine sliderGlow;
    
};
