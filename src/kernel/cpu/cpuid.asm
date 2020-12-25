;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		cpuid.asm
;; 
;; Copyright 2020 Scott Maday
;; You should have received a copy of the GNU General Public License along with this program. 
;; If not, see https://www.gnu.org/licenses/gpl-2.0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


GLOBAL	cpuid_get_str
cpuid_get_str:	; rdi=[vendor_str], rsi=[brand_str]
	; Get vendor
	mov		eax, 0x00
	cpuid
	mov		[rdi + 0], ebx
	mov		[rdi + 4], edx
	mov		[rdi + 8], ecx
	mov		[rdi + 12], BYTE 0
	; Get brand
	mov		eax, 0x80000000	; check if brand feature is implemented
	cpuid
	cmp		eax, 0x80000004
	jl		.return			; not supported
	mov		eax, 0x80000002
	cpuid
	mov		[rsi + 0], eax
	mov		[rsi + 4], ebx
	mov		[rsi + 8], ecx
	mov		[rsi + 12], edx
	mov		eax, 0x80000003
	cpuid
	mov		[rsi + 16], eax
	mov		[rsi + 20], ebx
	mov		[rsi + 24], ecx
	mov		[rsi + 28], edx
	mov		eax, 0x80000004
	cpuid
	mov		[rsi + 32], eax
	mov		[rsi + 36], ebx
	mov		[rsi + 40], ecx
	mov		[rsi + 44], edx
	.return:
		ret

GLOBAL cpuid_get_features
cpuid_get_features:		; rdi=[features]
	mov		eax, 0x01		; set 1 and 2
	cpuid
	mov		[rdi + 0], edx
	mov		[rdi + 4], ecx
	mov		eax, 0x07		; set 3, 4, and 5
	mov		ecx, 0x00
	cpuid
	mov		[rdi + 8], ebx
	mov		[rdi + 12], ecx
	mov		[rdi + 16], edx
	mov		ecx, 0x01		; set 6
	cpuid
	mov		[rdi + 20], eax
	mov		eax, 0x80000001	; amd1 and amd2
	cpuid
	mov		[rdi + 24], edx
	mov		[rdi + 28], ecx
	; Enable Intel's feature set 1 and AMD feature set 1 that are idempotent 
	and		edx, 0b00000001100000111111111111111111 ;try mov
	or		[rdi + 0], edx
	ret