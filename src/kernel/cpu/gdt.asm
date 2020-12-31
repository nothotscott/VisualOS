;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		gdt.asm
;; 
;; Copyright 2020 Scott Maday
;; You should have received a copy of the GNU General Public License along with this program. 
;; If not, see https://www.gnu.org/licenses/gpl-2.0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


EXTERN	g_gdt_descriptor
EXTERN	print_char

SECTION	.text

GLOBAL	gdt_load
gdt_load:
	cli
	lgdt	[g_gdt_descriptor]
	; TODO move TSS segment and load (ltr)
	mov		rdi, rsp
	push	0x10				; push data segment: g_idt[2]
	push	rdi					; push stack pointer
	pushfq						; push flags
	push	QWORD 0x08			; push code segment: g_idt[1]
	push	.flush				; push where RIP should go
	iretq						; far return in new code segment
	.flush:
		ret