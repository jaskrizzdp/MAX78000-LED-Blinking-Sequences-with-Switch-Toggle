
# # version 1 working but has gpio problem, header canntot be read
###############################################################################
# Project Makefile for MAX78000 LED Blinking Example
###############################################################################

# -------------------------------
# Path to MaximSDK
# -------------------------------
MAXIM_PATH = ../../../..

# -------------------------------
# Include paths required for MAX78000
# -------------------------------

# CMSIS device headers
IPATH += $(MAXIM_PATH)/Libraries/CMSIS/Device/Maxim/MAX78000/Include

# Peripheral driver headers
IPATH += $(MAXIM_PATH)/Libraries/PeriphDrivers/Include/MAX78000

# -------------------------------
# Optional debug flags
# -------------------------------
# DEBUG ?= 1
# PROJ_CFLAGS += -Wall
