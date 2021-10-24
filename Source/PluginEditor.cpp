/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "defines.h"

void ColemanJP02EQAudioProcessorEditor::makeRotarySlider(int x, int y,
                        juce::Slider& slider, parameterMap paramNum,
                        std::string units, float rangeInterval, bool freqslider) {
    
    auto& params = processor.getParameters();
    
    juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(paramNum);
    slider.setBounds(x, y, SLIDER_WIDTH, SLIDER_HEIGHT);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    slider.setTextValueSuffix(units);
    slider.setRange(audioParam->range.start, audioParam->range.end, rangeInterval);
    slider.setValue(*audioParam);
    slider.addListener(this);
    if (freqslider)
        slider.setSkewFactor(FREQ_SKEW); // makes slider roughly logarithmic
    addAndMakeVisible(slider);
}

void ColemanJP02EQAudioProcessorEditor::makeFilterTitle(int x, int y,
                                                        juce::Label& label, std::string name) {
    label.setText(name, juce::dontSendNotification);
    label.setBounds(x, y, SLIDER_WIDTH, 2*UNIT_LENGTH_Y);
    label.setJustificationType(juce::Justification::centredBottom);
    label.setFont(juce::Font (20.0f, juce::Font::bold));
    addAndMakeVisible(label);
}

void ColemanJP02EQAudioProcessorEditor::makeParamLabel(int y, juce::Label& label, std::string name) {
    label.setText(name, juce::dontSendNotification);
    label.setBounds(0, y, (LABEL_OFFSET + 4)*UNIT_LENGTH_X, SLIDER_HEIGHT);
    label.setJustificationType(juce::Justification::centred);
    label.setFont(juce::Font (16.0f));
    addAndMakeVisible(label);
}

//==============================================================================
ColemanJP02EQAudioProcessorEditor::ColemanJP02EQAudioProcessorEditor (ColemanJP02EQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(CONTAINER_WIDTH, CONTAINER_HEIGHT);
    
    /* toggle button */
    highPassActiveToggle.setBounds((int) 22*UNIT_LENGTH_X ,(int) 37*UNIT_LENGTH_Y,
                                   SLIDER_WIDTH*2, SLIDER_HEIGHT);
    highPassActiveToggle.setToggleState(HIGHPASS_ACTIVE_DEFAULT, juce::dontSendNotification);
    highPassActiveToggle.addListener(this);
    addAndMakeVisible(highPassActiveToggle);
    /* end toggle button */
    
    /* rotary sliders */
    // low shelf
    makeRotarySlider((int) (2+LABEL_OFFSET)*UNIT_LENGTH_X,(int) 6*UNIT_LENGTH_Y, lowShelfFcSlider, lowShelfFc, FREQ_SUFFIX, FREQ_RANGE_INTERVAL/10.0, true);
    makeRotarySlider((int) (2+LABEL_OFFSET)*UNIT_LENGTH_X,(int) 16*UNIT_LENGTH_Y, lowShelfGainSlider, lowShelfGain, GAIN_SUFFIX, GAIN_RANGE_INTERVAL, false);
    
    // low mid
    makeRotarySlider((int) (12+LABEL_OFFSET)*UNIT_LENGTH_X,(int) 6*UNIT_LENGTH_Y, lowMidFcSlider, lowMidFc, FREQ_SUFFIX, FREQ_RANGE_INTERVAL, true);
    makeRotarySlider((int) (12+LABEL_OFFSET)*UNIT_LENGTH_X,(int) 16*UNIT_LENGTH_Y, lowMidGainSlider, lowMidGain, GAIN_SUFFIX, GAIN_RANGE_INTERVAL, false);
    makeRotarySlider((int) (12+LABEL_OFFSET)*UNIT_LENGTH_X,(int) 26*UNIT_LENGTH_Y, lowMidQSlider, lowMidQ, Q_SUFFIX, Q_RANGE_INTERVAL, false);
    
    // high mid
    makeRotarySlider((int) (22+LABEL_OFFSET)*UNIT_LENGTH_X ,(int) 6*UNIT_LENGTH_Y, highMidFcSlider, highMidFc, FREQ_SUFFIX, FREQ_RANGE_INTERVAL, true);
    makeRotarySlider((int) (22+LABEL_OFFSET)*UNIT_LENGTH_X ,(int) 16*UNIT_LENGTH_Y, highMidGainSlider, highMidGain, GAIN_SUFFIX, GAIN_RANGE_INTERVAL, false);
    makeRotarySlider((int) (22+LABEL_OFFSET)*UNIT_LENGTH_X ,(int) 26*UNIT_LENGTH_Y, highMidQSlider, highMidQ, Q_SUFFIX, Q_RANGE_INTERVAL, false);
    
    // high shelf
    makeRotarySlider((int) (32+LABEL_OFFSET)*UNIT_LENGTH_X ,(int) 6*UNIT_LENGTH_Y, highShelfFcSlider, highShelfFc, FREQ_SUFFIX, FREQ_RANGE_INTERVAL, true);
    makeRotarySlider((int) (32+LABEL_OFFSET)*UNIT_LENGTH_X ,(int) 16*UNIT_LENGTH_Y, highShelfGainSlider, highShelfGain, GAIN_SUFFIX, GAIN_RANGE_INTERVAL, false);
    
    // high pass
    makeRotarySlider((int) 12*UNIT_LENGTH_X ,(int) 37*UNIT_LENGTH_Y, highPassFcSlider, highPassFc, FREQ_SUFFIX, FREQ_RANGE_INTERVAL, true);
    /* end rotary sliders*/
    
    /* filter labels */
    makeFilterTitle((2+LABEL_OFFSET)*UNIT_LENGTH_X, 2*UNIT_LENGTH_Y, lowShelfLabel, "Low Shelf");
    makeFilterTitle((12+LABEL_OFFSET)*UNIT_LENGTH_X, 2*UNIT_LENGTH_Y, lowMidLabel, "Low Mid");
    makeFilterTitle((22+LABEL_OFFSET)*UNIT_LENGTH_X, 2*UNIT_LENGTH_Y, highMidLabel, "High Mid");
    makeFilterTitle((32+LABEL_OFFSET)*UNIT_LENGTH_X, 2*UNIT_LENGTH_Y, highShelfLabel, "High Shelf");
    
    makeFilterTitle(2*UNIT_LENGTH_X, 40*UNIT_LENGTH_Y, highPassLabel, "High Pass");
    /* end filter labels */
    
    /* parameter labels */
    makeParamLabel(5*UNIT_LENGTH_Y, frequencyLabel, "Freq");
    makeParamLabel(15*UNIT_LENGTH_Y, gainLabel, "Gain");
    makeParamLabel(25*UNIT_LENGTH_Y, qLabel, "Q");
    /* end parameter labels */
}

ColemanJP02EQAudioProcessorEditor::~ColemanJP02EQAudioProcessorEditor()
{
}

void ColemanJP02EQAudioProcessorEditor::setParamFromSlider(juce::Slider& slider, parameterMap paramNum) {
    auto& params = processor.getParameters();
    juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(paramNum);
    *audioParam = slider.getValue();
}

void ColemanJP02EQAudioProcessorEditor::buttonStateChanged(juce::Button *button) {
    auto& params = processor.getParameters();
    juce::AudioParameterBool* audioParam = (juce::AudioParameterBool*)params.getUnchecked(highPassActive);
    *audioParam = button->getToggleState();
}

void ColemanJP02EQAudioProcessorEditor::buttonClicked(juce::Button *button) {
    
}

void ColemanJP02EQAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
    /* Low Shelf */
    if (slider == &lowShelfGainSlider) {
        setParamFromSlider(lowShelfGainSlider, lowShelfGain);
    } else if (slider == &lowShelfFcSlider) {
        setParamFromSlider(lowShelfFcSlider, lowShelfFc);
        
    /* Low Mid */
    } else if (slider == &lowMidGainSlider) {
        setParamFromSlider(lowMidGainSlider, lowMidGain);
    } else if (slider == &lowMidFcSlider) {
        setParamFromSlider(lowMidFcSlider, lowMidFc);
    } else if (slider == &lowMidQSlider) {
        setParamFromSlider(lowMidQSlider, lowMidQ);
        
    /* High Mid */
    } else if (slider == &highMidGainSlider) {
        setParamFromSlider(highMidGainSlider, highMidGain);
    } else if (slider == &highMidFcSlider) {
        setParamFromSlider(highMidFcSlider, highMidFc);
    } else if (slider == &highMidQSlider) {
        setParamFromSlider(highMidQSlider, highMidQ);

    /* High Shelf */
    } else if (slider == &highShelfGainSlider) {
        setParamFromSlider(highShelfGainSlider, highShelfGain);
    } else if (slider == &highShelfFcSlider) {
        setParamFromSlider(highShelfFcSlider, highShelfFc);

    /* High Pass */
    } else if (slider == &highPassFcSlider) {
        setParamFromSlider(highPassFcSlider, highPassFc);
    }
}

// helper method to draw vertical lines in the GUI
void createMainLine(float x, juce::Graphics& g) {
    juce::Line<float> line (juce::Point<float> (x, 1*UNIT_LENGTH_Y),
                            juce::Point<float> (x,35*UNIT_LENGTH_Y));
    g.drawLine(line, 2.0f);
}

//==============================================================================
void ColemanJP02EQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    
    // create vertical lines
    createMainLine((11+LABEL_OFFSET)*UNIT_LENGTH_X, g);
    createMainLine((21+LABEL_OFFSET)*UNIT_LENGTH_X, g);
    createMainLine((31+LABEL_OFFSET)*UNIT_LENGTH_X, g);
    
    // create horizontal line
    juce::Line<float> line (juce::Point<float> (1*UNIT_LENGTH_X, 36*UNIT_LENGTH_Y),
                            juce::Point<float> (CONTAINER_WIDTH - UNIT_LENGTH_X,36*UNIT_LENGTH_Y));
    g.drawLine(line, 2.0f);

}

void ColemanJP02EQAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
