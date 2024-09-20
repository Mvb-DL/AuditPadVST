#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <tuple>

class SimpleAudioProcessor : public juce::AudioProcessor
{
public:
    SimpleAudioProcessor();
    ~SimpleAudioProcessor() override;

    // AudioProcessor Overrides
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Custom methods to get information from the playhead
    double getCurrentTimeInSeconds() const;  // Retrieves the current time in seconds from the DAW's playhead
    int getCurrentSamples() const;           // Retrieves the current sample position from the DAW's playhead

    // Methods to manage timestamps and text entries
    void addTimestampWithText(double timeInSeconds, int timeInSamples, const juce::String& text);
    std::vector<double> getAllTimestamps() const;  // Retrieves all timestamps (time only)
    std::vector<std::tuple<double, int, juce::String>> getAllTimestampWithText() const;  // Retrieves all timestamps with associated text

private:
    std::vector<std::tuple<double, int, juce::String>> timestampTextList;  // List to store timestamps and associated texts

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleAudioProcessor)
};
