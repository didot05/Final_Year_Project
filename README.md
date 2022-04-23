# Final_Year_Project


============== Final Year Project 2022 ============

A subtractive software synthesiser was built using the JUCE framework.

Implemented Features:
- two Oscillators
- three filters: LPF, BPF, HPF
- two ADSR: filter ADSR(left)/Amplifier ADSR(right)
- four Effectors: Reverb, Compressor, Chorus, Phase
- Virtual midiKeyboard
- Oscilloscope(Meter)
- Help button

=========================================

To execute this synthesiser, users need to download 
the JUCE Framework in preceded.

1. Go to the JUCE github site: (https://github.com/juce-framework/JUCE)
2. Clone the JUCE Repo to the specific location you want 
: Use git clone command. Following link will be helpful
; https://www.atlassian.com/git/tutorials/setting-up-a-repository/git-clone
3. Build/Open the Projucer file (JUCE’ project management tool)
: Use the appropriate solution in ”... /JUCE/extras/Projucer/Builds/ ...” 
4. Run the Projucer
5. Select the appropriate exporter & Build the solution
6. Run the standalone build or add the VST3 build to your VST3 folder to use it in your DAW

After compiling the ’Projucer’ file, click the ’File’ on the caption and open the ’synth.jucer’ file.
Then, just run/compile the ’synth.jucer’ file’s code. Finally, you can find the compiled .exe file in
the following direction: *\Final Year Project\Builds\VisualStudio2019\x64\Debug\Standalone
Plugin\synth.exe.

'synth.exe' (standalone version) is also included in this project folder, 
so you can use this directly to run the program. 
However, without installing JUCE Framework, the execute file does not work.

On the other hand, if you try to use the VST version of the synthesiser, 
you can use 'synth.vst' file included in this folder also. 
It is totally the same as a standalone version synthesiser 
except for the presence or absence of a virtual midiKeyboard.
Refer link for the VST plugin;
https://docs.juce.com/master/tutorial_create_projucer_basic_plugin.html
