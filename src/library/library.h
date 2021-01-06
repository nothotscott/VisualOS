/*
 * File:		library.h
 * Description:	Auto-included for library cpp files
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#define	NULL	0


// Puts character [chr] in [buffer] at [position]
#define __PUTC(chr, buffer, position)								\
									{								\
										if(buffer != nullptr) {		\
											buffer[position] = chr;	\
										}							\
										position++;					\
									}
