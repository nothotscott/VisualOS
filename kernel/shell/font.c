/*
 * File:		font.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <font.h>

struct PSF1Font* font_load_psf1(void* directory, ushort* path, void* (*load_file_ptr)(void*, ushort*), void* (*malloc_ptr)(size_t)){
	void* font = load_file_ptr(directory, path);
	if(font == NULL){
		return NULL;
	}
}