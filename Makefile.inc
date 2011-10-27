###############################################################################
# Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
#
# (c) Copyright 2011 Institute of Computing, University of Campinas.
#     All Rights Reserved.
#
#  Created on : Oct 01, 2011 by andrade
#  Last update: Oct 02, 2011 by andrade
#
# This software is licensed under the Common Public License. Please see
# accompanying file for terms.
###############################################################################

###############################################################################
# User Flags
###############################################################################

# Compiler flags
# Uncomment follow line to enable debugging symbols

USER_FLAGS = -g3
USER_FLAGS += -Wall -Wextra -pedantic -fexceptions -O3

##############################
# Cplex and Concert settings
##############################

# For Linux
#SYSTEM = x86_sles10_4.1
#SYSTEM = x86-64_sles10_4.1

# For Mac OSX
#SYSTEM = x86-64_darwin9_gcc4.0

#LIBFORMAT = static_pic

#ILOG_DIR = /Users/carlos/ILOG/CPLEX

#CPLEXDIR = $(ILOG_DIR)/cplex
#CONCERTDIR = $(ILOG_DIR)/concert

#CPLEXINCDIRS = \
#	-I$(CPLEXDIR)/include \
#	-I$(CONCERTDIR)/include

#CPLEXLIBDIRS = \
#	-L$(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT) \
#	-L$(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

#CPLEXLIBS = -lilocplex -lcplex -lconcert -lm -lpthread

# Manntory for Mac OSX
#CPLEXLIBS += -framework CoreFoundation -framework IOKit

# Debug flags
#CPLEXFLAGS = -fPIC -fexceptions -DIL_STD -pthread #-DNDEBUG

# x86
#CPLEXFLAGS = -m32 -pthread -O -fPIC -fexceptions -DIL_STD -DNDEBUG

# x86-64
#CPLEXFLAGS = -m64 -pthread -fPIC -fexceptions -DIL_STD -DNDEBUG
	
# Compiler flags
#USER_FLAGS += $(CPLEXFLAGS)
	
###############################################################################
# Compiler defs
###############################################################################

# C++ Compiler command
CXX = g++

# C++ Compiler options
CXXFLAGS = $(USER_FLAGS)
CFLAGS = $(USER_FLAGS)

# Necessary Include dirs
INCLUDES = $(CPLEXINCDIRS)

# Necessary library dirs
LIBDIRS = $(CPLEXLIBDIRS)

# Libraries necessary to link.
LIBS = $(CPLEXLIBS) -lemon

###############################################################################
# Other defs
###############################################################################

AR = ar
ARFLAGS	=
RANLIB = ranlib
RM = rm
SHELL = /bin/bash
