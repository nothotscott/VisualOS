;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		isr.asm
;; 
;; Copyright 2020 Scott Maday
;; You should have received a copy of the GNU General Public License along with this program. 
;; If not, see https://www.gnu.org/licenses/gpl-2.0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


EXTERN	g_pagetable_l4


GLOBAL	paging_load
paging_load:
	mov	rax, g_pagetable_l4
	mov	cr3, rax
	ret