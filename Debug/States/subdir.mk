################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../States/BitmapEncodedState.c \
../States/LinearEncodedState.c \
../States/LookupTableState.c \
../States/PathCompressedState.c \
../States/SimpleLinearEncodedState.c 

OBJS += \
./States/BitmapEncodedState.o \
./States/LinearEncodedState.o \
./States/LookupTableState.o \
./States/PathCompressedState.o \
./States/SimpleLinearEncodedState.o 

C_DEPS += \
./States/BitmapEncodedState.d \
./States/LinearEncodedState.d \
./States/LookupTableState.d \
./States/PathCompressedState.d \
./States/SimpleLinearEncodedState.d 


# Each subdirectory must supply rules for building sources it contributes
States/%.o: ../States/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


