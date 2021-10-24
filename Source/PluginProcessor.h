/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "StkLite-4.6.1/BiQuad.h"

//==============================================================================
/**
*/
class ColemanJP02EQAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ColemanJP02EQAudioProcessor();
    ~ColemanJP02EQAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColemanJP02EQAudioProcessor)
    
    void calcAlgorithmParams();
    
    // User params
    juce::AudioParameterFloat* lowShelfFcParam;
    juce::AudioParameterFloat* lowShelfGainParam;
    
    juce::AudioParameterFloat* lowMidFcParam;
    juce::AudioParameterFloat* lowMidGainParam;
    juce::AudioParameterFloat* lowMidQParam;
    
    juce::AudioParameterFloat* highMidFcParam;
    juce::AudioParameterFloat* highMidGainParam;
    juce::AudioParameterFloat* highMidQParam;
    
    juce::AudioParameterFloat* highShelfFcParam;
    juce::AudioParameterFloat* highShelfGainParam;
    
    juce::AudioParameterFloat* highPassFcParam;
    juce::AudioParameterBool*  highPassActiveParam;
    
    // filters
    stk::BiQuad lowShelfFilterL;
    stk::BiQuad lowShelfFilterR;
    
    stk::BiQuad lowMidFilterL;
    stk::BiQuad lowMidFilterR;
    
    stk::BiQuad highMidFilterL;
    stk::BiQuad highMidFilterR;
    
    stk::BiQuad highShelfFilterL;
    stk::BiQuad highShelfFilterR;
    
    stk::BiQuad highPassFilterL;
    stk::BiQuad highPassFilterR;
    
    float mFs; //sampling rate
};
