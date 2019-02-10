
// Sunsoft FME-7 sound emulator

// Nes_Emu 0.7.0

#ifndef NES_FME7_APU_H
#define NES_FME7_APU_H

#include <stdint.h>
#include "blargg_common.h"
#include "Blip_Buffer.h"

struct fme7_apu_state_t
{
	enum { reg_count = 14 };
	uint8_t regs [reg_count];
	uint8_t phases [3]; // 0 or 1
	uint8_t latch;
	uint16_t delays [3]; // a, b, c
};
BOOST_STATIC_ASSERT( sizeof (fme7_apu_state_t) == 24 );

class Nes_Fme7_Apu : private fme7_apu_state_t {
public:
	Nes_Fme7_Apu();
	
	// See Nes_Apu.h for reference
	void reset();
	void volume( double );
	void treble_eq( blip_eq_t const& );
	void output( Blip_Buffer* );
	enum { osc_count = 3 };
	void osc_output( int index, Blip_Buffer* );
	void end_frame( blip_time_t );
	void save_state( fme7_apu_state_t* ) const;
	void load_state( fme7_apu_state_t const& );
	
	// Mask and addresses of registers
	enum { addr_mask = 0xe000 };
	enum { data_addr = 0xe000 };
	enum { latch_addr = 0xc000 };
	
	// (addr & addr_mask) == latch_addr
	void write_latch( int );
	
	// (addr & addr_mask) == data_addr
	void write_data( blip_time_t, int data );
	
	// End of public interface
private:
	// noncopyable
	Nes_Fme7_Apu( const Nes_Fme7_Apu& );
	Nes_Fme7_Apu& operator = ( const Nes_Fme7_Apu& );
	
	static unsigned char amp_table [16];
	
	struct {
		Blip_Buffer* output;
		int last_amp;
	} oscs [osc_count];
	blip_time_t last_time;
	
	enum { amp_range = 192 }; // can be any value; this gives best error/quality tradeoff
	Blip_Synth<blip_good_quality,1> synth;
	
	void run_until( blip_time_t );
};

inline void Nes_Fme7_Apu::volume( double v )
{
	synth.volume( 0.38 / amp_range * v ); // to do: fine-tune
}

inline void Nes_Fme7_Apu::treble_eq( blip_eq_t const& eq )
{
	synth.treble_eq( eq );
}

inline void Nes_Fme7_Apu::osc_output( int i, Blip_Buffer* buf )
{
	oscs [i].output = buf;
}

inline void Nes_Fme7_Apu::output( Blip_Buffer* buf )
{
	for ( int i = 0; i < osc_count; i++ )
		osc_output( i, buf );
}

inline Nes_Fme7_Apu::Nes_Fme7_Apu()
{
	output( NULL );
	volume( 1.0 );
	reset();
}

inline void Nes_Fme7_Apu::write_latch( int data ) { latch = data; }

inline void Nes_Fme7_Apu::write_data( blip_time_t time, int data )
{
	if ( (unsigned) latch >= reg_count )
	{
		#ifdef dprintf
			dprintf( "FME7 write to %02X (past end of sound registers)\n", (int) latch );
		#endif
		return;
	}
	
	run_until( time );
	regs [latch] = data;
}

inline void Nes_Fme7_Apu::end_frame( blip_time_t time )
{
	if ( time > last_time )
		run_until( time );
	
	last_time -= time;
}

inline void Nes_Fme7_Apu::save_state( fme7_apu_state_t* out ) const
{
	*out = *this;
}

inline void Nes_Fme7_Apu::load_state( fme7_apu_state_t const& in )
{
	reset();
	fme7_apu_state_t* state = this;
	*state = in;

	//Run sound channels for 0 cycles for clean audio after loading state
	run_until(last_time);
}

#endif
