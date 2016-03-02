#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "SerialFlash.h"
#include "effect_bitcrusher.h"

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1A;     //xy=240,7
AudioSynthWaveform       waveform1B;     //xy=240,42
AudioSynthWaveform       waveform1C;     //xy=240,77
AudioSynthWaveform       waveform1D;     //xy=240,112
AudioSynthWaveform       waveform2A;     //xy=248,157
AudioSynthWaveform       waveform2B;     //xy=248,192
AudioSynthWaveform       waveform2C;     //xy=248,227
AudioSynthWaveform       waveform2D;     //xy=248,262
AudioSynthWaveform       waveform3A;     //xy=256,307
AudioSynthWaveform       waveform3B;     //xy=256,342
AudioSynthWaveform       waveform3C;     //xy=256,377
AudioSynthWaveform       waveform3D;     //xy=256,412
AudioSynthWaveform       waveform4A;     //xy=264,457
AudioSynthWaveform       waveform4B;     //xy=264,492
AudioSynthWaveform       waveform4C;     //xy=264,527
AudioSynthWaveform       waveform4D;     //xy=264,562
AudioMixer4              mixer1;         //xy=430,58
AudioMixer4              mixer2;         //xy=438,208
AudioMixer4              mixer3;         //xy=446,358
AudioMixer4              mixer4;         //xy=454,508
AudioEffectBitcrusher    bitcrusher1;    //xy=584,58
AudioEffectBitcrusher    bitcrusher2;    //xy=592,208
AudioEffectBitcrusher    bitcrusher3;    //xy=600,358
AudioEffectBitcrusher    bitcrusher4;    //xy=608,508
AudioMixer4              mixer5;         //xy=994,220
AudioFilterStateVariable filter1;        //xy=1150,228
AudioFilterStateVariable filter3;        //xy=1286,229
AudioOutputI2SQuad       i2s_quad2;      //xy=1476,238
AudioConnection          patchCord1(waveform3D, 0, mixer3, 3);
AudioConnection          patchCord2(waveform2C, 0, mixer2, 2);
AudioConnection          patchCord3(waveform2D, 0, mixer2, 3);
AudioConnection          patchCord4(waveform2B, 0, mixer2, 1);
AudioConnection          patchCord5(waveform4D, 0, mixer4, 3);
AudioConnection          patchCord6(waveform3A, 0, mixer3, 0);
AudioConnection          patchCord7(waveform3B, 0, mixer3, 1);
AudioConnection          patchCord8(waveform3C, 0, mixer3, 2);
AudioConnection          patchCord9(waveform1A, 0, mixer1, 0);
AudioConnection          patchCord10(waveform4B, 0, mixer4, 1);
AudioConnection          patchCord11(waveform2A, 0, mixer2, 0);
AudioConnection          patchCord12(waveform4A, 0, mixer4, 0);
AudioConnection          patchCord13(waveform1B, 0, mixer1, 1);
AudioConnection          patchCord14(waveform4C, 0, mixer4, 2);
AudioConnection          patchCord15(waveform1C, 0, mixer1, 2);
AudioConnection          patchCord16(waveform1D, 0, mixer1, 3);
AudioConnection          patchCord17(mixer1, bitcrusher1);
AudioConnection          patchCord18(mixer2, bitcrusher2);
AudioConnection          patchCord19(mixer4, bitcrusher4);
AudioConnection          patchCord20(mixer3, bitcrusher3);
AudioConnection          patchCord21(bitcrusher1, 0, mixer5, 0);
AudioConnection          patchCord22(bitcrusher3, 0, mixer5, 2);
AudioConnection          patchCord23(bitcrusher2, 0, mixer5, 1);
AudioConnection          patchCord24(bitcrusher4, 0, mixer5, 3);
AudioConnection          patchCord25(mixer5, 0, filter1, 0);
AudioConnection          patchCord26(filter1, 0, filter3, 0);
AudioConnection          patchCord27(filter3, 0, i2s_quad2, 0);
AudioConnection          patchCord28(filter3, 0, i2s_quad2, 1);
AudioConnection          patchCord29(filter3, 0, i2s_quad2, 2);
AudioConnection          patchCord30(filter3, 0, i2s_quad2, 3);
AudioControlSGTL5000     sgtl5000_2;     //xy=1430.0,93.0
AudioControlSGTL5000     sgtl5000_1;     //xy=1434.0,49.0
// GUItool: end automatically generated code
