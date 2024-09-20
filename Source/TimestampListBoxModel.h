#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class TimestampListBoxModel : public juce::ListBoxModel
{
public:
    TimestampListBoxModel(SimpleAudioProcessor& processor) : audioProcessor(processor) {}

    int getNumRows() override
    {
        return static_cast<int>(audioProcessor.getAllTimestampWithText().size());
    }

    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override
    {
        auto timestampList = audioProcessor.getAllTimestampWithText();

        if (rowNumber < timestampList.size())
        {
            auto [time, sample, text] = timestampList[rowNumber];

            juce::String displayText = "Time: " + juce::String(time, 2) + "s, Sample: " + juce::String(sample) + ", Text: " + text;
            g.setColour(rowIsSelected ? juce::Colours::lightblue : juce::Colours::white);
            g.drawText(displayText, 0, 0, width, height, juce::Justification::centredLeft, true);
        }
    }

private:
    SimpleAudioProcessor& audioProcessor;
};
