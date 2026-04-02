################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/a_sensor.c \
../Core/Src/analog_sensor.c \
../Core/Src/bms.c \
../Core/Src/imu.c \
../Core/Src/main.c \
../Core/Src/motor_temp.c \
../Core/Src/sd_diskio_spi.c \
../Core/Src/sd_functions.c \
../Core/Src/sd_spi.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c 

OBJS += \
./Core/Src/a_sensor.o \
./Core/Src/analog_sensor.o \
./Core/Src/bms.o \
./Core/Src/imu.o \
./Core/Src/main.o \
./Core/Src/motor_temp.o \
./Core/Src/sd_diskio_spi.o \
./Core/Src/sd_functions.o \
./Core/Src/sd_spi.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o 

C_DEPS += \
./Core/Src/a_sensor.d \
./Core/Src/analog_sensor.d \
./Core/Src/bms.d \
./Core/Src/imu.d \
./Core/Src/main.d \
./Core/Src/motor_temp.d \
./Core/Src/sd_diskio_spi.d \
./Core/Src/sd_functions.d \
./Core/Src/sd_spi.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G491xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/a_sensor.cyclo ./Core/Src/a_sensor.d ./Core/Src/a_sensor.o ./Core/Src/a_sensor.su ./Core/Src/analog_sensor.cyclo ./Core/Src/analog_sensor.d ./Core/Src/analog_sensor.o ./Core/Src/analog_sensor.su ./Core/Src/bms.cyclo ./Core/Src/bms.d ./Core/Src/bms.o ./Core/Src/bms.su ./Core/Src/imu.cyclo ./Core/Src/imu.d ./Core/Src/imu.o ./Core/Src/imu.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/motor_temp.cyclo ./Core/Src/motor_temp.d ./Core/Src/motor_temp.o ./Core/Src/motor_temp.su ./Core/Src/sd_diskio_spi.cyclo ./Core/Src/sd_diskio_spi.d ./Core/Src/sd_diskio_spi.o ./Core/Src/sd_diskio_spi.su ./Core/Src/sd_functions.cyclo ./Core/Src/sd_functions.d ./Core/Src/sd_functions.o ./Core/Src/sd_functions.su ./Core/Src/sd_spi.cyclo ./Core/Src/sd_spi.d ./Core/Src/sd_spi.o ./Core/Src/sd_spi.su ./Core/Src/stm32g4xx_hal_msp.cyclo ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_hal_msp.su ./Core/Src/stm32g4xx_it.cyclo ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.cyclo ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su

.PHONY: clean-Core-2f-Src

