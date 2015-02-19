################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dedup/Dictionary.c \
../Dedup/DictionaryReader.c \
../Dedup/bloom.c \
../Dedup/hash.c 

OBJS += \
./Dedup/Dictionary.o \
./Dedup/DictionaryReader.o \
./Dedup/bloom.o \
./Dedup/hash.o 

C_DEPS += \
./Dedup/Dictionary.d \
./Dedup/DictionaryReader.d \
./Dedup/bloom.d \
./Dedup/hash.d 


# Each subdirectory must supply rules for building sources it contributes
Dedup/%.o: ../Dedup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


