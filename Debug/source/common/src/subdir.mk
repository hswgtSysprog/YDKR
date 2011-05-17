################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/common/src/keymanager.c \
../source/common/src/mq.c 

OBJS += \
./source/common/src/keymanager.o \
./source/common/src/mq.o 

C_DEPS += \
./source/common/src/keymanager.d \
./source/common/src/mq.d 


# Each subdirectory must supply rules for building sources it contributes
source/common/src/%.o: ../source/common/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


