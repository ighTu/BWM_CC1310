################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-arm_16.12.0.STS

GEN_OPTS__FLAG := --cmd_file="configPkg/compiler.opt"
GEN_CMDS__FLAG := -l"configPkg/linker.cmd"

ORDERED_OBJS += \
"./CC1310_LAUNCHXL.obj" \
"./ccfg.obj" \
"./main.obj" \
"./XI_Bios/bios.obj" \
"./XI_Bios/bios_CC1310.obj" \
"./XI_Bios/nokia_5110_lcd.obj" \
"./XI_Radio/radio.obj" \
"./XI_Radio/rfPacketTx.obj" \
"./XI_Sensors/bh1750.obj" \
"./XI_Sensors/bme280.obj" \
"./XI_Sensors/bmp280.obj" \
"./XI_Sensors/mpu9250.obj" \
"./XI_Sensors/sensors.obj" \
"./XI_Sensors/tmp007.obj" \
"./smartrf_settings/smartrf_settings.obj" \
"../CC1310_LAUNCHXL.cmd" \
$(GEN_CMDS__FLAG) \
-l"C:/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/cc13xxware_2_04_03_17272/driverlib/bin/ccs/driverlib.lib" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include XI_Bios/subdir_vars.mk
-include XI_Radio/subdir_vars.mk
-include XI_Sensors/subdir_vars.mk
-include smartrf_settings/subdir_vars.mk
-include subdir_rules.mk
-include XI_Bios/subdir_rules.mk
-include XI_Radio/subdir_rules.mk
-include XI_Sensors/subdir_rules.mk
-include smartrf_settings/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
bwm_CC1310_LAUNCHXL_TI_1.1.out \

EXE_OUTPUTS__QUOTED += \
"bwm_CC1310_LAUNCHXL_TI_1.1.out" \

BIN_OUTPUTS += \
bwm_CC1310_LAUNCHXL_TI_1.1.hex \

BIN_OUTPUTS__QUOTED += \
"bwm_CC1310_LAUNCHXL_TI_1.1.hex" \


# All Target
all: bwm_CC1310_LAUNCHXL_TI_1.1.out

# Tool invocations
bwm_CC1310_LAUNCHXL_TI_1.1.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_16.12.0.STS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi -z -m"bwm_CC1310_LAUNCHXL_TI_1.1.map" --heap_size=0 --stack_size=256 -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_16.12.0.STS/lib" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_16.12.0.STS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="bwm_CC1310_LAUNCHXL_TI_1.1_linkInfo.xml" --rom_model -o "bwm_CC1310_LAUNCHXL_TI_1.1.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

bwm_CC1310_LAUNCHXL_TI_1.1.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_16.12.0.STS/bin/armhex" --memwidth=8 --romwidth=8 --intel -o "bwm_CC1310_LAUNCHXL_TI_1.1.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_OPTS__QUOTED)$(GEN_CMDS__QUOTED)
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-$(RM) "CC1310_LAUNCHXL.d" "ccfg.d" "main.d" "XI_Bios\bios.d" "XI_Bios\bios_CC1310.d" "XI_Bios\nokia_5110_lcd.d" "XI_Radio\radio.d" "XI_Radio\rfPacketTx.d" "XI_Sensors\bh1750.d" "XI_Sensors\bme280.d" "XI_Sensors\bmp280.d" "XI_Sensors\mpu9250.d" "XI_Sensors\sensors.d" "XI_Sensors\tmp007.d" "smartrf_settings\smartrf_settings.d" 
	-$(RM) "CC1310_LAUNCHXL.obj" "ccfg.obj" "main.obj" "XI_Bios\bios.obj" "XI_Bios\bios_CC1310.obj" "XI_Bios\nokia_5110_lcd.obj" "XI_Radio\radio.obj" "XI_Radio\rfPacketTx.obj" "XI_Sensors\bh1750.obj" "XI_Sensors\bme280.obj" "XI_Sensors\bmp280.obj" "XI_Sensors\mpu9250.obj" "XI_Sensors\sensors.obj" "XI_Sensors\tmp007.obj" "smartrf_settings\smartrf_settings.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

