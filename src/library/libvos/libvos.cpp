/*
 * File:		libvos.cpp
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "libvos.h"

using namespace libvos;

/* Operator implementations */
void* operator new(size_t size) {
	return malloc(size);
}
void* operator new[](size_t size) {
	return malloc(size);
}
void operator delete(void* p, size_t) {
	free(p);
}
void operator delete[](void* p, size_t) {
	free(p);
}