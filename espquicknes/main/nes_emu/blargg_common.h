// Sets up common environment for Shay Green's libraries.
// To change configuration options, modify blargg_config.h, not this file.

// File_Extractor 1.0.0
#ifndef BLARGG_COMMON_H
#define BLARGG_COMMON_H

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* BLARGG_STATIC_ASSERT( expr ): Generates compile error if expr is 0.
Can be used at file, function, or class scope. */
#ifdef _MSC_VER
	// MSVC6 (_MSC_VER < 1300) __LINE__ fails when /Zl is specified
	#define BLARGG_STATIC_ASSERT( expr )    \
		void blargg_failed_( int (*arg) [2 / (int) !!(expr) - 1] )
#else
	// Others fail when declaring same function multiple times in class,
	// so differentiate them by line
	#define BLARGG_STATIC_ASSERT( expr )    \
		void blargg_failed_( int (*arg) [2 / !!(expr) - 1] [__LINE__] )
#endif

/* Pure virtual functions cause a vtable entry to a "called pure virtual"
error handler, requiring linkage to the C++ runtime library. This macro is
used in place of the "= 0", and simply expands to its argument. During
development, it expands to "= 0", allowing detection of missing overrides. */
#define BLARGG_PURE( def ) def

/* My code depends on ASCII anywhere a character or string constant is
compared with data read from a file, and anywhere file data is read and
treated as a string. */
#if '\n'!=0x0A || ' '!=0x20 || '0'!=0x30 || 'A'!=0x41 || 'a'!=0x61
	#error "ASCII character set required"
#endif

/* My code depends on int being at least 32 bits. Almost everything these days
uses at least 32-bit ints, so it's hard to even find a system with 16-bit ints
to test with. The issue can't be gotten around by using a suitable blargg_int
everywhere either, because int is often converted to implicitly when doing
arithmetic on smaller types. */
#if UINT_MAX < 0xFFFFFFFF
	#error "int must be at least 32 bits"
#endif

// In case compiler doesn't support these properly. Used rarely.
#define STATIC_CAST(T,expr) static_cast<T> (expr)
#define CONST_CAST( T,expr) const_cast<T> (expr)

// User configuration can override the above macros if necessary
#include "blargg_config.h"

// BLARGG_NEW is used in place of new in library code
#include <new>
#ifndef BLARGG_NEW
#define BLARGG_NEW new (std::nothrow)
#endif

// BOOST_STATIC_ASSERT( expr ): Generates compile error if expr is 0.
#ifndef BOOST_STATIC_ASSERT
#ifdef _MSC_VER
// MSVC6 (_MSC_VER < 1300) fails for use of __LINE__ when /Zl is specified
#define BOOST_STATIC_ASSERT( expr ) \
	void blargg_failed_( int (*arg) [2 / (int)!!(expr) - 1] )
#else
// Some other compilers fail when declaring same function multiple times in class,
// so differentiate them by line
#define BOOST_STATIC_ASSERT( expr ) \
	void blargg_failed_( int (*arg) [2 / !!(expr) - 1] [__LINE__] )
#endif
#endif

typedef struct
#ifdef NO_UNALIGNED_ACCESS
__attribute ((packed))
#endif
{
	uint32_t val;
}unaligned_uint32_t;


#endif
