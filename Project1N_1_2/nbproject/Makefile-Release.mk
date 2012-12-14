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
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Grammar.o \
	${OBJECTDIR}/Parser.o \
	${OBJECTDIR}/Main.o \
	${OBJECTDIR}/TreeManager.o \
	${OBJECTDIR}/SentenceParser.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-Wl,--stack,400194304
CXXFLAGS=-Wl,--stack,400194304

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/C/boost_1_52_0/stage/lib -lboost_serialization-mgw46-1_52

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk dist/Release/MinGW-Windows/project1n_1_2.exe

dist/Release/MinGW-Windows/project1n_1_2.exe: ${OBJECTFILES}
	${MKDIR} -p dist/Release/MinGW-Windows
	${LINK.cc} -static -static-libgcc -static-libstdc++ -std=gnu++0x -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/project1n_1_2 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Grammar.o: Grammar.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/boost_1_52_0 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Grammar.o Grammar.cpp

${OBJECTDIR}/Parser.o: Parser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/boost_1_52_0 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Parser.o Parser.cpp

${OBJECTDIR}/Main.o: Main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/boost_1_52_0 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Main.o Main.cpp

${OBJECTDIR}/TreeManager.o: TreeManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/boost_1_52_0 -MMD -MP -MF $@.d -o ${OBJECTDIR}/TreeManager.o TreeManager.cpp

${OBJECTDIR}/SentenceParser.o: SentenceParser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/boost_1_52_0 -MMD -MP -MF $@.d -o ${OBJECTDIR}/SentenceParser.o SentenceParser.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/MinGW-Windows/project1n_1_2.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
