################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DumpReader/BoundedBuffer/LinkedList.c \
../DumpReader/BoundedBuffer/PacketReader.c \
../DumpReader/BoundedBuffer/Scanner.c 

OBJS += \
./DumpReader/BoundedBuffer/LinkedList.o \
./DumpReader/BoundedBuffer/PacketReader.o \
./DumpReader/BoundedBuffer/Scanner.o 

C_DEPS += \
./DumpReader/BoundedBuffer/LinkedList.d \
./DumpReader/BoundedBuffer/PacketReader.d \
./DumpReader/BoundedBuffer/Scanner.d 


# Each subdirectory must supply rules for building sources it contributes
DumpReader/BoundedBuffer/%.o: ../DumpReader/BoundedBuffer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


