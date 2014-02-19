################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/carlos/Documentos/Programacion/QBill/src/ConfigParser.cpp \
/home/carlos/Documentos/Programacion/QBill/src/CsvParser.cpp \
/home/carlos/Documentos/Programacion/QBill/src/CutBill.cpp \
/home/carlos/Documentos/Programacion/QBill/src/QCutBill.cpp \
/home/carlos/Documentos/Programacion/QBill/src/StringFuncs.cpp \
/home/carlos/Documentos/Programacion/QBill/src/main.cpp 

OBJS += \
./src/ConfigParser.o \
./src/CsvParser.o \
./src/CutBill.o \
./src/QCutBill.o \
./src/StringFuncs.o \
./src/main.o 

CPP_DEPS += \
./src/ConfigParser.d \
./src/CsvParser.d \
./src/CutBill.d \
./src/QCutBill.d \
./src/StringFuncs.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/ConfigParser.o: /home/carlos/Documentos/Programacion/QBill/src/ConfigParser.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUNICODE -D_UNICODE -O0 -g3 -w -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/CsvParser.o: /home/carlos/Documentos/Programacion/QBill/src/CsvParser.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUNICODE -D_UNICODE -O0 -g3 -w -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/CutBill.o: /home/carlos/Documentos/Programacion/QBill/src/CutBill.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUNICODE -D_UNICODE -O0 -g3 -w -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/QCutBill.o: /home/carlos/Documentos/Programacion/QBill/src/QCutBill.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUNICODE -D_UNICODE -O0 -g3 -w -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/StringFuncs.o: /home/carlos/Documentos/Programacion/QBill/src/StringFuncs.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUNICODE -D_UNICODE -O0 -g3 -w -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/main.o: /home/carlos/Documentos/Programacion/QBill/src/main.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUNICODE -D_UNICODE -O0 -g3 -w -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


