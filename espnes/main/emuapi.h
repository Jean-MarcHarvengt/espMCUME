#ifndef EMUAPI_H
#define EMUAPI_H

#define INVX        1
//#define INVY        1
//#define HAS_SND     1
//#define CUSTOM_SND  1
#define HAS_I2CKBD  1
#define USE_WIRE    1

// Title:     <                                        >
#define TITLE "               NES Emulator             "
#define ROMSDIR "nes"

#define emu_Init(ROM) {initPPU();loadROM(filename);execNES();}
#define emu_Step(x) {execframe(x);}
#define emu_Input(x) {}

#define VID_FRAME_SKIP       0x3
#define PALETTE_SIZE         64
#define SINGLELINE_RENDERING 1
#define TFT_VBUFFER_YCROP    0

#define ACTION_NONE          0
#define ACTION_MAXKBDVAL     12
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

#define KEYBOARD_X           104
#define KEYBOARD_Y           78
#define KEYBOARD_KEY_H       30
#define KEYBOARD_KEY_W       21
#define KEYBOARD_HIT_COLOR   RGBVAL16(0xff,0x00,0x00)

const unsigned short keysw[] = {
  TAREA_XY,KEYBOARD_X,KEYBOARD_Y,
  TAREA_WH,KEYBOARD_KEY_W,KEYBOARD_KEY_H,
  TAREA_NEW_ROW,40,40,
  TAREA_END};
   
const unsigned short keys[] = {
  2,3};  
   
#ifdef HAS_I2CKBD
const unsigned short i2ckeys[] = {
  0X0080,0X0008};
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
extern void * emu_LineBuffer(int line);

#endif






