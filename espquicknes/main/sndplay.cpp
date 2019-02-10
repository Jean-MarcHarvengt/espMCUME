extern "C" {
#include "emuapi.h"
}


#include "esp_event.h"
#include "Nes_Emu.h"
#include "Data_Reader.h"
#include "abstract_file.h"
#include "Nes_Effects_Buffer.h"
#include "nes_ntsc.h"

static Nes_Emu *emu;


void snd_Init(void)
{
   emu = new Nes_Emu;
   register_optional_mappers();
   register_extra_mappers();

   //check_variables();

   //update_audio_mode(); //calls set_sample_rate and set_equalizer
   emu->set_palette_range(0); 
   //emu->set_pixels(video_buffer, videoBufferWidth);

   if (emu)
      emu->reset( false, false ); 
}

void snd_Step(void)
{
  vTaskDelay(20 / portTICK_PERIOD_MS);
  emu->emulate_frame(0,0);    
}

void snd_Start(char * filename)
{
  Mem_File_Reader reader(filename, 3525);
  emu->load_ines(reader);  
}


