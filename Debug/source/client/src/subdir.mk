################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/client/src/client.c \
../source/client/src/command.c \
../source/client/src/gui.c 

OBJS += \
./source/client/src/client.o \
./source/client/src/command.o \
./source/client/src/gui.o 

C_DEPS += \
./source/client/src/client.d \
./source/client/src/command.d \
./source/client/src/gui.d 


# Each subdirectory must supply rules for building sources it contributes
source/client/src/%.o: ../source/client/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


