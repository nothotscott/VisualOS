;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		gdt.asm
;; 
;; Copyright 2020 Scott Maday
;; You should have received a copy of the GNU General Public License along with this program. 
;; If not, see https://www.gnu.org/licenses/gpl-2.0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


EXTERN	g_gdt_descriptor

SECTION	.text

; Please credit me if you're going to use this for reference :)
GLOBAL	gdt_load
gdt_load:
	cli
	lgdt	[g_gdt_descriptor]
	; TODO move TSS segment and load (ltr)
	; This will exploit iretq's ability to set segments in long mode
	; The order iretq will pop is: RIP, CS, RFLAGS, RSP, SS
	; We first need to save data we wish to keep
	pop		rsi					; keep the return address in rsi
	mov		rdi, rsp			; keep the old stack pointer in rdi
	; Now to push what iretq needs
	push	QWORD 0x10			; push data segment: g_idt[2]
	push	rdi					; push old stack pointer
	pushfq						; push flags
	push	QWORD 0x08			; push code segment: g_idt[1]
	push	rsi					; set rip to the return address from earlier
	iretq						; far return in new code segment