;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		idt.asm
;; 
;; Copyright 2020 Scott Maday
;; You should have received a copy of the GNU General Public License along with this program. 
;; If not, see https://www.gnu.org/licenses/gpl-2.0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


GLOBAL	outb
outb:	; port=rdi, value=rsi
	mov	rdx, rdi	; port 
	mov rax, rsi	; value
	out dx, al
	ret

GLOBAL	inb
inb:	; port=rdi, return value=al
	mov	rdx, rdi	; port 
	in	al, dx
	ret