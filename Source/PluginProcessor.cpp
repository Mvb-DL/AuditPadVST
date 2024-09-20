#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>

// Constructor of SimpleAudioProcessor
SimpleAudioProcessor::SimpleAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

// Destructor of SimpleAudioProcessor
SimpleAudioProcessor::~SimpleAudioProcessor() {}

// Returns the name of the plugin
const juce::String SimpleAudioProcessor::getName() const {
    return JucePlugin_Name;
}

// MIDI related functions
bool SimpleAudioProcessor::acceptsMidi() const { return false; }
bool SimpleAudioProcessor::producesMidi() const { return false; }
bool SimpleAudioProcessor::isMidiEffect() const { return false; }
double SimpleAudioProcessor::getTailLengthSeconds() const { return 0.0; }

// Program related functions
int SimpleAudioProcessor::getNumPrograms() { return 1; }
int SimpleAudioProcessor::getCurrentProgram() { return 0; }
void SimpleAudioProcessor::setCurrentProgram(int) {}
const juce::String SimpleAudioProcessor::getProgramName(int) { return {}; }
void SimpleAudioProcessor::changeProgramName(int, const juce::String&) {}

// Prepare the processor before playback starts
void SimpleAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Preparation before playback
}

// Release resources when the plugin is no longer in use
void SimpleAudioProcessor::releaseResources() {
    // Resource release, e.g., when closing
}

// Check if the bus layout is supported
bool SimpleAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
#endif
}

// Process the audio block
void SimpleAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear the output buffer if there are no input channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Process each input channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        // Process audio data here
    }
}

// Return whether the processor has an editor
bool SimpleAudioProcessor::hasEditor() const { return true; }

// Create the editor for the plugin
juce::AudioProcessorEditor* SimpleAudioProcessor::createEditor() {
    return new SimpleAudioProcessorEditor(*this);
}

// Save the state of the plugin
void SimpleAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    // Save plugin state (add your state saving logic here if needed)
}

// Load the state of the plugin
void SimpleAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    // Load plugin state (add your state loading logic here if needed)
}


// Retrieve the current time in seconds from the DAW's playhead
double SimpleAudioProcessor::getCurrentTimeInSeconds() const
{
    if (auto* playHead = getPlayHead())  // Überprüfen, ob ein Playhead vorhanden ist
    {
        if (auto positionInfo = playHead->getPosition())  // Abrufen der PositionInfo über std::optional
        {
            if (positionInfo->getTimeInSeconds())  // Überprüfen, ob timeInSeconds vorhanden ist
                return *positionInfo->getTimeInSeconds();  // Optional-Value dereferenzieren und zurückgeben
        }
    }

    return 0.0;  // Standardwert zurückgeben, wenn kein Playhead oder keine PositionInfo verfügbar ist
}

// Retrieve the current sample position from the DAW's playhead
int SimpleAudioProcessor::getCurrentSamples() const
{
    if (auto* playHead = getPlayHead())  // Überprüfen, ob ein Playhead vorhanden ist
    {
        if (auto positionInfo = playHead->getPosition())  // Abrufen der PositionInfo über std::optional
        {
            if (positionInfo->getTimeInSamples())  // Überprüfen, ob timeInSamples vorhanden ist
                return static_cast<int>(*positionInfo->getTimeInSamples());  // Optional-Value dereferenzieren und zurückgeben
        }
    }

    return 0;  // Standardwert zurückgeben, wenn kein Playhead oder keine PositionInfo verfügbar ist
}



// Add a timestamp with text to the list
void SimpleAudioProcessor::addTimestampWithText(double timeInSeconds, int timeInSamples, const juce::String& text)
{
    // Add elements to the list using emplace_back
    timestampTextList.emplace_back(timeInSeconds, timeInSamples, text);
}

// Get all timestamps with their corresponding text
std::vector<std::tuple<double, int, juce::String>> SimpleAudioProcessor::getAllTimestampWithText() const
{
    return timestampTextList;
}

// Get all timestamps (time only)
std::vector<double> SimpleAudioProcessor::getAllTimestamps() const
{
    std::vector<double> timestamps;

    for (const auto& tuple : timestampTextList)
    {
        timestamps.push_back(std::get<0>(tuple)); // Access the first element of the std::tuple
    }
    return timestamps;
}

// Create an instance of the audio processor
juce::AudioProcessor* createPluginFilter()
{
    return new SimpleAudioProcessor();
}
