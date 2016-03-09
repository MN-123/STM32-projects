################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/_write.c \
../src/main.c \
../src/tm_stm32f10_fonts.c \
../src/tm_stm32f10_i2c.c \
../src/tm_stm32f10_ssd1306.c 

OBJS += \
./src/_write.o \
./src/main.o \
./src/tm_stm32f10_fonts.o \
./src/tm_stm32f10_i2c.o \
./src/tm_stm32f10_ssd1306.o 

C_DEPS += \
./src/_write.d \
./src/main.d \
./src/tm_stm32f10_fonts.d \
./src/tm_stm32f10_i2c.d \
./src/tm_stm32f10_ssd1306.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_STDOUT -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


