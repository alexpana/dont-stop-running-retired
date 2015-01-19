#!/usr/bin/python
import os
import subprocess

ROOT = os.path.join(os.path.dirname(os.path.realpath(__file__)), "..")
ASSETS_DIR = os.path.normpath(os.path.join(ROOT, "extern", "assets"))
BUILD_DIR = os.path.normpath(os.path.join(ROOT, "bin", "data"))

class color:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def usage():
	print "usage: build"

def build_textures():
	CONVERT_CMD = "nvcompress -bc2 %s %s"

	print "============================="
	print "Building textures"
	print "============================="
	print

	# make sure destination path exists
	if not os.path.exists(os.path.join(BUILD_DIR, "textures")):
		os.mkdir(os.path.join(BUILD_DIR, "textures"))

	for file in os.listdir(os.path.join(ASSETS_DIR, "textures")):
		print "* Converting:", color.OKGREEN + file + color.ENDC

		in_file = os.path.join(ASSETS_DIR, "textures", file)
		out_file = os.path.join(BUILD_DIR, "textures", file)
		out_file = os.path.splitext(out_file)[0] + ".dds"

		if os.path.exists(out_file):
			print "File exists, skipping"
		else:
			subprocess.call(CONVERT_CMD % (in_file, out_file), shell = True)
		print

if __name__ == "__main__":
	build_textures()
	
