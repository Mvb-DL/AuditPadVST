#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SimpleAudioProcessorEditor : public juce::AudioProcessorEditor,
                                   public juce::Button::Listener,       // Listener für den Button hinzufügen
                                   public juce::TextEditor::Listener    // Listener für den TextEditor hinzufügen
{
public:
    SimpleAudioProcessorEditor(SimpleAudioProcessor&);
    ~SimpleAudioProcessorEditor() override;

    // JUCE-Basisfunktionen überschreiben
    void paint(juce::Graphics&) override;
    void resized() override;
    bool keyPressed(const juce::KeyPress& key) override;

    // Listener-Funktion für Button-Events
    void buttonClicked(juce::Button* button) override;

    // Listener-Funktion für TextEditor-Events
    void focusGained(juce::Component::FocusChangeType) override;

private:
    SimpleAudioProcessor& audioProcessor;   // Referenz auf den Audio-Prozessor
    juce::String displayText;               // Text zur Anzeige
    bool isSpacePressed = false;            // Status-Flag für die Leertaste

    juce::TextEditor textEditor;            // Textfeld zur Eingabe
    juce::TextButton saveButton {"Speichern"};  // Button zum Speichern der Eingabe
    juce::Label displayLabel;               // Label zur Anzeige des gespeicherten Zeitstempels und Textes

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleAudioProcessorEditor)
};
