################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../server/src/YDKR-Server.c 

OBJS += \
./server/src/YDKR-Server.o 

C_DEPS += \
./server/src/YDKR-Server.d 


# Each subdirectory must supply rules for building sources it contributes
server/src/%.o: ../server/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


