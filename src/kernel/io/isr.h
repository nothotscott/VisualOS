/*
 * File:		isr.h
 * Description:	Interrupt service routines
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#define	IO_ISR_MAX	256

extern void (*g_isr_handlers[IO_ISR_MAX])(ulong_t);

extern void isr33();

// Adds isr [handler] for interrupt [num]
void isr_register_handler(size_t num, void (*handler)(ulong_t));

void isr33_handler();