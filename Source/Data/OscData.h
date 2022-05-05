//OscData.h

// For 'Oscillator' of the synthesiser

#pragma once
#include <JuceHeader.h>

const unsigned int max_Unison = 8;

typedef struct
{
	float oscChoice = 0, 
		oscGain = -10, // Loudness of sound
		oscPitch = 0, // High/low of sound
		detune = 0, // for enriched sound
		distortion = 1, // for enriched sound
		FmChoice = 0, 
		fmFreq = 0, // For FM modulation e.g. vibrato effect
		fmDepth = 0, // For FM modulation e.g. vibrato effect
		nObertones = 0, // for enriched sound
		pan = 0, // for enriched sound
		unison = 1; // for enriched sound (max: 8)
} OscParams;


class OscData
{
public:
	void renderNextBlock(juce::AudioBuffer<float>& synthBuffer);
	void setFreq(const int midiNoteNumber);
	void prepareToPlay(double sampleRate, int /*samplesPerBlock*/, int /*outputChannels*/);	
	void setParams(const OscParams& params);
	void resetAll();

private:
	float value1{ 0.3 };
	float freq1{ 0.f };
	int midiNoteNumber{ 0 };
	int freqSet{ 0 };
	float detuneVal{ 0.f };
	float distortionVal{ 1.f };

	float freq2{ 0.0f };
	float value2{ 0.0f };
	juce::dsp::Phase<float> OSCPhase1[max_Unison];
	juce::dsp::Phase<float> OSCPhase2;
	std::function<float(float)> OSC_1;
	std::function<float(float)> OSC_2;

	int oscType = -1;
	int fmType = -1;
	float pan = 0;
	int obertoneVal = 0;
	int unisonVal = 1;

	float sampleRate{ 48000.f };
	
	bool lookupTablesActual = false;
	std::unique_ptr<juce::dsp::LookupTableTransform<float>> meter1, meter2;

	void makeLookupTable1(const int osc1Selection);
	void makeLookupTable2(const int osc2Selection);

};