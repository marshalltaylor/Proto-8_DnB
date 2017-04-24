//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  March 21, 2015
//
//**********************************************************************//
//#include <Arduino.h>
//#include <Audio.h>
//#include <Wire.h>
//#include <SPI.h>
//#include <SD.h>

#include "proto-8Hardware.h"
#include "P8Interface.h"

//**Timers and stuff**************************//
#include "timerModule32.h"
#include "timeKeeper.h"

//Globals
uint32_t maxTimer = 60000000;
uint32_t maxInterval = 2000000;

IntervalTimer myTimer; // For interrupt

TimerClass32 panelUpdateTimer(30000);


TimerClass32 LEDsTimer(20);
TimerClass32 switchesTimer(500);
TimerClass32 knobsTimer(500);

TimerClass32 debugTimer(1000000);

//**P8Panel.h defines the Proto-8 hardware****//
#include "P8Panel.h"

//tick variable for interrupt driven timer1
uint32_t usTicks = 0;
uint8_t usTicksMutex = 1; //start locked out

//**Panel State Machine***********************//
#include "P8Interface.h"

P8Interface p8hid;

volatile int32_t pUTStartTime = 0;
volatile int32_t pUTLastTime = 0;
volatile int32_t pUTStopTime = 0;
volatile int32_t pUTLength = 0;

//Names use in P8PanelComponents.cpp and .h
LEDShiftRegister LEDs;
AnalogMuxTree knobs;
SwitchMatrix switches;

// -----------------------------------------------------------------------------
void setup() 
{
	//Initialize serial:
	Serial.begin(115200);
	//delay(2000);
	Serial.println("Program Started");
	
	LEDs.begin();
	knobs.begin();
	switches.begin();
	
	// initialize IntervalTimer
	myTimer.begin(serviceUS, 1);  // serviceMS to run every 0.001 seconds
	
	//Reset the panel
	p8hid.reset();
	
	
}

void loop()
{
	usTicksMutex = 1;

	panelUpdateTimer.update(usTicks);

	switchesTimer.update(usTicks);
	knobsTimer.update(usTicks);
	LEDsTimer.update(usTicks);

	debugTimer.update(usTicks);
	
	usTicksMutex = 0;

	//**Switches timer****************************//  
	if(switchesTimer.flagStatus() == PENDING)
	{
		switches.tick();
	
	}
	//**Knobs timer****************************//  
	if(knobsTimer.flagStatus() == PENDING)
	{
		knobs.tick();
	
	}		
	//**LEDs timer****************************//  
	if(LEDsTimer.flagStatus() == PENDING)
	{
		LEDs.tick();
	
	}
	//**Process the panel and state machine***********//  
	if(panelUpdateTimer.flagStatus() == PENDING)
	{
		pUTLastTime = pUTStartTime;
		pUTStartTime = usTicks;
		//Provide inputs

		//Tick the machine
		p8hid.processMachine(30);
		
		//Deal with potential outputs here
		
		pUTStopTime = usTicks;
		int32_t length = pUTStopTime - pUTStartTime;
		if(length > pUTLength )
		{
			pUTLength = length;
		}
	}
	//**Debug timer*******************************//  
	if(debugTimer.flagStatus() == PENDING)
	{
		Serial.println("--Processor--");
		Serial.print("millis: ");
		Serial.println( millis() );
		Serial.print("usTicks: ");
		Serial.println( usTicks );
//		Serial.print("AudioUsage:");
//		//Serial.print(AudioProcessorUsage());
//		//Serial.print(",");
//		Serial.print((float)AudioProcessorUsageMax(), 2);
//		AudioProcessorUsageMaxReset();
//		Serial.print("    ");
//		Serial.print("MemoryUsage: ");
//		//Serial.print(AudioMemoryUsage());
//		//Serial.print(",");
//		Serial.print(AudioMemoryUsageMax());
//		AudioMemoryUsageMaxReset();//reset
//		Serial.print(",   FreeRam: ");
		Serial.print("FreeRam: ");
		Serial.print(FreeRam());
		Serial.println();
	
		Serial.println("--Process Measurements--");
		//Serial.print("panelUpdateTimer (sTime, length): ");
		//Serial.print(pUTStartTime - pUTLastTime);
		//Serial.print(", ");
		//Serial.println(pUTStopTime - pUTStartTime);
		Serial.print("pUTLength: ");
		Serial.println(pUTLength);
		pUTLength = 0;

		Serial.println();
	}
	//midiA.read();

	
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

void serviceUS(void)
{
  uint32_t returnVar = 0;
  if(usTicks >= ( maxTimer + maxInterval ))
  {
	 if( usTicksMutex == 0 )
	 {
		 returnVar = usTicks - maxTimer;
	 }
	 else
	 {
		 returnVar = usTicks + 1;
	 }

  }
  else
  {
    returnVar = usTicks + 1;
  }
  usTicks = returnVar;
}
