/*
 * File:		types.h
 * Description:	Used throughout the OS build process exclusivley for typedefs in C and C++
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

typedef	unsigned char		byte;
typedef	unsigned short		ushort;
typedef	unsigned int		uint;
typedef	unsigned long long	ulong;
typedef unsigned long		size_t;
typedef unsigned long		uintptr_t;

typedef signed long long	slong;

#ifndef __cplusplus
typedef enum {false, true}	bool;
#endif