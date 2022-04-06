//CompressorComponent.h

//Code for the design of the Compressor.

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CompressorComponent : public juce::Component
{
public:
    CompressorComponent(juce::AudioProcessorValueTreeState& apvts, juce::String thresholdID,
        juce::String ratioID, juce::String attackID, juce::String releaseID);
    ~CompressorComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts,
        juce::String parameterID, std::unique_ptr<Attachment>& sliderAttachment);

    //Each of the Slider
    juce::Slider thresholdSlider;
    juce::Slider ratioSlider;
    juce::Slider attackSlider;
    juce::Slider releaseSlider;

    //Each of the label
    juce::Label thresholdLabel{ "Threshold", "Threshold" };
    juce::Label ratioLabel{ "Ratio", "Ratio" };
    juce::Label attackLabel{ "Attack", "Attack" };
    juce::Label releaseLabel{ "Release", "Release" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseSliderAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorComponent)
};
