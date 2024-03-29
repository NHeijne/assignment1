#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc.exe
CCC=g++.exe
CXX=g++.exe
FC=
AS=as.exe

# Macros
CND_PLATFORM=MinGW-Windows
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/CYKParser.o \
	${OBJECTDIR}/Grammar.o \
	${OBJECTDIR}/Main.o \
	${OBJECTDIR}/TreeManager.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-Wl,--stack,20011111 -Wl,--heap=300011111
CXXFLAGS=-Wl,--stack,20011111 -Wl,--heap=300011111

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/C/boost_1_52_0/stage/lib -lboost_serialization-mgw46-d-1_52

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/MinGW-Windows/project1n.exe

dist/Debug/MinGW-Windows/project1n.exe: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/MinGW-Windows
	${LINK.cc} -static-libgcc -static -std=gnu++0x -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/project1n ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/CYKParser.o: CYKParser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/C/boost_1_52_0 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CYKParser.o CYKParser.cpp

${OBJECTDIR}/Grammar.o: Grammar.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/C/boost_1_52_0 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Grammar.o Grammar.cpp

${OBJECTDIR}/Main.o: Main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/C/boost_1_52_0 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Main.o Main.cpp

${OBJECTDIR}/TreeManager.o: TreeManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/C/boost_1_52_0 -MMD -MP -MF $@.d -o ${OBJECTDIR}/TreeManager.o TreeManager.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/MinGW-Windows/project1n.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
