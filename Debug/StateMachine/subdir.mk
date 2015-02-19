################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../StateMachine/StateGenerator.c \
../StateMachine/StateMachine.c \
../StateMachine/StateMachineDumper.c \
../StateMachine/StateMachineGenerator.c \
../StateMachine/TableStateMachine.c \
../StateMachine/TableStateMachineGenerator.c 

OBJS += \
./StateMachine/StateGenerator.o \
./StateMachine/StateMachine.o \
./StateMachine/StateMachineDumper.o \
./StateMachine/StateMachineGenerator.o \
./StateMachine/TableStateMachine.o \
./StateMachine/TableStateMachineGenerator.o 

C_DEPS += \
./StateMachine/StateGenerator.d \
./StateMachine/StateMachine.d \
./StateMachine/StateMachineDumper.d \
./StateMachine/StateMachineGenerator.d \
./StateMachine/TableStateMachine.d \
./StateMachine/TableStateMachineGenerator.d 


# Each subdirectory must supply rules for building sources it contributes
StateMachine/%.o: ../StateMachine/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


