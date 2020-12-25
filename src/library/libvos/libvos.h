/*
 * File:		libvos.h
 * Description:	VisualOS routines that need to talk to the kernel in order to work
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

// For now, piggyback off the kernel's functions

#ifdef __cplusplus
namespace libvos {
#endif


// Allocates memory for size block
#ifdef __cplusplus
extern "C"
#endif
void* malloc(size_t size);

// Frees the memory at address
#ifdef __cplusplus
extern "C"
#endif
void free(void* address);


#ifdef __cplusplus
} // namespace
#endif
