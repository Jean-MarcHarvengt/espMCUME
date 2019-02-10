#ifndef EMUAPI_H
#define EMUAPI_H

#define INVX        1
//#define INVY        1
#define HAS_SND     1
#define CUSTOM_SND  1
#define HAS_I2CKBD  1
#define USE_WIRE    1

// Title:     <                                        >
#define TITLE "          Atari 800 Emulator            "
#define ROMSDIR "800"

#define emu_Init(ROM) {at8_Init(); at8_Start(ROM);}
#define emu_Step(x) {at8_Step();}
#define emu_Input(x) {}

#define VID_FRAME_SKIP       0x1
#define PALETTE_SIZE         256
#define SINGLELINE_RENDERING 1
#define TFT_VBUFFER_YCROP    0

#define R32(rgb) ((rgb>>16)&0xff) 
#define G32(rgb) ((rgb>>8)&0xff) 
#define B32(rgb) (rgb & 0xff) 
#define R16(rgb) ((rgb>>8)&0xf8) 
#define G16(rgb) ((rgb>>3)&0xfc) 
#define B16(rgb) ((rgb<<3)&0xf8) 

#define ACTION_NONE          0
#define ACTION_MAXKBDVAL     225
#define ACTION_EXITKBD       128
#define ACTION_RUNTFT        129

#ifdef KEYMAP_PRESENT

#define TAREA_W_DEF          32
#define TAREA_H_DEF          32
#define TAREA_END            255
#define TAREA_NEW_ROW        254
#define TAREA_NEW_COL        253
#define TAREA_XY             252
#define TAREA_WH             251

#define KEYBOARD_X           10
#define KEYBOARD_Y           8
#define KEYBOARD_KEY_H       40
#define KEYBOARD_KEY_W       30
#define KEYBOARD_HIT_COLOR   RGBVAL16(0xff,0x00,0x00)

const unsigned short keysw[] = {
  TAREA_XY,KEYBOARD_X,KEYBOARD_Y,
  TAREA_WH,KEYBOARD_KEY_W,KEYBOARD_KEY_H,
  TAREA_NEW_ROW,21,21,22,21,22,21,21,22,21,22,21,21,22,21,22,
  TAREA_NEW_ROW,34,21,21,21,21,21,21,21,21,21,21,21,21,34,
  TAREA_NEW_ROW,40,21,21,21,21,21,21,21,21,21,21,21,21,21,
  TAREA_NEW_ROW,50,21,21,21,21,21,21,21,21,21,21,21,45,
  TAREA_NEW_ROW,60,192,
  TAREA_END};

   
const unsigned short keys[] = {
  0x1C+1,0x1F+1,0x1F  ,0x1A+1,0x18+1,0x1D+1,0x1B+1,0x33+1,0x35+1,0x30+1,0x32+1,0x36+1,0x37+1,0,0,
  0x2C+1,0x2F+1,0x2F  ,0x2A+1,0x28+1,0x2D+1,0x2B+1,0x0B+1,0x0D+1,0x08+1,0x0A+1,0x0E,0x0F+1,0x0C+1,
  0,     0x3F+1,0x3F  ,0x3A+1,0x38+1,0x3D+1,0x39+1,0x01+1,0x05+1,0x00+1,0x02+1,0x06+1,0x07+1,0,
  0x3C+1,0x17+1,0x16+1,0x12+1,0x11+1,0x15+1,0x23+1,0x25+1,0x20+1,0x22+1,0x26+1,  0,0,
  0,     0x21+1
  };  

#ifdef HAS_I2CKBD
const unsigned short i2ckeys[] = {
  0, 0X0080,0X0008,0X0180,0X0108,0X0280,0X0208,0X0380,0X0308,0X0480,0X0408,0,0,0,0,
  0, 0X0040,0X0004,0X0140,0X0104,0X0240,0X0204,0X0340,0X0304,0X0440,0X0404,0,0,0X0402,
  0, 0X0020,0X0002,0X0120,0X0102,0X0220,0X0202,0X0320,0X0302,0X0420,0,0,0,0,
  0X0010,0X0001,0X0110,0X0101,0X0210,0X0201,0X0310,0X0301,0X0410,0,0,0,0,
  0, 0X0401};
#endif
   
#endif

#define MASK_JOY2_RIGHT 0x0001
#define MASK_JOY2_LEFT  0x0002
#define MASK_JOY2_UP    0x0004
#define MASK_JOY2_DOWN  0x0008
#define MASK_JOY2_BTN   0x0010
#define MASK_KEY_USER1  0x0020
#define MASK_KEY_USER2  0x0040
#define MASK_KEY_USER3  0x0080
#define MASK_JOY1_RIGHT 0x0100
#define MASK_JOY1_LEFT  0x0200
#define MASK_JOY1_UP    0x0400
#define MASK_JOY1_DOWN  0x0800
#define MASK_JOY1_BTN   0x1000


extern void emu_init(void);
extern void emu_printf(char * text);
extern void emu_printi(int val);
extern void * emu_Malloc(int size);
extern void emu_Free(void * pt);

extern int emu_FileOpen(char * filename);
extern int emu_FileRead(char * buf, int size);
extern unsigned char emu_FileGetc(void);
extern int emu_FileSeek(int seek);
extern void emu_FileClose(void);
extern int emu_FileSize(char * filename);
extern int emu_LoadFile(char * filename, char * buf, int size);
extern int emu_LoadFileSeek(char * filename, char * buf, int size, int seek);

extern void emu_InitJoysticks(void);
extern int emu_SwapJoysticks(int statusOnly);
extern unsigned short emu_DebounceLocalKeys(void);
extern int emu_ReadKeys(void);
extern int emu_GetPad(void);
extern int emu_ReadAnalogJoyX(int min, int max);
extern int emu_ReadAnalogJoyY(int min, int max);
extern int emu_ReadI2CKeyboard(void);
extern int emu_setKeymap(int index);

extern void emu_sndInit();
extern void emu_sndPlaySound(int chan, int volume, int freq);
extern void emu_sndPlayBuzz(int size, int val);

extern void emu_SetPaletteEntry(unsigned char r, unsigned char g, unsigned char b, int index);
extern void emu_DrawScreen(unsigned char * VBuf, int width, int height, int stride);
extern void emu_DrawLine(unsigned char * VBuf, int width, int height, int line);
extern void emu_DrawVsync(void);
extern int emu_FrameSkip(void);


#endif






