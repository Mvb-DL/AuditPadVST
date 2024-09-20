#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "TimestampListBoxModel.h"  // Füge dies hinzu

class SimpleAudioProcessorEditor : public juce::AudioProcessorEditor,
                                   public juce::Button::Listener,       
                                   public juce::TextEditor::Listener    
{
public:
    SimpleAudioProcessorEditor(SimpleAudioProcessor&);
    ~SimpleAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    bool keyPressed(const juce::KeyPress& key) override;
    void buttonClicked(juce::Button* button) override;
    void focusGained(juce::Component::FocusChangeType) override;

private:
    SimpleAudioProcessor& audioProcessor;
    juce::String displayText;
    bool isSpacePressed = false;

    juce::TextEditor textEditor;
    juce::TextButton saveButton {"Speichern"};
    juce::Label displayLabel;

    juce::ListBox listBox;
    TimestampListBoxModel listBoxModel;  // Verwende die vollständige Klasse

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleAudioProcessorEditor)
};
