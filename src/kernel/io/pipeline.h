/*
 * File:		pipeline.h
 * Description:	Handles excpeptions and passes signals down the global pipeline
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#include <queue.h>

// TODO the actual pipeline
extern struct Queue* g_pipeline;


// ISR 8
void double_fault_handler(struct InterruptStack* stack, size_t num);

// ISR 13
void general_protection_fault_handler(struct InterruptStack* stack, size_t num);

// ISR 14
void paging_fault_handler(struct InterruptStack* stack, size_t num);