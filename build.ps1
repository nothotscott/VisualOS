################################################################################
## File:		build.ps1
## Builds VisualOS and provides additional functionality such as running or
## using VirtualBox tools
## This script is agnostic to whether it's being called from WSL pwsh or native
## windows powerhell
##
## Copyright 2020 Scott Maday
## Check the LICENSE file that came with this program for licensing terms
################################################################################


param([string[]] $tasks)
[System.Collections.ArrayList] $tasks = $tasks
$tasks = $tasks -split ","

# Configurable
$OSNAME = "VisualOS"
$BUILD_DIR = "build"
$OVMF_URL = "https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd" #"https://versaweb.dl.sourceforge.net/project/edk2/OVMF/OVMF-X64-r15214.zip"
$OVMF_NAME = "OVMF.fd"
$DEFAULT_TASK = @("setup", "build-bootloader", "build-libc", "build-kernel", "build-vos", "build-img")

# Don't configure
$DEBUG_MODE = $true
$ABSOLUTE = Split-Path $script:MyInvocation.MyCommand.Path
$WSL = $(Get-ChildItem -Path Env:WSL_DISTRO_NAME -ErrorAction SilentlyContinue | Select-Object -last 1).Value
$VBOX = Get-Command "VBoxManage.exe" -ErrorAction SilentlyContinue
[System.Collections.ArrayList]$MAKE_VARS = @("OSNAME=$OSNAME", "BUILD_DIR=$BUILD_DIR")

# Ability to invoke windows if running WSL
function invoke-host {
	param (
		[string] $command
	)
	If ($WSL -ne $null) {
		return powershell.exe $command
	} else {
		return Invoke-Expression $command
	}
}

# Dependent on WSL or windows
$ABSOLUTE_WIN = & { If ($WSL -ne $null) { wslpath -w $ABSOLUTE } Else { $ABSOLUTE } }
$TEMP_DIR = & { If ($WSL -ne $null) { powershell.exe Write-Host '$env:temp' } Else { $env:temp } }
$NUM_PROCESSORS = [int](invoke-host "(Get-CimInstance Win32_ComputerSystem).NumberOfLogicalProcessors")

# Dependent configurations
$OVMF_DIR_WIN = Join-Path $TEMP_DIR ([io.path]::GetFileNameWithoutExtension($OVMF_URL)).ToString()

################################################################################


function build {
	param (
		[string] $target 
	)
	$jobs = $NUM_PROCESSORS - 1
	if ($WSL -eq $null) {
		wsl make "-j$jobs" $target $MAKE_VARS
	} else {
		make "-j$jobs" $target $MAKE_VARS
	}
}

function get_ovmf {
	$ovmf_url_name = Split-Path $OVMF_URL -leaf
	$ovmf_parent = Split-Path $OVMF_DIR_WIN -Parent
	$ovmf_file = Join-Path $ovmf_parent $ovmf_url_name
	$ovmf_ext = $ovmf_url_name.Split(".")[1];
	if ((invoke-host "Test-Path $OVMF_DIR_WIN") -eq "True") {
		return;
	}
	Write-Host "OVMF does not exist. Downloading and installing it..."
	Write-Information "Downloading OVMF to $ovmf_file"
	invoke-host "Invoke-WebRequest -Uri $OVMF_URL -OutFile $ovmf_file"
	if ($ovmf_ext -eq ".zip") {
		Write-Information "Extracting to $OVMF_DIR_WIN"
		invoke-host "Expand-Archive $ovmf_file -DestinationPath $OVMF_DIR_WIN"
		Write-Information "Cleaning up"
		invoke-host "Remove-Item $ovmf_file"
	} else {
		Write-Information "Organizing files"
		invoke-host "New-Item $OVMF_DIR_WIN -ItemType 'directory'"
		invoke-host "Move-Item $ovmf_file $OVMF_DIR_WIN/$OVMF_NAME"
	}
}

function launch-qemu {
	param (
		[bool] $debug 
	)
	$program_args = "-drive file=$ABSOLUTE_WIN/$BUILD_DIR/$OSNAME.img "
	$program_args += "-drive if=pflash,format=raw,unit=0,readonly=on,file=$OVMF_DIR_WIN/$OVMF_NAME "
	$program_args += "-machine type=q35 -cpu qemu64 -smp 2 -m 512M -net none -serial stdio -no-reboot "
	if ($debug -eq $true) {
		powershell.exe "Start-Process qemu-system-x86_64.exe -ArgumentList '$program_args -s -S'"
	} else {
		Start-Process "qemu-system-x86_64.exe" -NoNewWindow -Wait -ArgumentList $program_args
	}
}

function vbox-manage {
	param (
		[string] $params
	)
	if ($VBOX -eq $null) {
		Write-Host "Cannot find VirtualBoxManage.exe, check your PATH and ensure VirtualBox is included"
		return
	}
	Start-Process $VBOX -NoNewWindow -Wait -ArgumentList $params
}

#######################

if ($tasks.Count -eq 0) {
	$tasks = $DEFAULT_TASK
}
if ($WSL -ne $null) {
	Write-Host "Detected WSL $WSL where $ABSOLUTE is the equivalent windows path $ABSOLUTE_WIN"
}

foreach ($task in $tasks) {
	if ($task -eq "setup") {
		build setup
		get_ovmf
	} elseif ($task -eq "asmdump") {
		$MAKE_VARS.Add("ASMDUMP=true") | Out-Null
	} elseif ($task -eq "debug") {
		$DEBUG_MODE = $true
		$MAKE_VARS.Add("DEBUG_MODE=true") | Out-Null
	}

	elseif ($task -eq "clean") {
		build clean
		# Remove-Item $OVMF_DIR_WIN -Recurse -Force
	} elseif ($task -eq "clean-img") {
		build clean-img
	} elseif ($task -eq "clean-bootloader") {
		build clean-bootloader
	} elseif ($task -eq "clean-libs") {
		build clean-libs
	} elseif ($task -eq "clean-vos") {
		build clean-vos
	}
	
	elseif ($task -eq "build-bootloader") {
		build bootloader
	} elseif ($task -eq "build-libs") {
		build libs
	} elseif ($task -eq "build-kernel") {
		build kernel
	} elseif ($task -eq "build-vos") {
		build vos
	} elseif ($task -eq "build-img") {
		build img
	} elseif ($task -eq "build-iso") {
		build iso
	}

	elseif ($task -eq "vbox-vdi") {
		vbox-manage "convertfromraw --format VDI $BUILD_DIR/$OSNAME.img $BUILD_DIR/$OSNAME.vdi"
	}

	elseif ($tasks -eq "explore") {
		explorer.exe .
	} elseif ($tasks -eq "run") {
		get_ovmf
		launch-qemu
	} elseif ($tasks -eq "run-debug") {
		get_ovmf
		launch-qemu $true
	}

}