#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Src/CanParse.c Src/CanSend.c Src/ECAN1Config.c Src/ECAN1Drv.c Src/Main.c Src/Timers.c Src/traps.c Init.c Src/ADC.c Src/PWM.c Src/I2C.c Src/MPU6050.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Src/CanParse.o ${OBJECTDIR}/Src/CanSend.o ${OBJECTDIR}/Src/ECAN1Config.o ${OBJECTDIR}/Src/ECAN1Drv.o ${OBJECTDIR}/Src/Main.o ${OBJECTDIR}/Src/Timers.o ${OBJECTDIR}/Src/traps.o ${OBJECTDIR}/Init.o ${OBJECTDIR}/Src/ADC.o ${OBJECTDIR}/Src/PWM.o ${OBJECTDIR}/Src/I2C.o ${OBJECTDIR}/Src/MPU6050.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Src/CanParse.o.d ${OBJECTDIR}/Src/CanSend.o.d ${OBJECTDIR}/Src/ECAN1Config.o.d ${OBJECTDIR}/Src/ECAN1Drv.o.d ${OBJECTDIR}/Src/Main.o.d ${OBJECTDIR}/Src/Timers.o.d ${OBJECTDIR}/Src/traps.o.d ${OBJECTDIR}/Init.o.d ${OBJECTDIR}/Src/ADC.o.d ${OBJECTDIR}/Src/PWM.o.d ${OBJECTDIR}/Src/I2C.o.d ${OBJECTDIR}/Src/MPU6050.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Src/CanParse.o ${OBJECTDIR}/Src/CanSend.o ${OBJECTDIR}/Src/ECAN1Config.o ${OBJECTDIR}/Src/ECAN1Drv.o ${OBJECTDIR}/Src/Main.o ${OBJECTDIR}/Src/Timers.o ${OBJECTDIR}/Src/traps.o ${OBJECTDIR}/Init.o ${OBJECTDIR}/Src/ADC.o ${OBJECTDIR}/Src/PWM.o ${OBJECTDIR}/Src/I2C.o ${OBJECTDIR}/Src/MPU6050.o

# Source Files
SOURCEFILES=Src/CanParse.c Src/CanSend.c Src/ECAN1Config.c Src/ECAN1Drv.c Src/Main.c Src/Timers.c Src/traps.c Init.c Src/ADC.c Src/PWM.c Src/I2C.c Src/MPU6050.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ64GS608
MP_LINKER_FILE_OPTION=,--script=p33FJ64GS608.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Src/CanParse.o: Src/CanParse.c  .generated_files/flags/default/927b16d5108fb192b96bf3dd50c65c928bb7a1a2 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/CanParse.o.d 
	@${RM} ${OBJECTDIR}/Src/CanParse.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/CanParse.c  -o ${OBJECTDIR}/Src/CanParse.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/CanParse.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/CanSend.o: Src/CanSend.c  .generated_files/flags/default/315ce9d5aaf267c35517a2d95268b3aabd240d2a .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/CanSend.o.d 
	@${RM} ${OBJECTDIR}/Src/CanSend.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/CanSend.c  -o ${OBJECTDIR}/Src/CanSend.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/CanSend.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/ECAN1Config.o: Src/ECAN1Config.c  .generated_files/flags/default/c9c1a546eeb7393e23c2d6297afd4552de0d7bf8 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/ECAN1Config.o.d 
	@${RM} ${OBJECTDIR}/Src/ECAN1Config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/ECAN1Config.c  -o ${OBJECTDIR}/Src/ECAN1Config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/ECAN1Config.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/ECAN1Drv.o: Src/ECAN1Drv.c  .generated_files/flags/default/f8cab17a8fe1f6d655ae7ad73565be4169cbc675 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/ECAN1Drv.o.d 
	@${RM} ${OBJECTDIR}/Src/ECAN1Drv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/ECAN1Drv.c  -o ${OBJECTDIR}/Src/ECAN1Drv.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/ECAN1Drv.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/Main.o: Src/Main.c  .generated_files/flags/default/4eb24fbd838e007031add4334f4dde038d950673 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/Main.o.d 
	@${RM} ${OBJECTDIR}/Src/Main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/Main.c  -o ${OBJECTDIR}/Src/Main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/Main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/Timers.o: Src/Timers.c  .generated_files/flags/default/3a9feb973f6301c5b1390614f16d6172d141b413 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/Timers.o.d 
	@${RM} ${OBJECTDIR}/Src/Timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/Timers.c  -o ${OBJECTDIR}/Src/Timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/Timers.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/traps.o: Src/traps.c  .generated_files/flags/default/af2baaa35b1d70d1f5aef5fad62a7c56e13b6a6a .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/traps.o.d 
	@${RM} ${OBJECTDIR}/Src/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/traps.c  -o ${OBJECTDIR}/Src/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/traps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Init.o: Init.c  .generated_files/flags/default/170dee10b67c40b0002765cbcbcc8bb9497eb0e3 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Init.o.d 
	@${RM} ${OBJECTDIR}/Init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Init.c  -o ${OBJECTDIR}/Init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Init.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/ADC.o: Src/ADC.c  .generated_files/flags/default/21e24b3618c22b5b46e23faffb90eb75bcc26089 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/ADC.o.d 
	@${RM} ${OBJECTDIR}/Src/ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/ADC.c  -o ${OBJECTDIR}/Src/ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/ADC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/PWM.o: Src/PWM.c  .generated_files/flags/default/fc33a6d3ec8ee6c11cba2d7db36163eb0209e9b0 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/PWM.o.d 
	@${RM} ${OBJECTDIR}/Src/PWM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/PWM.c  -o ${OBJECTDIR}/Src/PWM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/PWM.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/I2C.o: Src/I2C.c  .generated_files/flags/default/44ad80e3e8b2f7e499d29ac72d1fb08cb1857eb3 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/I2C.o.d 
	@${RM} ${OBJECTDIR}/Src/I2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/I2C.c  -o ${OBJECTDIR}/Src/I2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/I2C.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/MPU6050.o: Src/MPU6050.c  .generated_files/flags/default/2c83e098230476d199d2203e23b4b7fa69beaf5f .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/MPU6050.o.d 
	@${RM} ${OBJECTDIR}/Src/MPU6050.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/MPU6050.c  -o ${OBJECTDIR}/Src/MPU6050.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/MPU6050.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
else
${OBJECTDIR}/Src/CanParse.o: Src/CanParse.c  .generated_files/flags/default/3ff4fcbf0339d46bb9a5236511679f8c698eb798 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/CanParse.o.d 
	@${RM} ${OBJECTDIR}/Src/CanParse.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/CanParse.c  -o ${OBJECTDIR}/Src/CanParse.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/CanParse.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/CanSend.o: Src/CanSend.c  .generated_files/flags/default/2f4df3a6dc91d29f1fbd6c42581e8f5340df4951 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/CanSend.o.d 
	@${RM} ${OBJECTDIR}/Src/CanSend.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/CanSend.c  -o ${OBJECTDIR}/Src/CanSend.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/CanSend.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/ECAN1Config.o: Src/ECAN1Config.c  .generated_files/flags/default/8b977416c8ee5d9841d9800708b6da149f415875 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/ECAN1Config.o.d 
	@${RM} ${OBJECTDIR}/Src/ECAN1Config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/ECAN1Config.c  -o ${OBJECTDIR}/Src/ECAN1Config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/ECAN1Config.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/ECAN1Drv.o: Src/ECAN1Drv.c  .generated_files/flags/default/186d91fff55659007cec0a1c4fe4bdd3db083889 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/ECAN1Drv.o.d 
	@${RM} ${OBJECTDIR}/Src/ECAN1Drv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/ECAN1Drv.c  -o ${OBJECTDIR}/Src/ECAN1Drv.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/ECAN1Drv.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/Main.o: Src/Main.c  .generated_files/flags/default/99cbaa832e8af41eee5c226625d3c6b2aa6e628f .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/Main.o.d 
	@${RM} ${OBJECTDIR}/Src/Main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/Main.c  -o ${OBJECTDIR}/Src/Main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/Main.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/Timers.o: Src/Timers.c  .generated_files/flags/default/15c811692dfdcf89c440064bdda34a0c3bcc3af5 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/Timers.o.d 
	@${RM} ${OBJECTDIR}/Src/Timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/Timers.c  -o ${OBJECTDIR}/Src/Timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/Timers.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/traps.o: Src/traps.c  .generated_files/flags/default/555af1dacca9aa1faff549ba8e1afa6f3118df39 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/traps.o.d 
	@${RM} ${OBJECTDIR}/Src/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/traps.c  -o ${OBJECTDIR}/Src/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/traps.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Init.o: Init.c  .generated_files/flags/default/5132ebea93b972eb7fcd89b314a4f34aa000b4b7 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Init.o.d 
	@${RM} ${OBJECTDIR}/Init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Init.c  -o ${OBJECTDIR}/Init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Init.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/ADC.o: Src/ADC.c  .generated_files/flags/default/d306eeef3188388687666b9fa6f80b601b65dd75 .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/ADC.o.d 
	@${RM} ${OBJECTDIR}/Src/ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/ADC.c  -o ${OBJECTDIR}/Src/ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/ADC.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/PWM.o: Src/PWM.c  .generated_files/flags/default/2b625377750f8d80d38db8424b5e5ae8ba10bc3a .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/PWM.o.d 
	@${RM} ${OBJECTDIR}/Src/PWM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/PWM.c  -o ${OBJECTDIR}/Src/PWM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/PWM.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/I2C.o: Src/I2C.c  .generated_files/flags/default/b773f56466ae6422132aba49d0196095b058366c .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/I2C.o.d 
	@${RM} ${OBJECTDIR}/Src/I2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/I2C.c  -o ${OBJECTDIR}/Src/I2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/I2C.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Src/MPU6050.o: Src/MPU6050.c  .generated_files/flags/default/b7ef30b9ea8ed65153e72c4e7c6a27eda5ada6ad .generated_files/flags/default/bb9c332fb0c2e171c9cb4f01b7667c80b6e23b10
	@${MKDIR} "${OBJECTDIR}/Src" 
	@${RM} ${OBJECTDIR}/Src/MPU6050.o.d 
	@${RM} ${OBJECTDIR}/Src/MPU6050.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Src/MPU6050.c  -o ${OBJECTDIR}/Src/MPU6050.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Src/MPU6050.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"h" -msmart-io=1 -Wall -msfr-warn=off   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/firmware.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
