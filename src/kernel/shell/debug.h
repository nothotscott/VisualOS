/*
 * File:		debug.h
 * Description: Used for debugging the kernel
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#define DEBUG_PORT	0x03f8	// COM 1


// Sends debug onto serial port
void debug_output(char* str);
void debug_output_warning(char* str);
void debug_output_error(char* str);