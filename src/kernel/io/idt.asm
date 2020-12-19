;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		idt.asm
;; Description:	x86_64 Interrupt Descriptor Table functions
;; 
;; Copyright 2020 Scott Maday
;; You should have received a copy of the GNU General Public License along with this program. 
;; If not, see https://www.gnu.org/licenses/gpl-2.0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

EXTERN	g_idt_descriptor

GLOBAL	idt_load
idt_load:
	lidt	[g_idt_descriptor]
	sti
	ret