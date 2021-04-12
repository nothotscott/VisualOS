# VisualOS
VisualOS is a x86-64 operating system with a "microish-kernel" that aims to remove the need for command line interfaces. 
This project loosely follows along [Poncho's youtube series](https://www.youtube.com/playlist?list=PLxN4E629pPnJxCQCLy7E0SQY_zuumOVyZ). 
This operating system is designed to be somewhat POSIX compatible. 

## Purpose
I've personally felt command line interfaces are a primitive means of interfacing with a computer. 
Command lines aren't user friendly since users are expected to memorize arguments and flags and each program's unique syntax. 
Users also frequently have to repeat themselves when combining tasks (i.e `mkdir foo && cd foo`) 
Command lines aren't friendly for computers either, since programs have to parse and sanitize the user's input through a single interface. 
This OS was a shower thought about the possibilty to evolve pass the archaic constraints command lines impose on computer enthusiasts. 
As such, I thought it would be a fun project to develop an OS as a proof of concept of my idea. 
I am not an experience OS developer however, so I've come to learn and I'm sure there's many better ways of implementing my idea.

## Included dependencies
The following are included and minimally modified to adapt to the build process and the VOS runtime
- gnu-efi is included under the BSD license
- PDCLib is included under the [Public domain](src/pdclib/COPYING.CC0)
	- Though PDCLib is public domain, credit to the original authors can be found in [pdclib/Notes.txt](src/pdclib/Notes.txt)

# Build
The kernel is written in C and x86-64 assembly. It uses gcc and nasm respectivley. It is best to compile in WSL and run the `build.ps1` PowerShell script. Tasks are comma separated. Common build tasks:
- `.\build.ps1 clean-bootloader,build-bootloader`
- `.\build.ps1 clean-libc,build-libc`
- `.\build.ps1 clean-vos,build-kernel`
- `.\build.ps1 build-img,run`

Make sure the following tools are installed
- GCC suite
- nasm
- mtools

# Features
- UEFI bootloader
- [PDCLib](https://github.com/nothotscott/pdclib) libc for both kernel and userspace

## Upcoming Features
- Symmetric Multiprocessing
- Scripting ([Lua](#Lua) or perhaps my own language)
### Planned Visual Features
- Programs will use a built-in GUI framework. Standard IO will redirect to GUI. User input will strictly through GUI fields.
- Programs that need to access other programs can only do so through libraries that should be bundled with the program in question.
- Programs are encouraged to save user settings in a file that avoids asking them for input that will likely be the same the next time they go to run it.
- Drives will mount similar to Windows, but present themselves as `$n/your/files.txt` where n is the drive number.
- Programs will work best in C++, since my library will be object oriented.

# Lua
Lua is the primary scripting language of this OS. Lua easily integrates with the C language and is lightweight.
Lua scripts will provide a simplified means of running programs nativley.
Lua is also far simpler then shell scripting and can be adapted for many situations. I plan lua scripts to look like the following:
```lua
local gui = vos.Gui:CreateWindow()
local layout = gui:SetLayout("STACK")
local addDeviceToList = function(device, isConnected)
	if device:GetType() == "DISK" and device:GetMedium() == "USB" then
		local label = vos.Gui:CreateComponent("LABEL")
		label.Text = tostring(device)
		layout:AddComponent(label)
	end
end

for i,device in pairs(vos.Devices:GetDevices()) do
	addDeviceToList(device, true)
end
vos.Devices.DeviceInserted:connect(addDeviceToList)
```