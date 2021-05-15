# VisualOS
VisualOS is a x86-64 operating system with a "microish kernel" that aims to remove the need for command line interfaces. 
This project loosely follows along [Poncho's youtube series](https://www.youtube.com/playlist?list=PLxN4E629pPnJxCQCLy7E0SQY_zuumOVyZ). 
This operating system is designed to be somewhat POSIX compatible.

## Purpose
I've personally felt command line interfaces are a primitive means of interfacing with a computer. 
Command lines aren't user friendly since users are expected to memorize arguments and flags and each program's unique syntax. 
Users also frequently have to repeat themselves when combining tasks (i.e `mkdir foo && ls`) 
Command lines aren't friendly for computers either, since programs have to parse and sanitize the user's input through a single interface. 
This OS was a shower thought about the possibilty to evolve pass the archaic constraints command lines impose on computer enthusiasts. 
As such, I thought it would be a fun project to develop an OS as a proof of concept of my idea. 
I am not an experience OS developer however, so I've come to learn and I'm sure there's many better ways of implementing my idea.

## What's a "microish kernel"
I coined the term "microish-kernel" to mean: a hybrid kernel who, on a spectrum ranging from monolithic-like to microkernel-like, lies at the microkernel extreme but isn't truly a microkernel. The difference with VisualOS is, the scheduler, memory management, and file system reside in kernelspace. These services in a traditional microkernel reside in userspace. Besides those, VisualOS's drivers, applications, and servers execute in userspace like a normal microkernel.

## Included dependencies
The following are included and minimally modified to adapt to the build process and the VOS runtime
- [limine](https://github.com/limine-bootloader/limine) is distributed under the [BSD 2-Clause](https://github.com/limine-bootloader/limine/blob/trunk/LICENSE.md)
- [musl](https://git.musl-libc.org/cgit/musl) is included under the [MIT License](src/libraries/musl/COPYRIGHT)
	- Note: musl has been modified to work with VisualOS. 

# Build
A Unix-like environment is required to build VisualOS. The following systems have been tested: WSL, Linux, MacOS  
Make sure the following tools are installed on the Unix-like system
- python3
- gcc suite or clang suite
- nasm
- mtools

Installing the required tools can be done by executing `sudo ./install_tools.sh` for debian-based systems or systems that use the apt package manager.
Simply execute `./main.py` to automatically build. Use `./main.py -h` for a list of more granular build options and execute `./main.py -r` to run a qemu instance of VisualOS.
Note: `python3 main.py` can be executed natively on Windows if on PowerShell and traversed to the correct WSL UNC path. The script will automatically re-execute itself in WSL.

# Features
- stivale2 boot protocol kernel
- Included [libc](src/libraries/musl) for both kernel and userspace

## Features in progress
- Symmetric Multiprocessing
## Upcoming Features
- First userspace programs and drivers
- Scripting ([Lua](#Lua) or perhaps my own language)
### Planned Visual Features
- Programs will use a built-in GUI framework. Standard IO will redirect to GUI. User input will strictly through GUI fields.
- Programs that need to access other programs can only do so through libraries that should be bundled with the program in question.
- Programs are encouraged to save user settings in a file that avoids asking them for input that will likely be the same the next time they go to run it.
- Drives will mount similar to Windows, but present themselves as `$n/your/files.txt` where n is the drive number.
- Programs will work best in C++, since my library will be object oriented.

# Lua (not released)
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