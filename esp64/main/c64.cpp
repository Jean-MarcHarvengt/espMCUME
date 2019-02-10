
extern "C" {
#include "emuapi.h"
}
#include "Teensy64.h"
#include <string.h>

#ifdef HAS_SND
#include "reSID.h"
AudioPlaySID playSID;
#endif

using namespace std;

/* IRAM_ATTR */
static void oneRasterLine(void) {
  static unsigned short lc = 1;

  while (true) {

    cpu.lineStartTime = get_ccount();
    cpu.lineCycles = cpu.lineCyclesAbs = 0;

    if (!cpu.exactTiming) {
    vic_do();
  } else {
    vic_do_simple();
  }

    if (--lc == 0) {
      lc = LINEFREQ / 10; // 10Hz
      cia1_checkRTCAlarm();
      cia2_checkRTCAlarm();
    }

    //Switch "ExactTiming" Mode off after a while:
    if (!cpu.exactTiming) break;
    if (get_ccount() - cpu.exactTimingStartTime >= EXACTTIMINGDURATION * (F_CPU / 1000)) {
    cpu_disableExactTiming();
      break;
    }
  };

}

const uint32_t ascii2scan[] = {
 //0 1 2 3 4 5 6 7 8 9 A B C D E F
   0,0,0,0,0,0,0,0,0,0,0,0,0,0x28,0,0, // return
 //     17:down                                                     29:right
   0x00,0x51,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x4f,0x00,0x00,
   //sp  !       "     #     $      %      &      '     (        )   *    +    ,    -    .    / 
   0x2c,0x201e,0x201f,0x2020,0x2021,0x2022,0x2023,0x2024,0x2025,0x2026,0x55,0x57,0x36,0x56,0x37,0x54,
   //0  1    2    3    4    5    6    7    8    9    :    ;    <      =    >      ?
   0x27,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x33,0x34,0x2036,0x32,0x2037,0x0238,
   //@    A    B    C    D    E    F    G    H    I    J    K    L    M    N    O
   47,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,
   //P  Q    R    S    T    U    V    W    X    Y    Z    [      \     ]     ^    _  
   0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x2026,0x31,0x2027,0x00,0x00,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // ' a b c d e f g h i j k l m n o
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x49,0, // p q r s t u v w x y z { | } ~ DEL
 //up left arr      133:f1   f2   f3   f4   f5   f6   f7   f8 
   75,78,0x00,0x00,0x00,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x00,0x00,0x00,  // 128-143
 //     145:up                                                      157:left
   0x00,0x2051,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x204f,0x00,0x00   // 144-159
};

// we also use USB matrix for the moment
static const uint8_t keymatrixmap[2][256] = {
  //Rows:
  // 0    1     2     3    4     5     6      7     8      9     A     B     C     D     E     F
  { 0x00, 0x00, 0x00, 0x00, 0x02, 0x08, 0x04, 0x04, 0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x20, //0x00
    0x10, 0x10, 0x10, 0x20, 0x80, 0x04, 0x02, 0x04, 0x08, 0x08, 0x02, 0x04, 0x08, 0x02, 0x80, 0x80, //0x10
    0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x01, 0x80, 0x01, 0x00, 0x80, 0x00, 0x00, 0x20, //0x20
    0x00, 0x00, 0x40, 0x20, 0x40, 0x00, 0x20, 0x20, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, //0x30
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x00, 0x00, 0x80, 0x01, //0x40
    0x00, 0x01, 0x00, 0x00, 0x40, 0x40, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x50
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x60
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x70
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x80
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x90
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xA0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xB0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xC0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xD0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xE0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x02
  }, //0xF0
  //Columns:
  // 0    1     2     3    4     5     6      7     8      9     A     B     C     D     E     F
  { 0x00, 0x00, 0x00, 0x00, 0x04, 0x10, 0x10, 0x04, 0x40, 0x20, 0x04, 0x20, 0x02, 0x04, 0x20, 0x04, //0x00
    0x10, 0x80, 0x40, 0x02, 0x40, 0x02, 0x20, 0x40, 0x40, 0x80, 0x02, 0x80, 0x02, 0x10, 0x01, 0x08, //0x10
    0x01, 0x08, 0x01, 0x08, 0x01, 0x08, 0x01, 0x08, 0x02, 0x80, 0x01, 0x00, 0x10, 0x00, 0x00, 0x40, //0x20
    0x00, 0x00, 0x20, 0x20, 0x04, 0x00, 0x80, 0x10, 0x00, 0x00, 0x10, 0x00, 0x20, 0x00, 0x40, 0x00, //0x30
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x40, 0x00, 0x00, 0x02, 0x04, //0x40
    0x00, 0x80, 0x00, 0x00, 0x80, 0x02, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x50
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x60
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x70
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x80
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x90
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xA0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xB0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xC0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xD0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xE0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x04, 0x10, 0x80
  }
}; //0xF0 

struct {
  union {
    uint32_t kv;
    struct {
      uint8_t ke,   //Extratasten SHIFT, STRG, ALT...
              kdummy,
              k,    //Erste gedrückte Taste
              k2;   //Zweite gedrückte Taste
    };
  };
  uint32_t lastkv;
  uint8_t shiftLock;
} kbdData = {0, 0, 0};


static bool setKey(uint32_t k) { 
	kbdData.kv = (k << 16);
	kbdData.ke = kbdData.k2;
	kbdData.k2 = 0;
	vTaskDelay(20 / portTICK_PERIOD_MS); 
	kbdData.kv = 0;  
	vTaskDelay(20 / portTICK_PERIOD_MS);   
	return true;
}

static void pushStringToTextEntry(char * text) {
    char c;
    while ((c = *text++)) {
        if (!setKey(ascii2scan[c])) {
            break;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


uint8_t cia1PORTA(void) {

  uint8_t v;

  v = ~cpu.cia1.R[0x02] | (cpu.cia1.R[0x00] & cpu.cia1.R[0x02]);
  int keys = emu_ReadKeys();
  if (!cpu.swapJoysticks) {
    if (keys & MASK_JOY2_BTN) v &= 0xEF;
    if (keys & MASK_JOY2_UP) v &= 0xFE;
    if (keys & MASK_JOY2_DOWN) v &= 0xFD;
    if (keys & MASK_JOY2_RIGHT) v &= 0xFB;
    if (keys & MASK_JOY2_LEFT) v &= 0xF7;
  } else {
    if (keys & MASK_JOY1_BTN) v &= 0xEF;
    if (keys & MASK_JOY1_UP) v &= 0xFE;
    if (keys & MASK_JOY1_DOWN) v &= 0xFD;
    if (keys & MASK_JOY1_RIGHT) v &= 0xFB;
    if (keys & MASK_JOY1_LEFT) v &= 0xF7;
  }	


  if (!kbdData.kv) return v; //Keine Taste gedrückt

  uint8_t filter = ~cpu.cia1.R[0x01] & cpu.cia1.R[0x03];
  
  if (kbdData.k) {
    if ( keymatrixmap[1][kbdData.k] & filter)  v &= ~keymatrixmap[0][kbdData.k];
  }

  if (kbdData.ke) {
    if (kbdData.ke & 0x02) { //Shift-links
      if ( keymatrixmap[1][0xff] & filter) v &= ~keymatrixmap[0][0xff];
    }
    if (kbdData.ke & 0x20) { //Shift-rechts
      if ( keymatrixmap[1][0xfe] & filter) v &= ~keymatrixmap[0][0xfe];
    }
    if (kbdData.ke & 0x11) { //Control
      if ( keymatrixmap[1][0xfd] & filter) v &= ~keymatrixmap[0][0xfd];
    }
    if (kbdData.ke & 0x88) { //Windows (=> Commodore)
      if ( keymatrixmap[1][0xfc] & filter) v &= ~keymatrixmap[0][0xfc];
    }
  }
 
  return v;

}


uint8_t cia1PORTB(void) {

  uint8_t v;

  v = ~cpu.cia1.R[0x03] | (cpu.cia1.R[0x00] & cpu.cia1.R[0x02]) ;

  int keys = emu_ReadKeys();
  if (!cpu.swapJoysticks) {
    if (keys & MASK_JOY1_BTN) v &= 0xEF;
    if (keys & MASK_JOY1_UP) v &= 0xFE;
    if (keys & MASK_JOY1_DOWN) v &= 0xFD;
    if (keys & MASK_JOY1_RIGHT) v &= 0xFB;
    if (keys & MASK_JOY1_LEFT) v &= 0xF7;
  } else {
    if (keys & MASK_JOY2_BTN) v &= 0xEF;
    if (keys & MASK_JOY2_UP) v &= 0xFE;
    if (keys & MASK_JOY2_DOWN) v &= 0xFD;
    if (keys & MASK_JOY2_RIGHT) v &= 0xFB;
    if (keys & MASK_JOY2_LEFT) v &= 0xF7;
  }

  if (!kbdData.kv) return v; //Keine Taste gedrückt

  uint8_t filter = ~cpu.cia1.R[0x00] & cpu.cia1.R[0x02];

  if (kbdData.k) {
    if ( keymatrixmap[0][kbdData.k] & filter) v &= ~keymatrixmap[1][kbdData.k];
  }

  if (kbdData.ke) {
    if (kbdData.ke & 0x02) { //Shift-links
      if ( keymatrixmap[0][0xff] & filter) v &= ~keymatrixmap[1][0xff];
    }
    if (kbdData.ke & 0x20) { //Shift-rechts
      if ( keymatrixmap[0][0xfe] & filter) v &= ~keymatrixmap[1][0xfe];
    }
    if (kbdData.ke & 0x11) { //Control
      if ( keymatrixmap[0][0xfd] & filter) v &= ~keymatrixmap[1][0xfd];
    }
    if (kbdData.ke & 0x88) { //Windows (=> Commodore)
      if ( keymatrixmap[0][0xfc] & filter) v &= ~keymatrixmap[1][0xfc];
    }
  }

  return v;
}


void c64_Init(void)
{
  disableEventResponder();
  resetPLA();
  resetCia1();
  resetCia2();
  resetVic();
  cpu_reset(); 
}


void c64_Step(void)
{
	oneRasterLine();
}

void c64_Start(char * filename)
{
}

static bool res=false;
void c64_Input(int bClick) {

  int hk = emu_ReadI2CKeyboard();
  if ( (hk != 0) && (res == false) ) {
    setKey(ascii2scan[hk]);
    res = true;
  } else if (hk == 0){
    res = false;
  }  

  if (bClick & MASK_KEY_USER1) {  
    pushStringToTextEntry("LOAD\"");
    pushStringToTextEntry("\"");
    setKey(ascii2scan[13]);
  	vTaskDelay(2000 / portTICK_PERIOD_MS);
    pushStringToTextEntry("RUN");
    setKey(ascii2scan[13]);  
  }
}

#ifdef HAS_SND      
void  SND_Process( void * stream, int len )
{
    playSID.update(stream, len);
}
#endif  
