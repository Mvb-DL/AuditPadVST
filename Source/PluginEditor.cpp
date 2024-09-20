#include "PluginEditor.h"
#include "PluginProcessor.h"

// Konstruktor des Editors
SimpleAudioProcessorEditor::SimpleAudioProcessorEditor(SimpleAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), listBoxModel(p), displayText("Press Space"), isSpacePressed(false)
{
    setSize(400, 300); // Fenstergröße des Editors

    // TextEditor initialisieren
    textEditor.setMultiLine(true);
    textEditor.setReturnKeyStartsNewLine(true);
    textEditor.setText("Bitte Text eingeben...");
    textEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
    textEditor.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    textEditor.setColour(juce::TextEditor::outlineColourId, juce::Colours::grey);
    textEditor.addListener(this);

    // Save-Button initialisieren und konfigurieren
    saveButton.setButtonText("Speichern");
    saveButton.onClick = [this]()
    {
        double currentTime = audioProcessor.getCurrentTimeInSeconds();
        int currentSample = audioProcessor.getCurrentSamples();
        juce::String userInput = textEditor.getText();

        if (!userInput.isEmpty())
        {
            audioProcessor.addTimestampWithText(currentTime, currentSample, userInput);
            textEditor.clear(); 
            displayLabel.setText("Gespeichert: [ Time: " + juce::String(currentTime, 2) + "s. Sample: " + juce::String(currentSample) + " ] Input: " + userInput, juce::dontSendNotification);

            textEditor.setVisible(false);
            saveButton.setVisible(false);
            displayText = "Press Space";
            isSpacePressed = false;

            listBox.updateContent(); // Aktualisieren der ListBox, wenn ein neuer Eintrag hinzugefügt wurde
            repaint();
        }
    };

    // Label zur Anzeige des gespeicherten Textes initialisieren
    displayLabel.setText("", juce::dontSendNotification);
    displayLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    displayLabel.setFont(15.0f);
    displayLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(displayLabel);

    // ListBox initialisieren und hinzufügen
    listBox.setModel(&listBoxModel); // Setzt das ListBoxModel, das in der Header-Datei definiert wurde
    addAndMakeVisible(listBox);

    setWantsKeyboardFocus(true);
    grabKeyboardFocus();
}

SimpleAudioProcessorEditor::~SimpleAudioProcessorEditor()
{
    textEditor.removeListener(this);
}

void SimpleAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText(displayText, getLocalBounds(), juce::Justification::centred, 1);
}

void SimpleAudioProcessorEditor::resized()
{
    textEditor.setBounds(10, 10, getWidth() - 20, 50);
    saveButton.setBounds(10, 70, getWidth() - 20, 30);
    displayLabel.setBounds(10, 110, getWidth() - 20, 30);
    listBox.setBounds(10, 250, getWidth() - 20, getHeight() - 260); // ListBox für gespeicherte Einträge
}

bool SimpleAudioProcessorEditor::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::spaceKey)
    {
        if (!isSpacePressed)
        {
            addAndMakeVisible(textEditor);
            addAndMakeVisible(saveButton);

            textEditor.setVisible(true);
            saveButton.setVisible(true);
            textEditor.grabKeyboardFocus();
            displayText = "Geben Sie Text ein und klicken Sie auf Speichern";
            isSpacePressed = true;
        }
        else
        {
            textEditor.setVisible(false);
            saveButton.setVisible(false);
            displayText = "Press Space";
            isSpacePressed = false;
        }

        repaint();
        return true;
    }

    return false;
}

void SimpleAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &saveButton)
    {
        double currentTime = audioProcessor.getCurrentTimeInSeconds();
        int currentSample = audioProcessor.getCurrentSamples();
        juce::String userInput = textEditor.getText();

        if (!userInput.isEmpty())
        {
            audioProcessor.addTimestampWithText(currentTime, currentSample, userInput);
            textEditor.clear();

            displayLabel.setText("Gespeichert: [ Time: " + juce::String(currentTime, 2) + "s. Sample: " + juce::String(currentSample) + " ] Input: " + userInput, juce::dontSendNotification);

            textEditor.setVisible(false);
            saveButton.setVisible(false);
            displayText = "Press Space";
            isSpacePressed = false;

            listBox.updateContent(); // Aktualisieren der ListBox
            repaint();
        }
    }
}

void SimpleAudioProcessorEditor::focusGained(juce::Component::FocusChangeType)
{
    if (textEditor.getText() == "Bitte Text eingeben...")
    {
        textEditor.clear();
    }
}
