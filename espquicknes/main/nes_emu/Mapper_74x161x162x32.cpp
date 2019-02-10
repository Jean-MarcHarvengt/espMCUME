/* Copyright notice for this file:
 * Copyright (C) 2018
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * This mapper was added by retrowertz for Libretro port of QuickNES.
 *
 * Mapper 180 Crazy Climber
 *
 */

#include "Nes_Mapper.h"

template < bool _is152 >
class Mapper_74x161x162x32 : public Nes_Mapper {
public:
	Mapper_74x161x162x32()
	{
		register_state( &bank, 1 );
	}

	virtual void reset_state()
	{
		if ( _is152 == 0 )
			bank = ~0;
	}

	virtual void apply_mapping()
	{
		if ( _is152 )
			write( 0, 0, bank );
		else
		{
			intercept_writes( 0x6000, 1 );
			write_intercepted( 0, 0x6000, bank );
		}
	}

	virtual bool write_intercepted( nes_time_t, nes_addr_t addr, int data )
	{
		if ( ( addr != 0x6000 ) || _is152 )
			return false;

		bank = data;
		set_prg_bank( 0x8000, bank_32k, ( bank >> 4 ) & 0x03 );
		set_chr_bank( 0x0000, bank_8k, ( ( bank >> 4 ) & 0x04 ) | ( bank & 0x03 ) );

		return true;
	}

	virtual void write( nes_time_t, nes_addr_t addr, int data )
	{
		if ( _is152 == 0) return;

		bank = handle_bus_conflict (addr, data );
		set_prg_bank( 0x8000, bank_16k, ( bank >> 4 ) & 0x07 );
		set_chr_bank( 0x0000, bank_8k, bank & 0x0F );
		mirror_single( ( bank >> 7) & 0x01 );
	}

	uint8_t bank;
};

void register_mapper_74x161x162x32();
void register_mapper_74x161x162x32()
{
	register_mapper< Mapper_74x161x162x32 <true> > ( 152 );
	register_mapper< Mapper_74x161x162x32 <false> > ( 86 );
}
