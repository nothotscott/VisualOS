#!/usr/bin/env python3
# Copyright 2021 Scott Maday
#
# This file is for native windows compatibility so long as it's invoked on the WSL UNC path

import os, sys, subprocess

if __name__ == "__main__":
	os.chdir(os.path.dirname(os.path.abspath(__file__)))
	if sys.platform == "win32":
		print("Switching contexts to WSL")
		subprocess.run(["wsl", "python3", "build.py"] + sys.argv[1:])
	else:
		import build
		build.main()