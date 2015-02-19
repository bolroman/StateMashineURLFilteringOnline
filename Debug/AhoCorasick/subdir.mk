################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AhoCorasick/ACBuilder.c \
../AhoCorasick/ACBuilder_old1.c \
../AhoCorasick/FindLP.c \
../AhoCorasick/NodeQueue.c 

OBJS += \
./AhoCorasick/ACBuilder.o \
./AhoCorasick/ACBuilder_old1.o \
./AhoCorasick/FindLP.o \
./AhoCorasick/NodeQueue.o 

C_DEPS += \
./AhoCorasick/ACBuilder.d \
./AhoCorasick/ACBuilder_old1.d \
./AhoCorasick/FindLP.d \
./AhoCorasick/NodeQueue.d 


# Each subdirectory must supply rules for building sources it contributes
AhoCorasick/%.o: ../AhoCorasick/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


