################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/src/keymanager.c \
../common/src/mq.c 

OBJS += \
./common/src/keymanager.o \
./common/src/mq.o 

C_DEPS += \
./common/src/keymanager.d \
./common/src/mq.d 


# Each subdirectory must supply rules for building sources it contributes
common/src/%.o: ../common/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


