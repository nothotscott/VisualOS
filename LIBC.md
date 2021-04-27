# Libc discussion
VisualOS is unique in the fact that the kernel needs a special kernel libc (which calls `syshandler_get`) to link against as well as userspace programs being linked against the normal libc (which performs syscall instructions)  
I'm not going to make my own libc since userspace components will need a libc regardless.

## Libc checklist
These are things a libc must have in order to be qualified to be a VOS-compliant
- No convoluted configuration scripts or needlessly complex ways to generate build files
- Build files must be relatively simple since I'll be translating it into a special Makefile. Bonus points for a plain makefile
- Can't be too OS dependent. Any OS dependence must be easily stripped out or altered to conform to VisualOS.
- Needs additional libraries (math and threading)
- Bonus points for having a permissive license

## Libc checklist
### OSDev Wiki
- [ ] Glibc
	- Convoluted configuration script
	- Inseparably linux dependent
- [x] Musl
	- Linux dependent, but is separable
	- Has a configuration script, but can be disregarded
- [ ] Newlib
	- Convoluted configuration script
- [x] PDCLib
	- Uses CMake, but is relatively simple
	- Does not have additional libraries
- [ ] uLibc
	- Makefiles seems needlessly large. I could distill just the parts I need.
	- Non-permissive license (LGPL)
- [x] Diet Libc
	- Non-permissive license (GPL2)
- [ ] Google's Bionic
	- Needlessly complex build process
	- Inseparably andrioid dependent
- [ ] Sortix Libc
	- Inseparably sortix dependent
- [ ] Libc11
	- Convoluted configuration script
- [ ] mlibc
	- Needlessly complex build process
### Wikipedia
- [ ] BSD libc
	- Inseparably FreeBSD dependent
- [ ] klibc
	- Inseparably linux dependent