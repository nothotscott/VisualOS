/*
 * File:		types.h
 * Description:	Used throughout the OS build process exclusivley for typedefs in C and C++
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

typedef	unsigned char		byte_t;
typedef	unsigned short		ushort_t;
typedef	unsigned int		uint_t;
typedef	unsigned long long	ulong_t;
typedef unsigned long		size_t;
typedef unsigned long		uintptr_t;

typedef signed long long	slong_t;

#ifndef __cplusplus
typedef enum {false, true}	bool;
#endif


// Rounds up [n] to the nearest [multiple]
#define ROUND_UP(n, multiple)	((n) % (multiple) == 0 ? (n) : (n) - (n) % (multiple) + (multiple))

// Returns [n]'s [bit] set to [value]
#define SET_BIT(n, bit, value)	((n) ^ (-(value) ^ (n)) & 1 << (bit))

// Returns [n]'s value of [bit]
#define GET_BIT(n, bit)			((1 << (bit) & (n)) > 0)


/* From cstdlib */

// stdarg.h
typedef __builtin_va_list	va_list;
#define va_start(v,l)		__builtin_va_start(v,l)
#define va_end(v)			__builtin_va_end(v)
#define va_arg(v,l)			__builtin_va_arg(v,l)
#define va_copy(d,s)		__builtin_va_copy(d,s)