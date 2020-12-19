;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		kernel.inc
;; Description:	Kernel level macros for nasm
;; 
;; Copyright 2020 Scott Maday
;; You should have received a copy of the GNU General Public License along with this program. 
;; If not, see https://www.gnu.org/licenses/gpl-2.0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


%macro	PUSH_REG 0
	push	rax
	push	rcx
	push	rdx
	push	r8
	push	r9
	push	r10
	push	r11
%endmacro

%macro	POP_REG 0
	pop	r11
	pop	r10
	pop	r9
	pop	r8
	pop	rdx
	pop	rcx
	pop	rax
%endmacro
