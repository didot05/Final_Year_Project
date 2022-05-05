//OscData.cpp

// For 'Oscillator' of the synthesiser

#include "OscData.h"
#define PIVal juce::MathConstants<float>::pi //PI 
#define DoublePIVal juce::MathConstants<float>::twoPi //PI * 2


float crop(float value, float minVal = 0.f, float maxVal = 1.f) {
	if (value < minVal) return minVal;
	if (value > maxVal) return maxVal;
	return value;
}
void make_Unison_Freq(float* freqVal, float freq1, int uniVal, float detunVal) {
	if (uniVal == 1) { freqVal[0] = freq1; return;}
	float detuneMax = freq1 * detunVal;
	float stage = detuneMax / uniVal * 2;
	float currentFreqVal = freq1 - detuneMax;
	for (int i = 0; i < uniVal; i++) {
		freqVal[i] = currentFreqVal;
		currentFreqVal += stage;
	}
}
void OscData::renderNextBlock(juce::AudioBuffer<float>& synthBuffer) {
	int numSamples = synthBuffer.getNumSamples();
	
	float *bufferLeft, *bufferRight;

	bufferLeft = synthBuffer.getWritePointer(0, 0);
	bufferRight = synthBuffer.getWritePointer(1, 0);
	
	float coEffLeft = pow(crop(1 - pan), 2.f),
		coEffRight = pow(crop(pan + 1), 2.f);
	
	float freqVal[max_Unison];

	freq1 = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber + freqSet) + detuneVal;
	make_Unison_Freq(freqVal, freq1, unisonVal, 0.01);

	for (int a = 0; a < numSamples; a++)
	{
		float OSCPhase2Increase = DoublePIVal * freq2 / sampleRate;
		float OSCPhase2Val = OSCPhase2.advance(OSCPhase2Increase) - PIVal;

		float sample = 0;
		for (int i = 0; i < unisonVal; i++) {
			float O1PhaseIncrement = DoublePIVal * freqVal[i] / sampleRate + value2 * freqVal[i] * OSC_2(OSCPhase2Val);
			float O1PhaseVal = OSCPhase1[i].advance(O1PhaseIncrement) - PIVal;
			sample += value1 * OSC_1(O1PhaseVal)/ unisonVal;
		}

		sample = (sample > 0.f) ? pow(sample, distortionVal) : -pow(abs(sample), distortionVal);

		//stereo panoram
		bufferLeft[a] += coEffLeft * sample;
		bufferRight[a] += coEffRight * sample;
	}
}

void OscData::prepareToPlay(double sampleRate, int /*samplesPerBlock*/, int /*outputChannels*/) { this->sampleRate = sampleRate; }

void OscData::setFreq(const int midiNoteNumber) { this->midiNoteNumber = midiNoteNumber; }

void OscData::setParams(const OscParams& params)
{
	value1 = std::pow(10.f, params.oscGain / 20.f);
	value2 = params.fmDepth / 2000.f;
	freqSet = params.oscPitch;
	freq2 = params.fmFreq;
	pan = params.pan;
	detuneVal = params.detune;
	distortionVal = params.distortion;
	unisonVal = params.unison;

	if (oscType != (int)params.oscChoice || obertoneVal != (int)params.nObertones) { oscType = params.oscChoice; obertoneVal = params.nObertones; makeLookupTable1(oscType); }
	if (fmType != (int)params.FmChoice) { fmType = params.FmChoice; makeLookupTable2(fmType); }
}

float triangle(float x) { return std::asin(std::sin(x)) * 2.f / PIVal; }
float saw(float x) { return std::atan(std::tan(-x / 2.f + PIVal / 2)) * 2.f / PIVal; }
float sawSmooth(float x) {
	float outputAudio = 0.f, revise = 12.f;
	for (float n = 1.f; n < revise; n++)
		outputAudio += (std::sin(n * x)) / n;
	return outputAudio * 2.f / PIVal;
}
float square(float x) { return std::floor(std::sin(x)) * 2.f + 1.f; }
float randomFloat(float x) { return rand() / float(RAND_MAX) * 2.f - 1.f; }
float reverseSawtooth(float x) { return -saw(x); }
float softSine(float x) { return tanh(1.2 * std::sin(x)); }
float hardSine(float x) {
	double outputAudioS = 1.3 * (std::sin(x));
	if (outputAudioS > 1) outputAudioS = 1;
	if (outputAudioS < -1) outputAudioS = -1;
	return outputAudioS;
}

//Oscilloscope(meter) 
void OscData::makeLookupTable1(const int osc1Selection)
{
	const size_t meterNumberVal = 128;
	
	float(*funcPtr)(float) = std::sin;
	switch (osc1Selection)
	{
	case 1: funcPtr = saw; break;
	case 2: funcPtr = square; break;
	case 3: funcPtr = triangle; break;
	case 4: funcPtr = randomFloat; break;
	case 5: funcPtr = sawSmooth; break;
	case 6: funcPtr = reverseSawtooth; break;
	case 7: funcPtr = softSine; break;
	case 8: funcPtr = hardSine; break;
	}
	int obertonesDuplicate = obertoneVal;

	auto* table = new juce::dsp::LookupTableTransform<float>(
	[obertonesDuplicate, funcPtr](float x) {
		float sum = 0.f;
		for (int i = 1; i <= 1 + obertonesDuplicate; i++)
			sum += funcPtr(i * x);
		sum /= (1.f + obertonesDuplicate);
		return sum;
	}
		,-PIVal, PIVal, meterNumberVal);

	meter1.reset(table);
	OSC_1 = [table](float x) { return (*table) (x); };
	if (osc1Selection == 4) OSC_1 = randomFloat;
}

//Oscilloscope(meter) 
void OscData::makeLookupTable2(const int osc2Selection)
{
	const size_t meterNumberVal = 128;

	float(*funcPtr)(float) = std::sin; // the basic sine waveform; y = sine(x);

	switch (osc2Selection)
	{
	case 1: funcPtr = saw; break;
	case 2: funcPtr = square; break;
	case 3: funcPtr = triangle; break;
	case 4: funcPtr = randomFloat; break;
	case 5: funcPtr = sawSmooth; break;
	case 6: funcPtr = reverseSawtooth; break;
	case 7: funcPtr = softSine; break;
	case 8: funcPtr = hardSine; break;
	}

	auto* table = new juce::dsp::LookupTableTransform<float>(funcPtr, -PIVal, PIVal, meterNumberVal);

	meter2.reset(table);
	OSC_2 = [table](float x) { return (*table) (x); };
	if (osc2Selection == 4) OSC_2 = randomFloat;
}

