## Process this file with automake to produce Makefile.in
##
## Automake file for CEC, the Columbia Esterel Compiler
##
## (C) 2003 Stephen A. Edwards
##
## See the file "COPYING" for copying information
##

NOTANGLE = @NOTANGLE@
NOTANGLECLINES = -L'\#line %L "%F"%N'
NOWEAVE = @NOWEAVE@
NOWEAVEFLAGS = -delay -index

ANTLR = @ANTLR@

AM_CXXFLAGS = -Wall -g -O

AM_CPPFLAGS = -I../libantlr

SUFFIXES = .nw .hpp .cpp .tex .dvi .ps .pdf .sh

.nw.hpp :
	$(NOTANGLE) $(NOTANGLECLINES) -R$@ $< > $@

.nw.cpp :
	$(NOTANGLE) $(NOTANGLECLINES) -R$@ $< > $@

.nw.sh :
	$(NOTANGLE) -R$@ $< > $@

.nw.tex :
	$(NOWEAVE) $(NOWEAVEFLAGS) $< > $@

.tex.dvi :
	latex $< < /dev/null
	while grep -s 'Rerun to get cross' $*.log; \
	  do latex $< < /dev/null; done

.dvi.ps :
	dvips -K -Pcmz -t letter -z -o $@ $<

.ps.pdf :
	ps2pdf $<

.1.ps :
	groff -T ps -man $< > $@

bin_PROGRAMS = \
	cec-strlxml \
	cec-xmlstrl \
	cec-expandmodules \
	cec-dismantle \
	cec-astgrc \
	cec-grcdot \
	cec-grcpdg\
	cec-smblif \
	cec-grcsim \
	cec-grcopt \
	cec-blifutil \
	cec-pdgblif \
	cec-v5-cmain \
	cec-grcc2 \
	cec-pdgccfg\
	cec-eec\
	cec-scfgc\
	cec-grc3val \
	cec-grcbal \
	cec-vm \
	cec-vm-wrapper
#	cec-grcvhdl \
#	cec-merge-simout
#	cec-v5-vinterface
#	cec-ublaze-interface
#	cec-vvpout

bin_SCRIPTS = cec cec-balasm

man_MANS = cec.1

# Source files built from e.g., .nw files

BUILT_SOURCES = AST.sh IR.hpp IR.cpp AST.hpp AST.cpp \
	EsterelParser.hpp EsterelLexer.hpp \
	EsterelTokenTypes.hpp EsterelTreeParser.hpp \
	EsterelTreeParserTokenTypes.hpp EsterelPrinter.hpp \
	EsterelPrinter.cpp \
	ExpandModules.hpp ExpandModules.cpp \
	Dismantle.hpp Dismantle.cpp \
	ASTGRC.hpp ASTGRC.cpp \
	GRCsim.hpp GRCsim.cpp cec-grcsim.cpp \
	GRCPrinter.hpp GRCPrinter.cpp cec-grcdot.cpp \
	GRCOpt.hpp GRCOpt.cpp \
	BLIF.hpp BLIF.cpp \
	GRCC2.hpp GRCC2.cpp \
	CPrinter.hpp CPrinter.cpp \
	cec-grcc2.cpp

EXTRA_DIST = IR.nw AST.nw esterel.g staticsemantics.g EsterelPrinter.nw \
	ExpandModules.nw Dismantle.nw ASTGRC.nw GRCPrinter.nw GRCsim.nw \
	GRCPDG.nw GRCOpt.nw BLIF.nw CPrinter.nw GRCC2.nw \
	cec.in cec \
	testall.sh testblif.sh \
	AST.sh \
	IR.pdf AST.pdf ASTGRC.pdf EsterelPrinter.pdf \
	ExpandModules.pdf Dismantle.pdf ASTGRC.pdf GRCPrinter.pdf GRCsim.pdf \
	GRCPDG.pdf GRCOpt.pdf BLIF.pdf cec.pdf CPrinter.pdf GRCC2.pdf \
	EsterelParser.html EsterelGrammar.pdf \
	cec-logo.tex \
	cec-balasm \
	balvm.h \
	$(man_MANS)

# Automatically-generated files for the Esterel parser and static semantic
# analyzer

esterel_ANTLRGEN = EsterelParser.cpp EsterelParser.hpp \
	EsterelLexer.cpp EsterelLexer.hpp \
	EsterelTokenTypes.hpp

staticsemantics_ANTLRGEN = EsterelTreeParser.cpp EsterelTreeParser.hpp \
	EsterelTreeParserTokenTypes.hpp

$(esterel_ANTLRGEN) : esterel.g
	rm -f $(esterel_ANTLRGEN)
	$(ANTLR) $<

$(staticsemantics_ANTLRGEN) : staticsemantics.g
	rm -f $(staticsemantics_ANTLRGEN)
	$(ANTLR) $<

# Automatically-generated HTML documentation for the grammar

EsterelParser.html : esterel.g
	$(ANTLR) -html $<

AST.sh : AST.nw

# Automatically-generated classes for AST objects

AST.hpp AST.cpp : AST.sh
	sh AST.sh

##############################
#
# Executables begin
#

# Esterel parser and static semantic analysis: generates AST in XML form

cec_strlxml_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp cec-strlxml.cpp \
	LineAST.hpp $(esterel_ANTLRGEN) $(staticsemantics_ANTLRGEN)

cec_strlxml_LDADD = ../libantlr/libantlr.a

# Esterel pretty-printer: generates Esterel source from AST in XML form

cec-xmlstrl.cpp : EsterelPrinter.nw
	$(NOTANGLE) $(NOTANGLECLINES) -R$@ $< > $@

cec_xmlstrl_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
	EsterelPrinter.hpp EsterelPrinter.cpp cec-xmlstrl.cpp

# Expands the "run" statement by performing macro-like expansion on an AST

cec_expandmodules_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
	ExpandModules.hpp ExpandModules.cpp cec-expandmodules.cpp

# Dismantler: translates some higher-level statements (e.g., present/case)
# into lower-level statements (e.g., nested if-then-else)

cec_dismantle_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
	Dismantle.hpp Dismantle.cpp cec-dismantle.cpp

# Dismantled-AST-to-GRC format translator.  Translates the AST into a
# control-flow graph plus a selection tree.

cec-astgrc.cpp : ASTGRC.nw
	$(NOTANGLE) $(NOTANGLECLINES) -R$@ $< > $@

cec_astgrc_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
	ASTGRC.hpp ASTGRC.cpp cec-astgrc.cpp

# GRC format optimizer

cec_grcopt_SOURCES =  IR.hpp IR.cpp AST.hpp AST.cpp \
	GRCOpt.hpp GRCOpt.cpp ASTGRC.hpp ASTGRC.cpp

# Translate GRC data into dot file format (from the graphviz package)

cec-grcdot.cpp : GRCPrinter.nw
	$(NOTANGLE) $(NOTANGLECLINES) -R$@ $< > $@

cec_grcdot_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
	EsterelPrinter.cpp EsterelPrinter.hpp \
	GRCPrinter.hpp GRCPrinter.cpp cec-grcdot.cpp

# Convert the control-flow portion of the GRC graph into a
# program dependence graph (PDG)

cec-grcpdg.cpp : GRCPDG.nw
	$(NOTANGLE) $(NOTANGLECLINES) -R$@ $< > $@

cec_grcpdg_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
	cec-grcpdg.cpp

# GRC and PDG simulator

cec-grcsim.cpp : GRCsim.nw
	$(NOTANGLE) $(NOTANGLECLINES) -R$@ $< > $@

cec_grcsim_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
	GRCsim.hpp GRCsim.cpp cec-grcsim.cpp

# BLIF format utilities (debugging output, simulation, Verilog output)

cec_blifutil_SOURCES = BLIF.hpp BLIF.cpp cec-blifutil.cpp

# Circuit synthesizer: PDG-to-BLIF converter

cec_pdgblif_SOURCES =  IR.hpp IR.cpp AST.hpp AST.cpp \
	cverilog.cpp cverilog_gates.cpp cverilog.hpp prblif.cpp\
	pdg2beeblebrox.cpp pdg2beeblebrox_utils.cpp beeble_statemachine.cpp \
	pdg2beeblebrox.hpp

# State machine synthesizer

cec_smblif_SOURCES = cec-smblif.cpp

# PDG to sequential control-flow graph converter

cec_pdgccfg_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
	cec-pdgccfg.cpp

cec_eec_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
	cec-eec.cpp

cec_scfgc_SOURCES =  IR.hpp IR.cpp AST.hpp AST.cpp \
	CPrinter.hpp CPrinter.cpp cec-scfgc.cpp

# GRC to 3-valued GRC converter
cec_grc3val_SOURCES =  IR.hpp IR.cpp AST.hpp AST.cpp \
	cec-grc3val.cpp

# GRC to C translator, 2nd version

cec-grcc2.cpp : GRCC2.nw
	$(NOTANGLE) $(NOTANGLECLINES) -R$@ $< > $@

cec_grcc2_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
	CPrinter.hpp CPrinter.cpp \
	GRCC2.hpp GRCC2.cpp cec-grcc2.cpp

# GRC to BAL translator

cec_grcbal_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
	cec-grcbal.cpp GRCBAL.hpp GRCBAL.cpp

# BAL virtual machine (simulator)

cec_vm_SOURCES = cec-vm.c balvm.c balvm.h balvm-instructions.h

balvm.c : cec
	./cec --vm-only > balvm.c

# GRC to VHDL translator

#cec_grcvhdl_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
	#cec-grcvhdl.cpp  VHDLPrinter.hpp VHDLPrinter.cpp \
	#EsterelPrinter.hpp EsterelPrinter.cpp

# Undocumented programs:

cec_v5_cmain_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp CPrinter.hpp \
	CPrinter.cpp cec-v5-cmain.cpp

cec_vm_wrapper_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp CPrinter.hpp \
	CPrinter.cpp cec-vm-wrapper.cpp

#cec_vvpout_SOURCES = vvp2out.cpp
#cec_merge_simout_SOURCES = merge_simout.cpp
#cec_v5_vinterface_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
#	cverilog.cpp cverilog_gates.cpp cverilog.hpp prblif.cpp\
#	v5_vinterface.cpp
#cec_ublaze_interface_SOURCES = IR.hpp IR.cpp AST.hpp AST.cpp \
#	ublaze_interface.cpp

# 
# Executables end
#
##############################

TESTS = testblif.sh testall.sh 

CLEANFILES = *.aux *.dvi *.log *.toc *.ps AST.sh clean-test

MAINTAINERCLEANFILES = $(BUILT_SOURCES) $(esterel_ANTLRGEN) \
	$(staticsemantics_ANTLRGEN) EsterelParser.html

.PHONY : clean-test
clean-test :
	rm -f *.ast.xml \
	    *.ast.strl \
	    *.ast.diff \
	    *.exp.xml \
	    *.exp.strl \
	    *.exp.diff \
	    *.dis.xml \
	    *.dis.strl \
	    *.dis.diff \
	    *.grc.xml \
	    *.grc.simout \
	    *.grc.sim.diff \
	    *.grc.dot \
	    *.grc.ps \
	    *.opt.xml \
	    *.opt.simout \
	    *.opt.sim.diff \
	    *.opt.dot \
	    *.opt.ps \
	    *.pdg.xml \
	    *.pdg.simout \
	    *.pdg.sim.diff \
	    *.pdg.dot \
	    *.pdg.ps \
	    *.pdg.blif \
	    *.sm \
	    *.core.blif \
	    *.sm.blif \
	    *.out.blif \
	    *.top.v \
	    *.blif.simout \
	    *.blif.sim.diff \
	    *.c \
	    *.c.sim.diff \
	    *.c.simout \
	    *.exe


EsterelGrammar.tex : EsterelParser.html
	echo "\documentclass{article}\title{Esterel Grammar}\date{}\begin{document}\maketitle\begin{verbatim}" > EsterelGrammar.tex
	sed -n '/<PRE>/,/<.PRE>/ p' EsterelParser.html | sed 's/<[^>]*>//g' | sed 's/	/  /g' | sed 's/\&quot\;/"/g' >> EsterelGrammar.tex
	echo "\end{verbatim}" >> EsterelGrammar.tex
	echo "\end{document}" >> EsterelGrammar.tex
