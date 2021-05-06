# Copyright 2021 Scott Maday
#
# The main build functions

import os, sys, logging, subprocess
import multiprocessing, shutil, urllib.request
#import pycdlib
from platform import uname
from optparse import OptionParser
from io import BytesIO
from xml.etree import ElementTree

# Main configuration
OSNAME			= "VisualOS"
BUILD_DIR		= "build"
BIN_DIR			= "bin"
DEFAULT_TASKS	= ["bootloader", "libs", "kernel", "vos"]
# Qemu configuration
QEMU_CPU		= "qemu64"
QEMU_MEMORY		= "512M"
QEMU_CORES		= 2
QEMU_MISC		= "-machine type=q35 -net none -serial stdio -no-reboot"

# Internal configuration
BOOTLOADER_RELEASE_REPO		= "https://github.com/limine-bootloader/limine.git"
BOOTLOADER_RELEASE_BRANCH	= "latest-binary"
BOOTLOADER_NAME				= "bootloader"
OVMF_RELEASE_URL			= "https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd"
OVMF_NAME					= "OVMF.fd"
MAKE_VARS					= ["OSNAME=" + OSNAME, "BUILD_DIR=" + BUILD_DIR]
OS_EXECUTABLE				= "vos.elf"
ROOTFS_XML					= "rootfs.xml"
IMG_EXT						= "img"

is_wsl = "Microsoft" in uname().release
logger = logging.getLogger()
debug = False
toolchain = "gcc"

### System functions ###

def host_path_absolute(path):
	abs_path = os.path.abspath(path)
	if is_wsl:
		completed_proc = subprocess.run(["wslpath", "-w", abs_path], capture_output=True)
		return completed_proc.stdout.decode("utf-8")[:-1]	# chop off newline
	else:
		return abs_path

def execute(command, exit_if_failed = True):
	results = os.system(command)
	if exit_if_failed and results != 0:
		logger.critical("Error occurred when executing the task: {}\nCode: {}".format(command, results))
		sys.exit(results)

def make(target):
	jobs = max(1, multiprocessing.cpu_count() - 1)
	make_vars = " ".join(MAKE_VARS + ["DEBUG_MODE=true"] if debug else MAKE_VARS)
	execute("make -j{} {} {}".format(jobs, target, make_vars))
def make_clean(target):
	execute("make clean-{}".format(target))

### Tasks ###

def make_executable():
	make("vos")

def create_img():
	if not os.path.exists(BUILD_DIR) or not os.path.isdir(BIN_DIR):
		logger.error("No build directory {}/".format(BUILD_DIR))
		return
	
	img_path = os.path.join(BUILD_DIR, "{}.{}".format(OSNAME, IMG_EXT))
	executable_path = os.path.join(BUILD_DIR, OS_EXECUTABLE)
	if not os.path.exists(os.path.join(BIN_DIR, BOOTLOADER_NAME)):
		logger.warning("No bootloader found before creating image. Getting it now.")
		get_bootloader()
	if not os.path.exists(executable_path):
		logger.warning("No VOS executable first. Creating it first.")
		make_executable()
	if os.path.exists(img_path):
		if os.path.getmtime(executable_path) < os.path.getmtime(img_path):
			logger.warning("Last image more recent than VOS executable. Remaking the executable, deleting and replacing image.")
			make_executable()
		else:
			logger.warning("Image already exists at {}. Deleting and replacing it anyway.".format(img_path))
		os.remove(img_path)
	execute("dd if=/dev/zero of={} bs=1M count=0 seek=512".format(img_path))
	execute("printf 'o\nn\np\n1\n\n\nt\nc\nw\n' | fdisk {}".format(img_path))
	execute("mkfs.vfat {}".format(img_path))
	
	# TODO abstract this
	tree = ElementTree.parse(ROOTFS_XML)
	def traverse(elem, accumulator):
		if elem.tag == "directory":
			name = elem.attrib["name"]
			fs_path = "{}/{}".format(accumulator, name)
			logger.debug("[{}]: Directory {}".format(img_path, fs_path))
			execute("mmd -i {} ::{}".format(img_path, fs_path))
			for sub_elem in elem:
				traverse(sub_elem, fs_path)
		if elem.tag == "wildcard":
			name = elem.attrib["name"]
			fs_path = "{}/{}".format(accumulator, name)
			dir_path = elem.text
			if not os.path.exists(dir_path):
				logger.error("Could not find directory {} needed for {}".format(dir_path, fs_path))
				return
			logger.debug("[{}]: Wildcard {} -> {}".format(img_path, dir_path, fs_path))
			execute("mcopy -i {} -s {} ::{}".format(img_path, dir_path, fs_path))
		if elem.tag == "file":
			file_path = elem.text
			if not os.path.exists(file_path):
				logger.error("Could not find file {} needed for {}".format(file_path, accumulator))
				return
			logger.debug("[{}]: File {} -> {}".format(img_path, file_path, accumulator if accumulator != "" else "/"))
			execute("mcopy -i {} {} ::{}".format(img_path, file_path, accumulator))

	root_elem = tree.getroot()
	for sub_elem in root_elem:
		traverse(sub_elem, "")
	

def get_bootloader():
	bootloader_path = os.path.join(BIN_DIR, BOOTLOADER_NAME)
	if not os.path.exists(BIN_DIR):
		os.makedirs(BIN_DIR)
	if os.path.exists(bootloader_path):
		logger.warning("Bootloader already exists at {}".format(bootloader_path))
		return
	execute("git clone {} -b {} {}".format(BOOTLOADER_RELEASE_REPO, BOOTLOADER_RELEASE_BRANCH, bootloader_path))
	logger.info("Created bootloader at {}".format(bootloader_path))

def get_ovmf():
	ovmf_path = os.path.join(BIN_DIR, OVMF_NAME)
	if not os.path.exists(BIN_DIR):
		os.makedirs(BIN_DIR)
	if os.path.exists(ovmf_path):
		logger.warning("OVMF already exists at {}".format(ovmf_path))
		return
	with urllib.request.urlopen(OVMF_RELEASE_URL) as response:
		with open(ovmf_path, "wb") as ovmf_file:
			ovmf_file.write(response.read())
	logger.info("Created OVMF at {}".format(ovmf_path))

def run_qemu():
	ovmf_path = os.path.join(BIN_DIR, OVMF_NAME)
	img_path = os.path.join(BUILD_DIR, "{}.{}".format(OSNAME, IMG_EXT))
	if not os.path.exists(ovmf_path):
		logger.warning("No OVMF found before running. Getting now.")
		get_ovmf()
	if not os.path.exists(img_path):
		logger.warning("No image found before running. Creating now.")
		create_img()
	qemu = "qemu-system-x86_64{}".format(".exe" if is_wsl else "")
	args = ["-drive", "if=pflash,format=raw,unit=0,readonly=on,file={}".format(host_path_absolute(ovmf_path)), "-drive", "file={}".format(host_path_absolute(img_path))]
	args += ["-cpu", QEMU_CPU, "-smp", str(QEMU_CORES), "-m", QEMU_MEMORY]
	args += QEMU_MISC.split(" ")
	if debug:
		args += ["-s", "-S"]
		if is_wsl:
			subprocess.run("powershell.exe 'Start-Process qemu-system-x86_64.exe -ArgumentList \"{}\"'".format(" ".join(args)), shell=True)
		else:
			command = " ".join([qemu] + args)
			subprocess.Popen(command, stdin=subprocess.PIPE, stderr=subprocess.PIPE, stdout=subprocess.PIPE, shell=True)
	else:
		subprocess.run([qemu] + args)


### Main ###

def main():
	global debug, toolchain
	logging.basicConfig(level=logging.DEBUG)

	parser = OptionParser()
	parser.add_option("-d", "--debug", action="store_true", default=False, help="Enables debug mode")
	parser.add_option("-t", "--toolchain", choices=["gcc", "llvm"], default="gcc", help="Sets the compiler toolchain")
	parser.add_option("--clean-bin", action="store_true", help="Deletes the bin directory")
	parser.add_option("--clean-libs", action="store_true", help="Cleans the libraries build")
	parser.add_option("--clean-kernel", action="store_true", help="Cleans the kernel build")
	parser.add_option("-b", "--bootloader", action="store_true", help="Downloads the latest bootloader binary")
	parser.add_option("-o", "--ovmf", action="store_true", help="Downloads the latest OVMF binary")
	parser.add_option("-l", "--libs", action="store_true", help="Builds the libraries")
	parser.add_option("-k", "--kernel", action="store_true", help="Builds the kernel")
	parser.add_option("-v", "--vos", action="store_true", help="Builds the VisualOS elf file")
	parser.add_option("-i", "--img", action="store_true", help="Builds img")
	parser.add_option("-r", "--run", action="store_true", help="Runs VisualOS in qemu")
	options, _ = parser.parse_args()
	if len(sys.argv) <= 1:
		for task in DEFAULT_TASKS:
			setattr(options, task, True)

	debug = options.debug
	toolchain = options.toolchain
	if options.clean_bin:
		if os.path.exists(BIN_DIR):
			shutil.rmtree(BIN_DIR)
	if options.clean_libs:
		make_clean("libs")
	if options.clean_kernel:
		make_clean("kernel")
	if options.bootloader:
		get_bootloader()
		get_ovmf()
	if options.libs:
		make("libs")
	if options.kernel:
		make("kernel")
	if options.vos:
		make_executable()
	if options.img:
		create_img()
	if options.run:
		run_qemu()

if __name__ == "__main__":
	main()