/*
 * File:		keyboard.h
 * Description:	Handles keyboard interrupts
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once


extern const char g_keyboard_scancodes[];


void keyboard_handler(struct InterruptStack* stack, size_t num);
