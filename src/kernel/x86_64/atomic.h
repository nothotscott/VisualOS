/*
 * File:		atomic.h
 * Description:	Atomic library for common scenarios
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */


// Locks/unlocks [mutex] (typically flags of a struct) at bit position [bit]
// Returns true on success
bool atomic_lock(void* mutex, uint64_t bit);
bool atomic_unlock(void* mutex, uint64_t bit);