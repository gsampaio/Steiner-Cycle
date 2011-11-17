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
# User lib includes
###############################################################################

##!!!! COLOQUE AQUI O DIRETORIO DE SEU CODIGO
DIRS = RA073177

OBJLIBS = $(addprefix lib, $(addsuffix .a, $(DIRS)))

###############################################################################
# The user source files and defines
###############################################################################
EXE = RA073177.e

SOURCES = steiner_cycle_solver.cpp main.cpp
OBJS = $(SOURCES:.cpp=.o)

GV = *.gv
SVG = *.svg

###############################################################################
# Lib and include definitions
###############################################################################

include Makefile.inc

###############################################################################
# Build Rules
###############################################################################

all: $(EXE)

.PHONY: all strip clean doc docclean depclean $(DIRS)
.SUFFIXES: .cpp .o

$(DIRS):
	@echo "|---------------------[ Entering $@ ]---------------------|"
	make -C $@

$(EXE): $(DIRS) $(OBJS)
	@echo
	@echo "--> Linking objects... "
	$(CXX) $(CXXFLAGS) $(OBJS) $(OBJLIBS) -o $@ $(LIBDIRS) $(LIBS)
	@echo

.cpp.o:
	@echo "--> Compiling $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(USER_DEFINES) -c $< -o $@
	@echo

strip: $(EXE)
	@echo "-->Stripping $<..."
	strip $<
	@echo

doc:
	doxygen Doxyfile

clean:
	@echo "--> Cleaning compiled..."
	$(RM) -rf $(EXE) $(OBJS) $(OBJLIBS) $(GV) $(SVG)
	for dir in $(DIRS); do \
		make -C $$dir clean; \
	done
	
docclean:
	@echo "--> Cleaning doc..."
	$(RM) -rf doc
