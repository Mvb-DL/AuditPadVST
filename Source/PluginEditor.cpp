#include "PluginEditor.h"
#include "PluginProcessor.h"

SimpleAudioProcessorEditor::SimpleAudioProcessorEditor(SimpleAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), displayText("Press Space"), isSpacePressed(false)
{
    setSize(400, 300); // Fenstergröße des Editors

    // TextEditor initialisieren
    textEditor.setMultiLine(true); // Mehrzeiliges Textfeld
    textEditor.setReturnKeyStartsNewLine(true); // Eingabetaste startet eine neue Zeile
    textEditor.setText("Bitte Text eingeben..."); // Standardtext im Textfeld
    textEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black); // Hintergrundfarbe
    textEditor.setColour(juce::TextEditor::textColourId, juce::Colours::white); // Textfarbe
    textEditor.setColour(juce::TextEditor::outlineColourId, juce::Colours::grey); // Rahmenfarbe

    // TextEditor Listener hinzufügen
    textEditor.addListener(this); // Fügen Sie diesen Editor als Listener hinzu

    // Save-Button initialisieren und konfigurieren
    saveButton.setButtonText("Speichern");
    saveButton.onClick = [this]()
    {
        double currentTime = audioProcessor.getCurrentTimeInSeconds();
        int currentSample = audioProcessor.getCurrentSamples();
        juce::String userInput = textEditor.getText();

        if (!userInput.isEmpty())
        {
            audioProcessor.addTimestampWithText(currentTime, currentSample, userInput); // Zeitstempel und Text speichern
            textEditor.clear(); // Textfeld nach dem Speichern leeren

            // Zeitstempel und Text im Label anzeigen
            displayLabel.setText("Gespeichert: [ Time: " + juce::String(currentTime, 2) + "s. Sample: " + juce::String(currentSample) + " ] Input: " + userInput, juce::dontSendNotification);

            // TextEditor und Save-Button nach dem Speichern ausblenden
            textEditor.setVisible(false);
            saveButton.setVisible(false);
            displayText = "Press Space"; // Hinweistext zurücksetzen
            isSpacePressed = false; // Status-Flag zurücksetzen

            repaint(); // Neu zeichnen, um den neuen Zustand anzuzeigen
        }
    };

    // Label zur Anzeige des gespeicherten Textes initialisieren
    displayLabel.setText("", juce::dontSendNotification); // Initialer Text
    displayLabel.setColour(juce::Label::textColourId, juce::Colours::white); // Textfarbe
    displayLabel.setFont(15.0f); // Schriftgröße
    displayLabel.setJustificationType(juce::Justification::centred); // Zentrierter Text
    addAndMakeVisible(displayLabel); // Label sichtbar machen

    // Ermöglicht die Tastaturerfassung
    setWantsKeyboardFocus(true);
    grabKeyboardFocus(); // Sicherstellen, dass der Editor den Fokus hat
}

SimpleAudioProcessorEditor::~SimpleAudioProcessorEditor()
{
    textEditor.removeListener(this); // Listener entfernen, wenn der Editor zerstört wird
}

void SimpleAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black); // Hintergrundfarbe des Editors

    g.setColour(juce::Colours::white); // Textfarbe auf Weiß setzen
    g.setFont(15.0f); // Schriftgröße setzen

    g.drawFittedText(displayText, getLocalBounds(), juce::Justification::centred, 1); // Display-Text zentriert zeichnen
}

void SimpleAudioProcessorEditor::resized()
{
    // TextEditor und Save-Button positionieren
    textEditor.setBounds(10, 10, getWidth() - 20, getHeight() - 100); // TextEditor mit Rand
    saveButton.setBounds(10, getHeight() - 80, getWidth() - 20, 30); // Save-Button unter dem TextEditor
    displayLabel.setBounds(10, getHeight() - 40, getWidth() - 20, 30); // Label zur Anzeige des gespeicherten Texts
}

bool SimpleAudioProcessorEditor::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::spaceKey)  // Überprüfen, ob die Leertaste gedrückt wurde
    {
        if (!isSpacePressed) // Wenn Leertaste zum ersten Mal gedrückt wurde
        {   
            
            addAndMakeVisible(textEditor); 
            addAndMakeVisible(saveButton);
            
            
            textEditor.setVisible(true); // TextEditor sichtbar machen
            saveButton.setVisible(true); // Save-Button sichtbar machen
            textEditor.grabKeyboardFocus(); // Tastaturfokus auf TextEditor setzen
            displayText = "Geben Sie Text ein und klicken Sie auf Speichern"; // Hinweistext ändern
            isSpacePressed = true; // Status-Flag setzen
        }
        else 
        {
            textEditor.setVisible(false); // TextEditor unsichtbar machen
            saveButton.setVisible(false); // Save-Button unsichtbar machen
            displayText = "Press Space"; // Hinweistext zurücksetzen
            isSpacePressed = false; // Status-Flag zurücksetzen
        }

        repaint();  // Neu zeichnen, um den neuen Zustand anzuzeigen

        return true;  // Signalisiert, dass die Taste verarbeitet wurde
    }

    return false;  // Andere Tasten werden nicht verarbeitet
}

void SimpleAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &saveButton) // Wenn der Save-Button geklickt wurde
    {
        double currentTime = audioProcessor.getCurrentTimeInSeconds();
        int currentSample = audioProcessor.getCurrentSamples();
        juce::String userInput = textEditor.getText();

        if (!userInput.isEmpty())
        {
            audioProcessor.addTimestampWithText(currentTime, currentSample, userInput);
            textEditor.clear(); // Textfeld nach dem Speichern leeren

            // Zeitstempel und Text im Label anzeigen
            displayLabel.setText("Gespeichert: [ Time: " + juce::String(currentTime, 2) + "s. Sample: " + juce::String(currentSample) + " ] Input: " + userInput, juce::dontSendNotification);

            // TextEditor und Save-Button ausblenden
            textEditor.setVisible(false);
            saveButton.setVisible(false);
            displayText = "Press Space"; // Hinweistext zurücksetzen
            isSpacePressed = false; // Status-Flag zurücksetzen

            repaint(); // Neu zeichnen, um den neuen Zustand anzuzeigen
        }
    }
}

void SimpleAudioProcessorEditor::focusGained(juce::Component::FocusChangeType)
{
    if (textEditor.getText() == "Bitte Text eingeben...") // Prüfen, ob der Standardtext noch vorhanden ist
    {
        textEditor.clear(); // Textfeld leeren
    }
}
