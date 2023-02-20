# Project Name
TARGET = grnltr

# Sources
CPP_SOURCES = grnltr.cpp windows.cpp

# Library Locations
LIBDAISY_DIR = ../../libDaisy
DAISYSP_DIR = ../../DaisySP

# Includes FatFS source files within project.
USE_FATFS = 1

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

