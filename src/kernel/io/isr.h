/*
 * File:		isr.h
 * Description:	Interrupt service routines
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once


extern void isr_nothing();

extern void isr1();

void isr1_handler();