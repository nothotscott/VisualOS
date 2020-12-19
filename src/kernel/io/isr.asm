;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		isr.asm
;; Description:	x86_64 Interrupt Descriptor Table functions
;; 
;; Copyright 2020 Scott Maday
;; You should have received a copy of the GNU General Public License along with this program. 
;; If not, see https://www.gnu.org/licenses/gpl-2.0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


EXTERN	isr1_handler

GLOBAL	isr1
isr1:
	jmp		isr_common

isr_common:
	PUSH_REG
	; mov	rdi, rsp	; stack location as a parameter
	call	isr1_handler
	POP_REG
	iretq