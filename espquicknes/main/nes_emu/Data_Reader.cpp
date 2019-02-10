// File_Extractor 1.0.0. http://www.slack.net/~ant/

#include "Data_Reader.h"

#include "blargg_endian.h"
#include <stdio.h>
#include <errno.h>

/* Copyright (C) 2005-2009 Shay Green. This module is free software; you
can redistribute it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version. This
module is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details. You should have received a copy of the GNU Lesser General Public
License along with this module; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA */

#include "blargg_source.h"

// Data_Reader

const char * Data_Reader::read( void* p, int n )
{
	if ( n < 0 )
		return "Internal usage bug";
	
	if ( n <= 0 )
		return 0;
	
	if ( n > remain() )
		return "Truncated file";
	
	const char * err = read_v( p, n );
	if ( !err )
		remain_ -= n;
	
	return err;
}

const char * Data_Reader::read_avail( void* p, int* n_ )
{
	int n = min( (uint64_t)(*n_), remain() );
	*n_ = 0;
	
	if ( n < 0 )
		return "Internal usage bug";
	
	if ( n <= 0 )
		return 0;
	
	const char * err = read_v( p, n );
	if ( !err )
	{
		remain_ -= n;
		*n_ = n;
	}
	
	return err;
}

const char * Data_Reader::read_avail( void* p, long* n )
{
	int i = STATIC_CAST(int, *n);
	const char * err = read_avail( p, &i );
	*n = i;
	return err;
}

const char * Data_Reader::skip_v( int count )
{
	char buf [512];
	while ( count )
	{
		int n = min( count, (int) sizeof buf );
		count -= n;
		RETURN_ERR( read_v( buf, n ) );
	}
	return 0;
}

const char * Data_Reader::skip( int n )
{
	if ( n < 0 )
		return "Internal usage bug";
	
	if ( n <= 0 )
		return 0;
	
	if ( n > remain() )
		return "Truncated file";
	
	const char * err = skip_v( n );
	if ( !err )
		remain_ -= n;
	
	return err;
}


// File_Reader

const char * File_Reader::seek( uint64_t n )
{
	if ( n < 0 )
		return "Internal usage bug";
	
	if ( n == tell() )
		return 0;
	
	if ( n > size() )
		return "Truncated file";
	
	const char * err = seek_v( n );
	if ( !err )
		set_tell( n );
	
	return err;
}

const char * File_Reader::skip_v( uint64_t n )
{
	return seek_v( tell() + n );
}

// Mem_File_Reader

Mem_File_Reader::Mem_File_Reader( const void* p, long s ) :
	begin( STATIC_CAST(const char*, p) )
{
	set_size( s );
}

const char * Mem_File_Reader::read_v( void* p, int s )
{
	memcpy( p, begin + tell(), s );
	return 0;
}

const char * Mem_File_Reader::seek_v( int )
{
	return 0;
}
