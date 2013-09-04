################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/carlos/Documentos/Programacion/QBill/src/CutBill.cpp \
/home/carlos/Documentos/Programacion/QBill/src/Palette.cpp \
/home/carlos/Documentos/Programacion/QBill/src/QCutBill.cpp \
/home/carlos/Documentos/Programacion/QBill/src/gradient.cpp \
/home/carlos/Documentos/Programacion/QBill/src/main.cpp 

OBJS += \
./src/CutBill.o \
./src/Palette.o \
./src/QCutBill.o \
./src/gradient.o \
./src/main.o 

CPP_DEPS += \
./src/CutBill.d \
./src/Palette.d \
./src/QCutBill.d \
./src/gradient.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/CutBill.o: /home/carlos/Documentos/Programacion/QBill/src/CutBill.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUNICODE -D_UNICODE -I/usr/include/wx-2.8 -I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-release-2.8 -O0 -g3 -w -c -fmessage-length=0 `wx-config --cxxflags` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Palette.o: /home/carlos/Documentos/Programacion/QBill/src/Palette.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUNICODE -D_UNICODE -I/usr/include/wx-2.8 -I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-release-2.8 -O0 -g3 -w -c -fmessage-length=0 `wx-config --cxxflags` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/QCutBill.o: /home/carlos/Documentos/Programacion/QBill/src/QCutBill.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUNICODE -D_UNICODE -I/usr/include/wx-2.8 -I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-release-2.8 -O0 -g3 -w -c -fmessage-length=0 `wx-config --cxxflags` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gradient.o: /home/carlos/Documentos/Programacion/QBill/src/gradient.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUNICODE -D_UNICODE -I/usr/include/wx-2.8 -I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-release-2.8 -O0 -g3 -w -c -fmessage-length=0 `wx-config --cxxflags` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/main.o: /home/carlos/Documentos/Programacion/QBill/src/main.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DUNICODE -D_UNICODE -I/usr/include/wx-2.8 -I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-release-2.8 -O0 -g3 -w -c -fmessage-length=0 `wx-config --cxxflags` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


