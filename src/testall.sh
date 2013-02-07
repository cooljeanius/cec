#!/bin/sh

# Comprehensive testing system for CEC
# Runs the parser, module expander, dismantler, GRC generator, dependency
# generator, GRC optimizer, PDG generator, and circuit generator.
#
# Most results are compared to reference files, and many are run through
# either the GRC or BLIF simulator before checking.

STRLXML=./cec-strlxml
XMLSTRL=./cec-xmlstrl
EXPAND=./cec-expandmodules
DISMANTLE=./cec-dismantle
ASTGRC="./cec-astgrc"
GRCSIM=./cec-grcsim
GRCDOT=./cec-grcdot
GRCOPT=./cec-grcopt
GRCPDG=./cec-grcpdg
PDGBLIF=./cec-pdgblif
SMBLIF=./cec-smblif
BLIFSIM=./cec-blifutil
BLIFDOT="./cec-blifutil -d"
GRCC=./cec-grcc
GRCC2=./cec-grcc2
V5CMAIN=./cec-v5-cmain
GRCVHDL=./cec-grcvhdl
PDGCCFG=./cec-pdgccfg
EEC=./cec-eec
SCFGC=./cec-scfgc
CC=cc
GRCBAL=./cec-grcbal
BALASM=./cec-balasm
VMWRAPPER=./cec-vm-wrapper
VM=./cec-vm

DUMPBALVMC="./cec --vm-only"

CCARGS=-Wall

# Set time limit to 40 seconds on all operations
# ulimit -t 40

globallog=testall.log
rm -f $globallog
error=0
globalerror=0

keep=0
dot=0
ps=0
astgrcargs=

Usage() {
    echo "Usage: testall.sh [options] [.strl files]"
    echo "-k    Keep intermediate files"
    echo "-d    Generate .dot files for intermediate representations"
    echo "-p    Generate .ps files for intermediate representations"
    echo "-s    Do not do schizophrenic expansion in astgrc"
    echo "-h    Print this help"
    exit 1
}

# echo requires some testing to see whether it accepts the -n option
case `echo "testing\c"; echo 1,2,3`,`echo -n testing; echo 1,2,3` in
  *c*,-n*) ECHO_N= ECHO_C='
' ECHO_T='      ' ;;
  *c*,*  ) ECHO_N=-n ECHO_C= ECHO_T= ;;
  *)       ECHO_N= ECHO_C='\c' ECHO_T= ;;
esac

SignalError() {
    if [ $error -eq 0 ] ; then
	echo "FAILED"
	error=1
    fi
    echo "  $1"
}

# Compare <outfile> <reffile1> <reffile2> <difffile>
# Compares the outfile with reffile1 if reffile1 exists, otherwise,
# compares the outfile with reffile2.  Differences, if any, written to difffile
Compare() {
    generatedfiles="$generatedfiles $4"
    if [ -f "$2" ] ; then
	echo diff -b $1 $2 ">" $4 1>&2
	diff -b "$1" "$2" > "$4" 2>&1 || {
	    SignalError "$1 differs"
	    echo "FAILED $1 differs from $2" 1>&2
	}
    elif [ -f "$3" ] ; then
	echo diff -b $1 $3 ">" $4 1>&2
	diff -b "$1" "$3" > "$4" 2>&1 || {
	    SignalError "$1 differs"
	    echo "FAILED $1 differs from $3" 1>&2
	}
    else
	SignalError "$3 missing"
	echo "FAILED: $3 missing" 1>&2
    fi
}

# Run <args>
# Report the command, run it, and report any errors
Run() {
    echo $* 1>&2
    eval $* || {
	SignalError "$1 failed on $*"
	return 1
    }
}

# RunPass <executable> <infile> <outfile>
# Remembers that the <outfile> was generated
RunPass() {
    generatedfiles="$generatedfiles $3"
    Run $1 "<" $2 ">" $3
}

# RunGRCSIM <refname> <basename> <extension>
RunGRCSIM() {
    generatedfiles="$generatedfiles $2.$3.simout"
    if [ -f $1.tv ] ; then
	Run $GRCSIM -t $1.tv "<" $2.$3.xml ">" $2.$3.simout
    else
	Run $GRCSIM -c `cat ${reffile}.simout | wc -l` "<" $2.$3.xml ">" $2.$3.simout
    fi &&
    Compare $2.$3.simout $1.simout $1.simout \
	$2.$3.sim.diff
}

Check() {
    error=0
    basename=`echo $1 | sed 's/.*\\///
                             s/.strl//'`
    reffile=`echo $1 | sed 's/.strl$//'`
    basedir="`echo $1 | sed 's/\/[^\/]*$//'`/."

    echo $ECHO_N "$basename...$ECHO_C"

    echo 1>&2
    echo "###### Testing $basename" 1>&2

    generatedfiles=

    # Test the parser

    madegrc=0 &&
    RunPass $STRLXML $1 ${basename}.ast.xml &&
    RunPass $XMLSTRL ${basename}.ast.xml ${basename}.ast.strl &&
    Compare ${basename}.ast.strl ${reffile}.out $1 ${basename}.ast.diff &&

    # Test the expander

    RunPass $EXPAND ${basename}.ast.xml ${basename}.exp.xml &&
    RunPass $XMLSTRL ${basename}.exp.xml ${basename}.exp.strl &&
    Compare ${basename}.exp.strl ${reffile}.expout $1 ${basename}.exp.diff &&

    # Test the dismantler
   
    RunPass $DISMANTLE ${basename}.exp.xml ${basename}.dis.xml &&
    RunPass $XMLSTRL ${basename}.dis.xml ${basename}.dis.strl &&
    Compare ${basename}.dis.strl ${reffile}.disout $1 ${basename}.dis.diff &&

    # Test GRC generation

    RunPass "$ASTGRC $astgrcargs" ${basename}.dis.xml ${basename}.grc.xml &&
    madegrc=1

    # Generate a .dot file if requested

    if [ $madegrc -ne 0 -a $dot -ne 0 ] ; then
	RunPass $GRCDOT ${basename}.grc.xml ${basename}.grc.dot
	if [ $ps -ne 0 ] ; then
	    Run dot -Tps -o ${basename}.grc.ps ${basename}.grc.dot
	fi
    fi

    # Run the GRC simulator

    if [ $madegrc -ne 0 ] ; then
	if [ ! -f ${reffile}.do-not-simulate ] ; then
	    RunGRCSIM ${reffile} ${basename} grc
	else
	    echo "##### did not simulate grc because ${reffile}.do-not-simulate exists" 1>&2
	fi
    fi

    # Run the GRC optimizer

    madeopt=0 &&
    [ $madegrc -ne 0 ] && 
    RunPass "$GRCOPT" ${basename}.grc.xml ${basename}.opt.xml &&
    madeopt=1

    # Generate a .dot file if requested

    if [ $madeopt -ne 0 -a $dot -ne 0 ] ; then
	RunPass $GRCDOT ${basename}.opt.xml ${basename}.opt.dot
	if [ $ps -ne 0 ] ; then
	    Run dot -Tps -o ${basename}.opt.ps ${basename}.opt.dot
	fi
    fi

    # Run the GRC simulator

    if [ $madeopt -ne 0 ] ; then
	if [ ! -f ${reffile}.do-not-simulate ] ; then
	    RunGRCSIM ${reffile} ${basename} opt
	else
	    echo "##### did not simulate opt because ${reffile}.do-not-simulate exists" 1>&2
	fi
    fi

    # Translate into PDG

    madepdg=0 &&
    [ $madeopt -ne 0 ] &&
    RunPass "$GRCPDG" ${basename}.opt.xml ${basename}.pdg.xml &&
    madepdg=1

    # Generate .dot file if requested

    if [ $madepdg -ne 0 -a $dot -ne 0 ] ; then
	RunPass $GRCDOT ${basename}.pdg.xml ${basename}.pdg.dot
	if [ $ps -ne 0 ] ; then
	    Run dot -Tps -o ${basename}.pdg.ps ${basename}.pdg.dot
	fi
    fi

    # Run the GRC simulator and check the results

    if [ $madepdg -ne 0 ] ; then
	if [ ! -f ${reffile}.do-not-simulate ] ; then
	    RunGRCSIM ${reffile} ${basename} pdg
	else
	    echo "##### did not simulate pdg because ${reffile}.do-not-simulate exists" 1>&2
	fi
    fi

    # Run the circuit synthesizer and simulate

    if [ $madepdg -ne 0 ] ; then
	if [ ! -f ${reffile}.do-not-synthesize ] ; then

	    madeblif=0 &&
	    generatedfiles="$generatedfiles ${basename}.core.blif ${basename}.sm" &&
	    Run $PDGBLIF --sm ${basename}.sm "<" ${basename}.pdg.xml \
		">" ${basename}.core.blif &&

        # Convert the trivial state machine file into BLIF

	    generatedfiles="$generatedfiles ${basename}.sm.blif ${basename}.out.blif" &&
	    Run $SMBLIF ${basename}.sm ${basename}.sm.blif &&
	    Run sed '/^[.]end/d' ${basename}.core.blif ">" ${basename}.out.blif &&
	    Run cat ${basename}.sm.blif ">>" ${basename}.out.blif &&
	    Run echo ".end" ">>" ${basename}.out.blif &&
	    madeblif=1

        # Generate .dot file if requested

	    if [ $madeblif -ne 0 -a $dot -ne 0 ] ; then
		RunPass "$BLIFDOT" ${basename}.out.blif ${basename}.blif.dot
		if [ $ps -ne 0 ] ; then
		    Run dot -Tps -o ${basename}.blif.ps ${basename}.blif.dot
		fi
	    fi

        # Run the circuit simulator and compare the results

	    [ $madeblif -ne 0 ] &&
	    generatedfiles="$generatedfiles ${basename}.blif.simout ${basename}.blif.sim.diff" &&
	    if [ -f ${reffile}.tv ] ; then
	    # Use a test vector file
		Run $BLIFSIM -t ${reffile}.tv ${basename}.out.blif \
		    ">" ${basename}.blif.simout
	    else
            # Run a certain number of cycles
		Run $BLIFSIM -c `cat ${reffile}.simout | wc -l` \
		    ${basename}.out.blif ">" ${basename}.blif.simout
	    fi &&
	    Compare ${basename}.blif.simout ${reffile}.simout  ${reffile}.simout \
		${basename}.blif.sim.diff

	else
	    echo "##### did not synthesize circuit because ${reffile}.do-not-synthesize exists" 1>&2
	fi
    fi
       
    # Use GRCC2 to generate C code and compile
    
    if [ ! -f ${reffile}.do-not-generate-c ] ; then
	[ $madegrc -ne 0 ] &&
	madegrcc2o=0 &&
	RunPass "$GRCC2 -B ${basename}" ${basename}.opt.xml ${basename}.grcc2.c &&
	generatedfiles="$generatedfiles ${basename}.grcc2.o" &&
	Run $CC $CCARGS  -c ${basename}.grcc2.c -I ${basedir} &&
	madegrcc2o=1

    # Generate and compile a testbench unless do-not-execute is present

	madecmain=0 &&
	[ $madegrc -ne 0 ] && if [ ! -f ${reffile}.do-not-execute ] ; then
	    generatedfiles="$generatedfiles ${basename}.main.c ${basename}.main.o" &&
	    Run $V5CMAIN -B $basename "<" ${basename}.opt.xml ">" ${basename}.main.c &&
	    Run $CC $CCARGS  -c ${basename}.main.c -I ${basedir} &&
	    madecmain=1
	else
	    echo "##### did not generate testbench because ${reffile}.do-not-execute exists" 1>&2
	fi

    # Link the testbench with the GRCC2-generated code and run

	[ $madecmain -ne 0 ] &&
	[ $madegrcc2o -ne 0 ] &&
	generatedfiles="$generatedfiles ${basename}.grcc2.exe ${basename}.grcc2.simout" &&
	Run $CC $CCARGS -o ${basename}.grcc2.exe \
	    ${basename}.grcc2.o ${basename}.main.o &&
	if [ -f ${reffile}.tv ] ; then
	    Run ./${basename}.grcc2.exe -t ${reffile}.tv \
		">" ${basename}.grcc2.simout
	else
	    Run ./${basename}.grcc2.exe -c  `cat ${reffile}.simout | wc -l` \
		">" ${basename}.grcc2.simout
	fi &&
	Compare ${basename}.grcc2.simout ${reffile}.simout \
	    ${reffile}.simout ${basename}.grcc2.sim.diff

    # Use GRCC2 in ANSI mode to generate C code, run, and compare

	[ $madegrc -ne 0 ] &&
	madegrcc2o=0 &&
	RunPass "$GRCC2 -B ${basename} -a" ${basename}.opt.xml \
	    ${basename}.grcc2a.c &&
	generatedfiles="$generatedfiles ${basename}.grcc2a.o" &&
	Run $CC $CCARGS  -c ${basename}.grcc2a.c -I ${basedir} &&
	[ $madecmain -ne 0 ] &&
	generatedfiles="$generatedfiles ${basename}.grcc2a.exe ${basename}.grcc2a.simout" &&
	Run $CC $CCARGS -o ${basename}.grcc2a.exe \
	    ${basename}.grcc2a.o ${basename}.main.o &&
	if [ -f ${reffile}.tv ] ; then
	    Run ./${basename}.grcc2a.exe -t ${reffile}.tv \
		">" ${basename}.grcc2a.simout
	else
	    Run ./${basename}.grcc2a.exe -c  `cat ${reffile}.simout | wc -l` \
		">" ${basename}.grcc2a.simout
	fi &&
	Compare ${basename}.grcc2a.simout ${reffile}.simout \
	    ${reffile}.simout ${basename}.grcc2a.sim.diff

    else
	echo "##### did not generate C because ${reffile}.do-not-generate-c exists" 1>&2
    fi

    # Generate VHDL FIXME: currently disabled

    #if [ $madeopt -ne 0 ] ; then
    #	RunPass "$GRCVHDL" ${basename}.opt.xml ${basename}.vhdl
    #fi

    # Generate C using the PDG restructuring algorithm, compile,
    # link with the simulator front-end, and run

    if [ ! -f ${reffile}.do-not-generate-c ] ; then
	madeccfg=0 &&
	madescfg=0 &&
	[ $madepdg -ne 0 ] &&	
	RunPass "$PDGCCFG" ${basename}.pdg.xml ${basename}.ccfg.xml &&
	madeccfg=1 &&
        RunPass "$EEC" ${basename}.ccfg.xml ${basename}.scfg.xml &&
	madescfg=1 &&
        RunPass "$SCFGC -B ${basename}" ${basename}.scfg.xml ${basename}.pdg.c &&
	generatedfiles="$generatedfiles ${basename}.pdg.o" &&
	Run $CC $CCARGS  -I ${basedir} -c ${basename}.pdg.c &&
	[ $madecmain -ne 0 ] &&
	generatedfiles="$generatedfiles ${basename}.pdg.exe ${basename}.pdg.simout" &&
	Run $CC $CCARGS -o ${basename}.pdg.exe \
	    ${basename}.pdg.o ${basename}.main.o &&
	if [ -f ${reffile}.tv ] ; then
	    Run ./${basename}.pdg.exe -t ${reffile}.tv \
		">" ${basename}.pdg.simout
	else
	    Run ./${basename}.pdg.exe -c  `cat ${reffile}.simout | wc -l` \
		">" ${basename}.pdg.simout
	fi &&
	Compare ${basename}.pdg.simout ${reffile}.simout \
	    ${reffile}.simout ${basename}.pdg.sim.diff

        # Generate a .dot file for the ccfg and scfg files if requested

	if [ $madeccfg -ne 0 -a $dot -ne 0 ] ; then
	    RunPass $GRCDOT ${basename}.ccfg.xml ${basename}.ccfg.dot
	    if [ $ps -ne 0 ] ; then
		Run dot -Tps -o ${basename}.ccfg.ps ${basename}.ccfg.dot
	    fi
	fi

	if [ $madescfg -ne 0 -a $dot -ne 0 ] ; then
	    RunPass $GRCDOT ${basename}.scfg.xml ${basename}.scfg.dot
	    if [ $ps -ne 0 ] ; then
		Run dot -Tps -o ${basename}.scfg.ps ${basename}.scfg.dot
	    fi
	fi

	# Generate BAL, the assembly language for the virtual machine,
	# assemble it, and simulate the results

	madebal=0 &&
	if [ ! -f ${reffile}.do-not-generate-bal ] ; then
	    [ $madeopt -ne 0 ] &&
	    RunPass "$GRCBAL" ${basename}.opt.xml ${basename}.bal &&
	    RunPass "$BALASM" ${basename}.bal ${basename}.bal.o &&
	    madebal=1 &&
	    if [ 0 -ne 0 -a -f ${reffile}.tv ] ; then
		generatedfiles="$generatedfiles ${basename}.bal.simout" &&
		Run $VM ${basename}.bal.o ${reffile}.tv \
		    ${basename}.bal.simout &&
		Compare ${basename}.bal.simout ${reffile}.simout \
		    ${reffile}.simout ${basename}.bal.sim.diff
	    else
		echo "##### did not run the VM simulator because  ${reffile}.tv  does not exist" 1>&2
	    fi
	else
	    echo "##### did not generate BAL because ${reffile}.do-not-generate-bal exists" 1>&2
	fi

	if [ $madebal -ne 0 -a $dot -ne 0 ] ; then
	    RunPass "$GRCBAL -s" ${basename}.opt.xml ${basename}.bal.dot &&
	    if [ $ps -ne 0 ] ; then
		Run dot -Tps -o ${basename}.bal.ps ${basename}.bal.dot
	    fi
	fi

	# Using a wrapper, compile a VM-based executable and run it
	if [ $madebal -ne 0 -a $madecmain -ne 0 ] ; then
	    generatedfiles="$generatedfiles ${basename}.bal.c" &&
	    Run cat balvm.c ">" $basename.bal.c &&
	    Run "$BALASM --c" "<" $basename.bal ">>" ${basename}.bal.c &&
	    Run $VMWRAPPER -B ${basename} "<" ${basename}.opt.xml ">>" $basename.bal.c &&
            generatedfiles="$generatedfiles ${basename}.vm.exe" &&
	    Run $CC $CCARGS  -o ${basename}.vm.exe -I . -I ${basedir} \
		${basename}.main.c ${basename}.bal.c &&
	    	generatedfiles="$generatedfiles ${basename}.vm.simout" &&
		if [ -f ${reffile}.tv ] ; then
		    Run ./${basename}.vm.exe -t ${reffile}.tv \
			">" ${basename}.vm.simout
		else
		    Run ./${basename}.vm.exe -c  \
                       `cat ${reffile}.simout | wc -l` \
			">" ${basename}.vm.simout
		fi &&
		Compare ${basename}.vm.simout ${reffile}.simout \
		    ${reffile}.simout ${basename}.vm.sim.diff
	   
	fi

    fi

    # Report the status and clean up the generated files

    if [ $error -eq 0 ] ; then
	if [ $keep -eq 0 ] ; then
	    rm -f $generatedfiles
	fi
	echo "OK"
    else
	echo "###### FAILED" 1>&2
	globalerror=$error
    fi
}

# RunFail <args>
# Report the command, run it, and expect an error
RunFail() {
    if [ $error -eq 0 ] ; then
	echo $* 1>&2
	eval $* && {
	    echo "FAILED: $1 did not report an error"
	    error=1
        }
    fi
}


CheckFail() {
    error=0
    basename=`echo $1 | sed 's/.*\\///
                             s/.strl//'`
    reffile=`echo $1 | sed 's/.strl$//'`

    generatedfiles=

    echo $ECHO_N "$basename...$ECHO_C"

    echo 1>&2
    echo "###### Testing $basename" 1>&2

    generatedfiles="$generatedfiles ${basename}.ast.xml ${basename}.ast.errors"
    RunFail $STRLXML "<" $1 ">" ${basename}.ast.xml "2>" ${basename}.ast.errors
    Compare ${basename}.ast.errors ${reffile}.out ${reffile}.out ${basename}.ast.diff
    
    if [ $error -ne 1 ] ; then
	if [ $keep -eq 0 ] ; then
	    rm -f $generatedfiles
	fi
	generatedfiles=
	echo "OK"
	if [ $error -eq 0 ] ; then
	    echo "###### SUCCESS" 1>&2
	else
	    echo "###### PARTIAL SUCCESS" 1>&2
	fi
	error=0
    else
	echo "###### FAILED" 1>&2
	globalerror=$error
	error=0
    fi

}

while getopts kdpsh c; do
    case $c in
	k) # Keep intermediate files
	    keep=1
	    ;;
	d) # Generate dot files
	    keep=1
	    dot=1
	    ;;
	p) # Generate Postscript files
	    keep=1
	    dot=1
	    ps=1
	    ;;
	s) # Do not use schizophrenic expansion
	    astgrcargs=-s
	    ;;
	h) # Help
	    Usage
	    ;;
    esac
done

shift `expr $OPTIND - 1`

if [ $# -ge 1 ]
then
    files=$@
else
    files="../tests/fail*.strl ../tests/test*.strl"
fi

$DUMPBALVMC > balvm.c

for file in $files
do
  case $file in
      *test-*strl)
	  Check $file 2>> $globallog
	  ;;
      *fail-*strl)
	  CheckFail $file 2>> $globallog
	  ;;
      *)
	  echo "Unknown file type $file"
	  globalerror=1
	  ;;
  esac
done

exit $globalerror
