################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/muParserX/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
.DEFAULT_GOAL = all

# All Target
all: pre QBill post

pre:
	@echo ""
	@echo "################################"
	@echo "#                              #"
	@echo "#        Building QBill        #"
	@echo "#                              #"
	@echo "################################"
	@echo ""
	@test -d bin/ || mkdir -p bin/
	@cp -rp ../../App_Folder/Resources bin


post:
	@echo ""
	@echo "####################"
	@echo "#                  #"
	@echo "#       Done.      #"
	@echo "#                  #"
	@echo "####################"
	@echo ""

# Tool invocations
QBill: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "bin/QBill" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) QBill
	-@echo ' '
	@rm -rf bin

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
