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
 */

#include "Nes_Mapper.h"
 
class Mapper_241 : public Nes_Mapper {
public:
	Mapper_241()
	{
		register_state( &bank, 1 );
	}

	virtual void reset_state()
	{ }

	virtual void apply_mapping()
	{
		enable_sram();
		write( 0, 0, bank );
	}

	virtual void write( nes_time_t, nes_addr_t, int data )
	{
		bank = data;
		set_prg_bank( 0x8000, bank_32k, bank );
	}

	uint8_t bank;
};

void register_mapper_241();
void register_mapper_241()
{
	register_mapper< Mapper_241 > ( 241 );
}
