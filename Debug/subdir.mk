################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CommonFunctions.c \
../StateMachineTest.c \
../acsmx.c \
../main.c 

OBJS += \
./CommonFunctions.o \
./StateMachineTest.o \
./acsmx.o \
./main.o 

C_DEPS += \
./CommonFunctions.d \
./StateMachineTest.d \
./acsmx.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


