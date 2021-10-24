/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "defines.h"

//==============================================================================
ColemanJP02EQAudioProcessor::ColemanJP02EQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    /* Low Shelf */
    addParameter(lowShelfFcParam = new juce::AudioParameterFloat("Low Shelf Cutoff (Hz)", //ParameterID
                                                                 "LowShelfFc", //parameterName
                                                                 LOWSHELF_FC_LOW, //minValue
                                                                 LOWSHELF_FC_HIGH, //maxValue
                                                                 LOWSHELF_FC_DEFAULT)); //defaultValue
    addParameter(lowShelfGainParam = new juce::AudioParameterFloat("Low Shelf Gain (dB)", //ParameterID
                                                                 "LowShelfGain", //parameterName
                                                                 DB_LOW, //minValue
                                                                 DB_HIGH, //maxValue
                                                                 DB_DEFAULT)); //defaultValue
    
    /* Low Mid */
    addParameter(lowMidFcParam = new juce::AudioParameterFloat("Low Mid Frequency (Hz)", //ParameterID
                                                                 "LowMidFreq", //parameterName
                                                               LOWMID_FC_LOW, //minValue
                                                               LOWMID_FC_HIGH, //maxValue
                                                               LOWMID_FC_DEFAULT)); //defaultValue
    addParameter(lowMidGainParam = new juce::AudioParameterFloat("Low Mid Gain (dB)", //ParameterID
                                                                 "LowMidGain", //parameterName
                                                                 DB_LOW, //minValue
                                                                 DB_HIGH, //maxValue
                                                                 DB_DEFAULT)); //defaultValue

    addParameter(lowMidQParam = new juce::AudioParameterFloat("Low Mid Q", //ParameterID
                                                                 "LowMidQ", //parameterName
                                                                 Q_LOW, //minValue
                                                                 Q_HIGH, //maxValue
                                                                 Q_DEFAULT)); //defaultValue
    
    /* High Mid */
    addParameter(highMidFcParam = new juce::AudioParameterFloat("High Mid Frequency (Hz)", //ParameterID
                                                                 "HighMidFreq", //parameterName
                                                                HIGHMID_FC_LOW, //minValue
                                                                HIGHMID_FC_HIGH, //maxValue
                                                                HIGHMID_FC_DEFAULT)); //defaultValue
    addParameter(highMidGainParam = new juce::AudioParameterFloat("High Mid Gain (dB)", //ParameterID
                                                                 "HighMidGain", //parameterName
                                                                  DB_LOW, //minValue
                                                                  DB_HIGH, //maxValue
                                                                  DB_DEFAULT)); //defaultValue
    addParameter(highMidQParam = new juce::AudioParameterFloat("High Mid Q", //ParameterID
                                                               "HighMidQ", //parameterName
                                                               Q_LOW, //minValue
                                                               Q_HIGH, //maxValue
                                                               Q_DEFAULT)); //defaultValue

    /* High Shelf */
    addParameter(highShelfFcParam = new juce::AudioParameterFloat("High Shelf Cutoff (Hz)", //ParameterID
                                                                 "HighShelfFc", //parameterName
                                                                  HIGHSHELF_FC_LOW, //minValue
                                                                  HIGHSHELF_FC_HIGH, //maxValue
                                                                  HIGHSHELF_FC_DEFAULT)); //defaultValue
    addParameter(highShelfGainParam = new juce::AudioParameterFloat("High Shelf Gain (dB)", //ParameterID
                                                                 "HighShelfGain", //parameterName
                                                                    DB_LOW, //minValue
                                                                    DB_HIGH, //maxValue
                                                                    DB_DEFAULT)); //defaultValue

    /* High Pass */
    addParameter(highPassFcParam = new juce::AudioParameterFloat("High Pass Cutoff (Hz)", //ParameterID
                                                                 "HighPassFc", //parameterName
                                                                 HIGHPASS_FC_LOW, //minValue
                                                                 HIGHPASS_FC_HIGH, //maxValue
                                                                 HIGHPASS_FC_DEFAULT)); //defaultValue
    addParameter(highPassActiveParam = new juce::AudioParameterBool("High Pass Active",
                                                                    "HighPassActive",
                                                                    HIGHPASS_ACTIVE_DEFAULT));
}

ColemanJP02EQAudioProcessor::~ColemanJP02EQAudioProcessor()
{
}

//==============================================================================
const juce::String ColemanJP02EQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ColemanJP02EQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ColemanJP02EQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ColemanJP02EQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ColemanJP02EQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ColemanJP02EQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ColemanJP02EQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ColemanJP02EQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ColemanJP02EQAudioProcessor::getProgramName (int index)
{
    return {};
}

void ColemanJP02EQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ColemanJP02EQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mFs = sampleRate;
}

void ColemanJP02EQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ColemanJP02EQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
//    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
    
    /* removed mono ability to force stereo in host */
    if(layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()){
        return false;
    }

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

// helper method to print filter coefficients
void printArr(std::string name, float* arr) {
    std::string s = name + ":\n";
    // a's for website, b's for this library
    s += std::to_string(arr[0]) + "\n";
    s += std::to_string(arr[1]) + "\n";
    s += std::to_string(arr[2]) + "\n";
    s += "(1.0)\n";
    s += std::to_string(arr[3]) + "\n";
    s += std::to_string(arr[4]) + "\n";
    DBG(s);
}

//int loopVar = 0;

void ColemanJP02EQAudioProcessor::calcAlgorithmParams(){
    float coeffs1[5], coeffs2[5], coeffs3[5], coeffs4[5], coeffs5[5];
    
    /* High Pass Filter */
    Mu45FilterCalc::calcCoeffsHPF(coeffs1,
                                  highPassFcParam->get(),
                                  HIGHPASS_Q, mFs);
    highPassFilterL.setCoefficients(coeffs1[0],coeffs1[1],
                                    coeffs1[2],coeffs1[3],
                                    coeffs1[4]);
    highPassFilterR.setCoefficients(coeffs1[0],coeffs1[1],
                                    coeffs1[2],coeffs1[3],
                                    coeffs1[4]);
    
    
    /* Low Shelf Filter */
    Mu45FilterCalc::calcCoeffsLowShelf(coeffs2,
                                  lowShelfFcParam->get(),
                                  lowShelfGainParam->get(), mFs);
    lowShelfFilterL.setCoefficients(coeffs2[0],coeffs2[1],
                                    coeffs2[2],coeffs2[3],
                                    coeffs2[4]);
    lowShelfFilterR.setCoefficients(coeffs2[0],coeffs2[1],
                                    coeffs2[2],coeffs2[3],
                                    coeffs2[4]);
    
//    DBG("low shelf gain: " << std::to_string(lowShelfGainParam2->get()));
    
    /* Low Mid Filter */
    Mu45FilterCalc::calcCoeffsPeak(coeffs3,
                                  lowMidFcParam->get(),
                                  lowMidGainParam->get(),
                                  lowMidQParam->get(), mFs);
    lowMidFilterL.setCoefficients(coeffs3[0],coeffs3[1],
                                  coeffs3[2],coeffs3[3],
                                  coeffs3[4]);
    lowMidFilterR.setCoefficients(coeffs3[0],coeffs3[1],
                                  coeffs3[2],coeffs3[3],
                                  coeffs3[4]);
    
    /* High Mid Filter */
    Mu45FilterCalc::calcCoeffsPeak(coeffs4,
                                  highMidFcParam->get(),
                                  highMidGainParam->get(),
                                  highMidQParam->get(), mFs);
    highMidFilterL.setCoefficients(coeffs4[0],coeffs4[1],
                                   coeffs4[2],coeffs4[3],
                                   coeffs4[4]);
    highMidFilterR.setCoefficients(coeffs4[0],coeffs4[1],
                                   coeffs4[2],coeffs4[3],
                                   coeffs4[4]);
    
    /* High Shelf Filter */
    Mu45FilterCalc::calcCoeffsHighShelf(coeffs5,
                                  highShelfFcParam->get(),
                                  highShelfGainParam->get(), mFs);
    highShelfFilterL.setCoefficients(coeffs5[0],coeffs5[1],
                                     coeffs5[2],coeffs5[3],
                                     coeffs5[4]);
    highShelfFilterR.setCoefficients(coeffs5[0],coeffs5[1],
                                     coeffs5[2],coeffs5[3],
                                     coeffs5[4]);
    
//    if (loopVar > 100) {
//        DBG("lowShelfFcParam: " << std::to_string(lowShelfFcParam->get()));
//        DBG("lowShelfGainParam: " << std::to_string(lowShelfGainParam->get()));
//
//        DBG("lowMidFcParam: " << std::to_string(lowMidFcParam->get()));
//        DBG("lowMidGainParam: " << std::to_string(lowMidGainParam->get()));
//        DBG("lowMidQParam: " << std::to_string(lowMidQParam->get()));
//
//        DBG("highMidFcParam: " << std::to_string(highMidFcParam->get()));
//        DBG("highMidGainParam: " << std::to_string(highMidGainParam->get()));
//        DBG("highMidQParam: " << std::to_string(highMidQParam->get()));
//
//        DBG("highShelfFcParam: " << std::to_string(highShelfFcParam->get()));
//        DBG("highShelfGainParam: " << std::to_string(highShelfGainParam->get()));
//
//        DBG("highPassFcParam: " << std::to_string(highPassFcParam->get()));
//        DBG("highPassActiveParam: " << std::to_string(highPassActiveParam->get()));
//
//        DBG("Filter coeffs");
//        printArr("High pass", coeffs1);
//        printArr("Low shelf", coeffs2);
//        printArr("Low mid", coeffs3);
//        printArr("High mid", coeffs4);
//        printArr("High shelf", coeffs5);
//        loopVar = 0;
//    }
//    loopVar++;

}

void ColemanJP02EQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    calcAlgorithmParams();
    
    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = buffer.getWritePointer(1);

    for (int samp = 0; samp < buffer.getNumSamples(); samp++) {
        float left = leftChannel[samp];
        float right = rightChannel[samp];
        
        // filter left
        if (highPassActiveParam->get())
            left = highPassFilterL.tick(left);
        left = lowShelfFilterL.tick(left);
        left = lowMidFilterL.tick(left);
        left = highMidFilterL.tick(left);
        left = highShelfFilterL.tick(left);
        
        // filter right
        if (highPassActiveParam->get())
            right = highPassFilterR.tick(right);
        right = lowShelfFilterR.tick(right);
        right = lowMidFilterR.tick(right);
        right = highMidFilterR.tick(right);
        right = highShelfFilterR.tick(right);
        
        // save
        leftChannel[samp] = left;
        rightChannel[samp] = right;
    }
}

//==============================================================================
bool ColemanJP02EQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ColemanJP02EQAudioProcessor::createEditor()
{
    return new ColemanJP02EQAudioProcessorEditor (*this);
}

//==============================================================================
void ColemanJP02EQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ColemanJP02EQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ColemanJP02EQAudioProcessor();
}
