################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/user_defined_folder/FLASH_MEMORY/mt25ql.cpp \
../Core/user_defined_folder/FLASH_MEMORY/mt25ql_test.cpp 

OBJS += \
./Core/user_defined_folder/FLASH_MEMORY/mt25ql.o \
./Core/user_defined_folder/FLASH_MEMORY/mt25ql_test.o 

CPP_DEPS += \
./Core/user_defined_folder/FLASH_MEMORY/mt25ql.d \
./Core/user_defined_folder/FLASH_MEMORY/mt25ql_test.d 


# Each subdirectory must supply rules for building sources it contributes
Core/user_defined_folder/FLASH_MEMORY/%.o: ../Core/user_defined_folder/FLASH_MEMORY/%.cpp Core/user_defined_folder/FLASH_MEMORY/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

