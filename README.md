# VisualOS
VisualOS is a x86-64 operating system that aims to remove the need for command line interfaces.
This project follows along [Poncho's youtuber series](https://www.youtube.com/playlist?list=PLxN4E629pPnJxCQCLy7E0SQY_zuumOVyZ).
This operating system is not designed to be POSIX compatible.

## Purpose
I've personally felt command line interfaces are a primitive means of interfacing with a computer.
Command lines aren't user friends since users are expected to memorize arguments and each program's unique syntax. Users also frequently have to repeat themselves when combining tasks (i.e `mkdir foo && cd foo`)
Command lines aren't computer friendly since programs have to parse and sanitize the user's input through a single interface.
This OS was a shower thought about the possibilty to evolve pass the archaic constraints command lines impose on computer enthusiasts.
As such, I thought it would be a fun project to develop an OS as a proof of concept of my idea.
I am not an experience OS developer however, so I'm sure there's many better ways of implementing my idea.

# Features
UEFI bootloader

## Planned Features
Programs will use a built-in GUI framework. There will no be standard io. User input will strictly through GUI fields.
Programs that need to access other programs can only do so through libraries that should be bundled with the program in question.
Programs are encouraged to save user settings in a file that avoids asking them for input that will likley be the same the next time they go to run it.
Drives will mount similar to Windows, but present themselves as `$n/your/files.txt` where n is the drive number

## Lua
Lua is the primary scripting language of this OS. Lua easily integrates with the C language and is lightweight.
Lua scripts will provide a simplified means of running programs nativley. There will be 