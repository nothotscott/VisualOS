param($tasks)

# Configurable
$OSNAME = "VisualOS"
$BUILD_DIR = "build"
$OVMF_URL = "https://github.com/Absurdponcho/OVMFbin"

# Don't configure
$ABSOLUTE = Split-Path $script:MyInvocation.MyCommand.Path
$OVMF_DIR = Split-Path $OVMF_URL -leaf
$DEFAULT_TASK = @("setup", "build-bootloader","build-kernel","build-img")
$MAKE_VARS = @("OSNAME=$OSNAME", "BUILD_DIR=$BUILD_DIR")
$VBOX = Get-Command "VBoxManage.exe" -ErrorAction SilentlyContinue

function build {
	param (
        [string] $target 
	)
	wsl make $target $MAKE_VARS
}

function get_ovmf {
	git clone $OVMF_URL
}

function launch-qemu {
	qemu-system-x86_64 -drive file="$ABSOLUTE/$BUILD_DIR/$OSNAME.img" -m 256M -cpu qemu64 -net none -drive if=pflash,format=raw,unit=0,file="$ABSOLUTE/$OVMF_DIR/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$ABSOLUTE/$OVMF_DIR/OVMF_VARS-pure-efi.fd"
}

function vbox-manage {
	param (
		[string[]] $params
	)
	if($VBOX -eq $null){
		Write-Host "Cannot find VirtualBoxManage.exe, check your PATH and ensure VirtualBox is included"
		return
	}
	& $VBOX $params
}

#######################

if($null -eq $tasks){
	$tasks = $DEFAULT_TASK
}

foreach($task in $tasks){
	if($task -eq "setup"){
		build setup
		if(-not (Test-Path "$OVMF_DIR")){
			get_ovmf
		}
	}

	if($task -eq "clean"){
		build clean
	}
	if($task -eq "bootloaderclean"){
		build gnueficlean
	}
	if($task -eq "kernelclean"){
		build kernelclean
	}

	elseif($task -eq "build-bootloader"){
		build gnu-efi
	}
	elseif($task -eq "build-kernel"){
		build kernel
	}
	elseif($task -eq "build-img"){
		build img
	}
	elseif($task -eq "build-iso"){
		build iso
	}

	elseif($task -eq "vbox-vdi"){
		vbox-manage @("convertfromraw", "--format", "VDI", "$BUILD_DIR/$OSNAME.img", "$BUILD_DIR/$OSNAME.vdi")
	}

	elseif($tasks -eq "run"){
		launch-qemu
	}

}