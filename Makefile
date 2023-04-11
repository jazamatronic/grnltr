# Project Name
BUILD_TARGET ?= pod

# Sources
CPP_SOURCES = grnltr.cpp windows.cpp 

ifeq "$(BUILD_TARGET)" "bluemchen"
TARGET = grnltr_bluemchen
BLUEMCHEN_DIR = ./kxmx_bluemchen
CPP_SOURCES += $(BLUEMCHEN_DIR)/src/kxmx_bluemchen.cpp bluemchen.cpp
endif

ifeq "$(BUILD_TARGET)" "pod"
TARGET = grnltr_pod
CPP_SOURCES += pod.cpp
endif

# Library Locations
LIBDAISY_DIR = ../../libDaisy
DAISYSP_DIR = ../../DaisySP

# Includes FatFS source files within project.
USE_FATFS = 1

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

ifeq "$(BUILD_TARGET)" "bluemchen"
C_DEFS += -DTARGET_BLUEMCHEN
C_INCLUDES += -I$(BLUEMCHEN_DIR)/src
endif

ifeq "$(BUILD_TARGET)" "pod"
C_DEFS += -DTARGET_POD
endif

VERSION = $(shell git tag --sort=v:refname | tail -n1)
ifdef DEBUG_POD 
C_DEFS += -DDEBUG_POD
VERSION := $(VERSION)-dbg
endif
C_DEFS += -DVER=\"$(VERSION)\"

release: build/$(TARGET).bin
	cp build/$(TARGET).bin rel/$(TARGET).$(VERSION).bin
