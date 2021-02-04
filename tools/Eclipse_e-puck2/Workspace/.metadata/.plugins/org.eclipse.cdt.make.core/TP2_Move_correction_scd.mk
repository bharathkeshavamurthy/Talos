# This is a generated file. Please do not edit.

.PHONY: all

COMMANDS := 	\
	    scd_cmd_1

all: $(COMMANDS)

scd_cmd_1:
	@echo begin generating scanner info for $@
	arm-none-eabi-gcc -mcpu=cortex-m4 -O0 -D 'STM32F4=1' -D 'STM32F407xx=1' -mthumb -mno-thumb-interwork -I "Y:\Documents\SmartGit\e-puck2-tps\TP2_move\src" -I "Y:\Documents\SmartGit\e-puck2-tps\TP2_move\ST" -E -P -v -dD specs.c
	@echo end generating scanner info for $@


