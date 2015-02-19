################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Common/BitArray/BitArray.c 

OBJS += \
./Common/BitArray/BitArray.o 

C_DEPS += \
./Common/BitArray/BitArray.d 


# Each subdirectory must supply rules for building sources it contributes
Common/BitArray/%.o: ../Common/BitArray/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


