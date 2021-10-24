/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ColemanJP02EQAudioProcessorEditor  : public juce::AudioProcessorEditor,
public juce::Slider::Listener, public juce::Button::Listener
{
public:
    ColemanJP02EQAudioProcessorEditor (ColemanJP02EQAudioProcessor&);
    ~ColemanJP02EQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonStateChanged(juce::Button* button) override;
    void buttonClicked(juce::Button* button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ColemanJP02EQAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColemanJP02EQAudioProcessorEditor)
    
    juce::Slider lowShelfFcSlider;
    juce::Slider lowShelfGainSlider;
    
    juce::Slider lowMidFcSlider;
    juce::Slider lowMidGainSlider;
    juce::Slider lowMidQSlider;
    
    juce::Slider highMidFcSlider;
    juce::Slider highMidGainSlider;
    juce::Slider highMidQSlider;
    
    juce::Slider highShelfFcSlider;
    juce::Slider highShelfGainSlider;
    
    juce::Slider highPassFcSlider;
    juce::ToggleButton highPassActiveToggle { "High pass active" };
    
    juce::Label lowShelfLabel;
    juce::Label lowMidLabel;
    juce::Label highMidLabel;
    juce::Label highShelfLabel;
    
    juce::Label highPassLabel;
    
    juce::Label frequencyLabel;
    juce::Label gainLabel;
    juce::Label qLabel;
    
    enum parameterMap {
        lowShelfFc,
        lowShelfGain,
        lowMidFc,
        lowMidGain,
        lowMidQ,
        highMidFc,
        highMidGain,
        highMidQ,
        highShelfFc,
        highShelfGain,
        highPassFc,
        highPassActive
    };
    
    void makeRotarySlider(int x, int y, juce::Slider& slider, parameterMap paramNum, std::string units, float rangeInterval, bool freqslider);
    void setParamFromSlider(juce::Slider& slider, parameterMap paramNum);
    void makeFilterTitle(int x, int y, juce::Label& label, std::string name);
    void makeParamLabel(int y, juce::Label& label, std::string name);
};
