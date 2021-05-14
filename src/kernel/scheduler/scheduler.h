/*
 * File:		scheduler.h
 * Description:	The main scheduler
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once


// Begins scheduling tasks to the processor calling this
// There is no going back once the scheduler has been entered
void scheduler_entry();