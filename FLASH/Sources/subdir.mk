################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Bluetooth.c" \
"../Sources/Exceptions.c" \
"../Sources/GY953.c" \
"../Sources/IMUUpdate.c" \
"../Sources/IntcInterrupts.c" \
"../Sources/MPU9250.c" \
"../Sources/UART.c" \
"../Sources/analyse.c" \
"../Sources/angle.c" \
"../Sources/control.c" \
"../Sources/debug.c" \
"../Sources/init.c" \
"../Sources/ivor_branch_table.c" \
"../Sources/lcd.c" \
"../Sources/main.c" \
"../Sources/matrix.c" \
"../Sources/stepmotor.c" \

C_SRCS += \
../Sources/Bluetooth.c \
../Sources/Exceptions.c \
../Sources/GY953.c \
../Sources/IMUUpdate.c \
../Sources/IntcInterrupts.c \
../Sources/MPU9250.c \
../Sources/UART.c \
../Sources/analyse.c \
../Sources/angle.c \
../Sources/control.c \
../Sources/debug.c \
../Sources/init.c \
../Sources/ivor_branch_table.c \
../Sources/lcd.c \
../Sources/main.c \
../Sources/matrix.c \
../Sources/stepmotor.c \

OBJS += \
./Sources/Bluetooth_c.obj \
./Sources/Exceptions_c.obj \
./Sources/GY953_c.obj \
./Sources/IMUUpdate_c.obj \
./Sources/IntcInterrupts_c.obj \
./Sources/MPU9250_c.obj \
./Sources/UART_c.obj \
./Sources/analyse_c.obj \
./Sources/angle_c.obj \
./Sources/control_c.obj \
./Sources/debug_c.obj \
./Sources/init_c.obj \
./Sources/ivor_branch_table_c.obj \
./Sources/lcd_c.obj \
./Sources/main_c.obj \
./Sources/matrix_c.obj \
./Sources/stepmotor_c.obj \

OBJS_QUOTED += \
"./Sources/Bluetooth_c.obj" \
"./Sources/Exceptions_c.obj" \
"./Sources/GY953_c.obj" \
"./Sources/IMUUpdate_c.obj" \
"./Sources/IntcInterrupts_c.obj" \
"./Sources/MPU9250_c.obj" \
"./Sources/UART_c.obj" \
"./Sources/analyse_c.obj" \
"./Sources/angle_c.obj" \
"./Sources/control_c.obj" \
"./Sources/debug_c.obj" \
"./Sources/init_c.obj" \
"./Sources/ivor_branch_table_c.obj" \
"./Sources/lcd_c.obj" \
"./Sources/main_c.obj" \
"./Sources/matrix_c.obj" \
"./Sources/stepmotor_c.obj" \

C_DEPS += \
./Sources/Bluetooth_c.d \
./Sources/Exceptions_c.d \
./Sources/GY953_c.d \
./Sources/IMUUpdate_c.d \
./Sources/IntcInterrupts_c.d \
./Sources/MPU9250_c.d \
./Sources/UART_c.d \
./Sources/analyse_c.d \
./Sources/angle_c.d \
./Sources/control_c.d \
./Sources/debug_c.d \
./Sources/init_c.d \
./Sources/ivor_branch_table_c.d \
./Sources/lcd_c.d \
./Sources/main_c.d \
./Sources/matrix_c.d \
./Sources/stepmotor_c.d \

OBJS_OS_FORMAT += \
./Sources/Bluetooth_c.obj \
./Sources/Exceptions_c.obj \
./Sources/GY953_c.obj \
./Sources/IMUUpdate_c.obj \
./Sources/IntcInterrupts_c.obj \
./Sources/MPU9250_c.obj \
./Sources/UART_c.obj \
./Sources/analyse_c.obj \
./Sources/angle_c.obj \
./Sources/control_c.obj \
./Sources/debug_c.obj \
./Sources/init_c.obj \
./Sources/ivor_branch_table_c.obj \
./Sources/lcd_c.obj \
./Sources/main_c.obj \
./Sources/matrix_c.obj \
./Sources/stepmotor_c.obj \

C_DEPS_QUOTED += \
"./Sources/Bluetooth_c.d" \
"./Sources/Exceptions_c.d" \
"./Sources/GY953_c.d" \
"./Sources/IMUUpdate_c.d" \
"./Sources/IntcInterrupts_c.d" \
"./Sources/MPU9250_c.d" \
"./Sources/UART_c.d" \
"./Sources/analyse_c.d" \
"./Sources/angle_c.d" \
"./Sources/control_c.d" \
"./Sources/debug_c.d" \
"./Sources/init_c.d" \
"./Sources/ivor_branch_table_c.d" \
"./Sources/lcd_c.d" \
"./Sources/main_c.d" \
"./Sources/matrix_c.d" \
"./Sources/stepmotor_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Bluetooth_c.obj: ../Sources/Bluetooth.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Bluetooth.args" -o "Sources/Bluetooth_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Exceptions_c.obj: ../Sources/Exceptions.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Exceptions.args" -o "Sources/Exceptions_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/GY953_c.obj: ../Sources/GY953.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/GY953.args" -o "Sources/GY953_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/IMUUpdate_c.obj: ../Sources/IMUUpdate.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/IMUUpdate.args" -o "Sources/IMUUpdate_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/IntcInterrupts_c.obj: ../Sources/IntcInterrupts.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/IntcInterrupts.args" -o "Sources/IntcInterrupts_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/MPU9250_c.obj: ../Sources/MPU9250.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/MPU9250.args" -o "Sources/MPU9250_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/UART_c.obj: ../Sources/UART.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/UART.args" -o "Sources/UART_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/analyse_c.obj: ../Sources/analyse.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/analyse.args" -o "Sources/analyse_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/angle_c.obj: ../Sources/angle.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/angle.args" -o "Sources/angle_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/control_c.obj: ../Sources/control.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/control.args" -o "Sources/control_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/debug_c.obj: ../Sources/debug.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/debug.args" -o "Sources/debug_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/init_c.obj: ../Sources/init.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/init.args" -o "Sources/init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/ivor_branch_table_c.obj: ../Sources/ivor_branch_table.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/ivor_branch_table.args" -o "Sources/ivor_branch_table_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/lcd_c.obj: ../Sources/lcd.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/lcd.args" -o "Sources/lcd_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/main.args" -o "Sources/main_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/matrix_c.obj: ../Sources/matrix.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/matrix.args" -o "Sources/matrix_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/stepmotor_c.obj: ../Sources/stepmotor.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/stepmotor.args" -o "Sources/stepmotor_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


