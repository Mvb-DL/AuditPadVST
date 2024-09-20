#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>

// Konstruktor des SimpleAudioProcessor
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

// Destruktor des SimpleAudioProcessor
SimpleAudioProcessor::~SimpleAudioProcessor() {}

// Gibt den Namen des Plugins zurück
const juce::String SimpleAudioProcessor::getName() const {
    return JucePlugin_Name;
}

// MIDI-bezogene Funktionen
bool SimpleAudioProcessor::acceptsMidi() const { return false; }
bool SimpleAudioProcessor::producesMidi() const { return false; }
bool SimpleAudioProcessor::isMidiEffect() const { return false; }
double SimpleAudioProcessor::getTailLengthSeconds() const { return 0.0; }

// Programmbezogene Funktionen
int SimpleAudioProcessor::getNumPrograms() { return 1; }
int SimpleAudioProcessor::getCurrentProgram() { return 0; }
void SimpleAudioProcessor::setCurrentProgram(int) {}
const juce::String SimpleAudioProcessor::getProgramName(int) { return {}; }
void SimpleAudioProcessor::changeProgramName(int, const juce::String&) {}

// Vorbereitung des Prozessors vor der Wiedergabe
void SimpleAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Hier können Sie Code zur Vorbereitung vor der Wiedergabe hinzufügen
}

// Freigabe von Ressourcen, wenn das Plugin nicht mehr verwendet wird
void SimpleAudioProcessor::releaseResources() {
    // Hier können Sie Code zur Freigabe von Ressourcen hinzufügen
}

// Überprüfen, ob das Bus-Layout unterstützt wird
bool SimpleAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    // Unterstützt nur Mono- oder Stereoausgabe
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
#endif
}

// Verarbeiten des Audio-Blocks
void SimpleAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Leeren der Ausgabekanäle, wenn keine Eingabekanäle vorhanden sind
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Verarbeitung der Audiodaten
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        // Hier kann die Audiobearbeitung erfolgen
    }
}

// Gibt zurück, ob der Prozessor einen Editor hat
bool SimpleAudioProcessor::hasEditor() const { return true; }

// Erzeugt den Editor für das Plugin
juce::AudioProcessorEditor* SimpleAudioProcessor::createEditor() {
    return new SimpleAudioProcessorEditor(*this);
}

// Speichern des Zustands des Plugins
void SimpleAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    // Hier kann der Zustand des Plugins gespeichert werden
}

// Laden des Zustands des Plugins
void SimpleAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    // Hier kann der Zustand des Plugins geladen werden
}

// Abfragen der aktuellen Zeit in Sekunden von der DAW's Playhead
double SimpleAudioProcessor::getCurrentTimeInSeconds() const {
    if (auto* playHead = getPlayHead()) {
        if (auto positionInfo = playHead->getPosition()) {
            if (positionInfo->getTimeInSeconds()) {
                return *positionInfo->getTimeInSeconds();  // Gibt die Zeit in Sekunden zurück
            }
        }
    }
    return 0.0;  // Standardwert zurückgeben, wenn keine Zeitinformationen verfügbar sind
}

// Abfragen der aktuellen Sample-Position von der DAW's Playhead
int SimpleAudioProcessor::getCurrentSamples() const {
    if (auto* playHead = getPlayHead()) {
        if (auto positionInfo = playHead->getPosition()) {
            if (positionInfo->getTimeInSamples()) {
                return static_cast<int>(*positionInfo->getTimeInSamples());  // Gibt die Sample-Position zurück
            }
        }
    }
    return 0;  // Standardwert zurückgeben, wenn keine Sample-Informationen verfügbar sind
}

// Hinzufügen eines Zeitstempels mit Text zur Liste
void SimpleAudioProcessor::addTimestampWithText(double timeInSeconds, int timeInSamples, const juce::String& text) {
    timestampTextList.emplace_back(timeInSeconds, timeInSamples, text);
}

// Abfragen aller Zeitstempel mit den zugehörigen Texten
std::vector<std::tuple<double, int, juce::String>> SimpleAudioProcessor::getAllTimestampWithText() const {
    return timestampTextList;
}

// Abfragen aller Zeitstempel (nur Zeit)
std::vector<double> SimpleAudioProcessor::getAllTimestamps() const {
    std::vector<double> timestamps;
    for (const auto& tuple : timestampTextList) {
        timestamps.push_back(std::get<0>(tuple)); // Zugriff auf das erste Element des std::tuple
    }
    return timestamps;
}

// Erzeugt eine Instanz des Audio-Prozessors
juce::AudioProcessor* createPluginFilter() {
    return new SimpleAudioProcessor();
}
