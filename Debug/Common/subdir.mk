################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Common/PatternTable.c \
../Common/StateTable.c 

OBJS += \
./Common/PatternTable.o \
./Common/StateTable.o 

C_DEPS += \
./Common/PatternTable.d \
./Common/StateTable.d 


# Each subdirectory must supply rules for building sources it contributes
Common/%.o: ../Common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


