################################################################################
## File:		build.ps1
## Builds VisualOS and provides additional functionality such as running or
## using VirtualBox tools
## This script is agnostic to whether it's being called from WSL pwsh or native
## windows powerhell
##
## Copyright 2020 Scott Maday
## You should have received a copy of the GNU General Public License along with this program. 
## If not, see https://www.gnu.org/licenses/gpl-2.0
################################################################################


param($tasks)

# Configurable
$OSNAME = "VisualOS"
$BUILD_DIR = "build"
$OVMF_URL = "https://github.com/Absurdponcho/OVMFbin"
$DEFAULT_TASK = @("setup", "build-bootloader", "build-library", "build-kernel", "build-img")

# Don't configure
$ABSOLUTE = Split-Path $script:MyInvocation.MyCommand.Path
$WSL = $(Get-ChildItem -Path Env:WSL_DISTRO_NAME -ErrorAction SilentlyContinue | Select-Object -last 1).Value
$VBOX = Get-Command "VBoxManage.exe" -ErrorAction SilentlyContinue
[System.Collections.ArrayList]$MAKE_VARS = @("OSNAME=$OSNAME", "BUILD_DIR=$BUILD_DIR")

# Dependent on WSL or windows
$ABSOLUTE_WIN = & { If ($WSL -ne $null) { wslpath -w $ABSOLUTE } Else { $ABSOLUTE } }
$TEMP_DIR = & { If ($WSL -ne $null) { powershell.exe Write-Host '$env:temp' } Else { $env:temp } }

# Dependent configurations
$OVMF_DIR_WIN = Join-Path ($TEMP_DIR) (Split-Path $OVMF_URL -leaf)


function build {
	param (
		[string] $target 
	)
	if ($WSL -eq $null) {
		wsl make $target $MAKE_VARS
	} else {
		make $target $MAKE_VARS
	}
}

function get_ovmf {
	$ovmf_name = Split-Path $OVMF_URL -leaf
	$ovmf_parent = Split-Path $OVMF_DIR_WIN -Parent
	if ($WSL -ne $null) {
		if ((powershell.exe "Test-Path $OVMF_DIR_WIN") -eq "True") {
			return;
		}
	} else {
		if ((Test-Path $OVMF_DIR_WIN) -eq $true) {
			return;
		}
	}
	git clone $OVMF_URL
	if ($WSL -ne $null) {
		powershell.exe "Move-Item $ovmf_name $ovmf_parent"
	} else {
		Move-Item $ovmf_name $ovmf_parent
	}
}

function launch-qemu {
	$args = "-drive file=$ABSOLUTE_WIN/$BUILD_DIR/$OSNAME.img -m 256M -no-reboot -cpu qemu64 -net none -drive if=pflash,format=raw,unit=0,file=$OVMF_DIR_WIN/OVMF_CODE-pure-efi.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=$OVMF_DIR_WIN/OVMF_VARS-pure-efi.fd"
	Start-Process "qemu-system-x86_64.exe" -NoNewWindow -Wait -ArgumentList $args
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

if ($tasks -eq $null) {
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
	}

	elseif ($task -eq "clean") {
		build clean
		# Remove-Item $OVMF_DIR_WIN -Recurse -Force
	} elseif ($task -eq "clean-img") {
		build clean-img
	} elseif ($task -eq "clean-bootloader") {
		build clean-gnuefi
	} elseif ($task -eq "clean-vos") {
		build clean-vos
	}
	
	elseif ($task -eq "build-bootloader") {
		build gnu-efi
	} elseif ($task -eq "build-library") {
		build library
	} elseif ($task -eq "build-kernel") {
		build kernel
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
	}

}