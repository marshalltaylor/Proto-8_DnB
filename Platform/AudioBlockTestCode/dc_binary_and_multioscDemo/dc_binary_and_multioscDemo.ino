#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "synth_dc_binary.h"
#include "synth_multiosc.h"
#include "wavegen.h"


// Don't touch includes when using web gui

// GUItool: begin automatically generated code
AudioSynthWaveformDcBinary dc_amplitude;     //xy=269,1047
AudioSynthWaveformDcBinary dc_cent;     //xy=269,1157
AudioSynthWaveformDcBinary dc_3_12;     //xy=270,1104
AudioSynthWaveformDcBinary dc_out_volume;     //xy=270,1231
AudioSynthMultiOsc       multiosc1;      //xy=460,1127
AudioEffectMultiply      multiply1;      //xy=614,1250
AudioOutputI2S           i2s1;           //xy=768,1207
AudioConnection          patchCord1(dc_amplitude, 0, multiosc1, 0);
AudioConnection          patchCord2(dc_cent, 0, multiosc1, 5);
AudioConnection          patchCord3(dc_cent, 0, multiosc1, 6);
AudioConnection          patchCord4(dc_cent, 0, multiosc1, 7);
AudioConnection          patchCord5(dc_cent, 0, multiosc1, 8);
AudioConnection          patchCord6(dc_3_12, 0, multiosc1, 1);
AudioConnection          patchCord7(dc_3_12, 0, multiosc1, 2);
AudioConnection          patchCord8(dc_3_12, 0, multiosc1, 3);
AudioConnection          patchCord9(dc_3_12, 0, multiosc1, 4);
AudioConnection          patchCord10(dc_out_volume, 0, multiply1, 1);
AudioConnection          patchCord11(multiosc1, 0, multiply1, 0);
AudioConnection          patchCord12(multiply1, 0, i2s1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=757,1148
// GUItool: end automatically generated code



//Set knob names to analog input names here (use for remapping)
// Knobs go:
//  1   2   3
//    4   5
#define KNOB3 A3 //
#define KNOB2 A2 //
#define KNOB1 A1 //Main Parameter
#define KNOB4 A6 //
#define KNOB5 A7 //

uint16_t lastKnob1Value = 0;
uint16_t lastKnob2Value = 0;
uint16_t lastKnob3Value = 0;
uint16_t lastKnob4Value = 0;
uint16_t lastKnob5Value = 0;

uint16_t debugCounter = 0;

//Used to prevent free run looping
uint32_t next;

void setup() {
	//**** General Setup ****//
	Serial.begin(115200);
	delay(500);
	Serial.println("Sketch has started");
	delay(1500);

	//**** Timer Setup ****//
	next = millis() + 2000;

	//**** Audio Setup ****//
	AudioNoInterrupts();
    
	AudioMemory(100);
    
	sgtl5000_1.enable();
	sgtl5000_1.volume(1); //Force check first volume
	sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
	sgtl5000_1.unmuteHeadphone();
	sgtl5000_1.lineInLevel(1); //use 2 for 1:1
	sgtl5000_1.lineOutLevel(13);
    
	//Configure initial system here
	dc_amplitude.amplitude_int(0);
	dc_cent.amplitude_int(0x7FFF);
	//dc_3_12.amplitude_int(0);
	dc_3_12.amplitude_3_12(note2bpo[24]);
	dc_out_volume.amplitude_int(0);

	//**** Gen Waveforms ****//
	// NOTICE: osc should be started in the following order
	//1. Use .begin()
	//2. Write wave data to multiosc's pointer
	multiosc1.staticAmp[0] = 255;
	multiosc1.staticAmp[1] = 255;
	multiosc1.staticAmp[2] = 255;
	multiosc1.staticAmp[3] = 255;

	multiosc1.begin();
	WaveGenerator testWave;
	testWave.setParameters( 255, 0, 255, 0, 45 );			
	//testWave.writeWaveU16_257( multiosc1.getPointer( 0 ) );

	AudioInterrupts();
	
}

void loop() {
	//Do this at the interval defined above, don't just free run the loop
	if (millis() >= next)
	{
		next = millis() + 15;

		//Get the knob values
		uint16_t newKnob1Value = analogRead(KNOB1);
		uint16_t newKnob2Value = analogRead(KNOB2);
		uint16_t newKnob3Value = analogRead(KNOB3);
		uint16_t newKnob4Value = analogRead(KNOB4);
		uint16_t newKnob5Value = analogRead(KNOB5);

		//Set audio platform parameters based on those values
		if( (lastKnob1Value >> 2) != (newKnob1Value >> 2) )
		{
			//Serial.println("New Knob Input");
			//Take action
			lastKnob1Value = newKnob1Value;
			dc_amplitude.amplitude_int((int16_t)newKnob1Value<<5);
		}
		if( (lastKnob3Value >> 2) != (newKnob3Value >> 2) )
		{
			//Take action
			lastKnob3Value = newKnob3Value;
			dc_out_volume.amplitude_int((int16_t)lastKnob3Value<<5);
		}
		if( (lastKnob4Value ) != (newKnob4Value ) )
		{
			//Serial.println("New Knob Input");
			//Take action
			lastKnob4Value = newKnob4Value;
			if( newKnob4Value < 512 )
			{
				float tempbpo = note2bpo[(uint32_t)newKnob4Value * 127 / 512];
				//Serial.println((uint32_t)newKnob4Value * 127 / 512);
				dc_3_12.amplitude_3_12(tempbpo); //add for dc tune
			}
			else
			{
				dc_3_12.amplitude_int((int16_t)(newKnob4Value - 512) << 6);
			}
		}
		if( (lastKnob5Value) != (newKnob5Value) )
		{
			//Serial.println("New Knob Input");
			//Take action
			lastKnob5Value = newKnob5Value;
			dc_cent.amplitude_int((int16_t)newKnob5Value<<5);
		}
		
		//When enough regular 15ms loops have occured, send out debug data to the serial
		if( debugCounter > 50 )
		{
			debugCounter = 0;
			Serial.println((int32_t)(multiosc1.debugSave >> 32), HEX);
			Serial.println((int32_t)multiosc1.debugSave, HEX);
			multiosc1.debugFlag = 1;
			Serial.println("Knob values: ");
			Serial.print("1: ");
			Serial.println(newKnob1Value);
			Serial.print("2: ");
			Serial.println(newKnob2Value);
			Serial.print("3: ");
			Serial.println(newKnob3Value);
			Serial.print("4: ");
			Serial.println(newKnob4Value);
			Serial.print("5: ");
			Serial.println(newKnob5Value);
			
			Serial.print("Processor: ");
			Serial.print((float)AudioProcessorUsageMax(), 2);
			AudioProcessorUsageMaxReset();
			Serial.print("    Memory: ");
			Serial.print(AudioMemoryUsageMax());
			AudioMemoryUsageMaxReset();//reset
			Serial.print("    FreeRam: ");
			Serial.print(FreeRam());
			
			
			Serial.println();
		}
		debugCounter++;
	}
}

uint32_t FreeRam(){ // for Teensy 3.0
	uint32_t stackTop;
	uint32_t heapTop;

	// current position of the stack.
	stackTop = (uint32_t) &stackTop;

	// current position of heap.
	void* hTop = malloc(1);
	heapTop = (uint32_t) hTop;
	free(hTop);

	// The difference is the free, available ram.
	return stackTop - heapTop;
}