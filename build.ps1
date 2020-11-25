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

# Don't configure
$ABSOLUTE = Split-Path $script:MyInvocation.MyCommand.Path
$OVMF_DIR = Split-Path $OVMF_URL -leaf
$DEFAULT_TASK = @("setup", "build-bootloader", "build-kernel", "build-img")
$MAKE_VARS = @("OSNAME=$OSNAME", "BUILD_DIR=$BUILD_DIR")
$VBOX = Get-Command "VBoxManage.exe" -ErrorAction SilentlyContinue
$WSL = $(Get-ChildItem -Path Env:WSL_DISTRO_NAME -ErrorAction SilentlyContinue | Select-Object -last 1).Value

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
	if ((Test-Path "$OVMF_DIR") -eq $true) {
		return;
	}
	git clone $OVMF_URL
}

function launch-qemu {
	Start-Process "qemu-system-x86_64.exe" -PassThru -NoNewWindow -Wait -ArgumentList "-drive file=$ABSOLUTE/$BUILD_DIR/$OSNAME.img -m 256M -cpu qemu64 -net none -drive if=pflash,format=raw,unit=0,file=$ABSOLUTE/$OVMF_DIR/OVMF_CODE-pure-efi.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=$ABSOLUTE/$OVMF_DIR/OVMF_VARS-pure-efi.fd"
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
	$ABSOLUTE = wslpath -w $ABSOLUTE
	Write-Host "Detected WSL $WSL in equivalent windows path $ABSOLUTE"
}

foreach ($task in $tasks) {
	if ($task -eq "setup") {
		build setup
		get_ovmf
	}

	if ($task -eq "clean") {
		build clean
	}
	if ($task -eq "clean-super") {
		build clean-super
		Remove-Item $OVMF_DIR -Recurse -Force
	}
	if ($task -eq "clean-dev") {
		build clean-dev
	}
	if ($task -eq "clean-bootloader") {
		build clean-bootloader
	}
	if ($task -eq "clean-kernel") {
		build clean-kernel
	}

	elseif ($task -eq "build-bootloader") {
		build gnu-efi
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