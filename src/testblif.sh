#!/bin/sh

BLIFSIM="./cec-blifutil -o 1"
BLIFDOT="./cec-blifutil -d"

globallog=testblif.log
rm -f $globallog
error=0
globalerror=0

keep=0
dot=0
ps=0

Usage() {
    echo "Usage: testblif.sh [options] [.blif files]"
    echo "-k    Keep intermediate files"
    echo "-d    Generate .dot files for intermediate representations"
    echo "-p    Generate .ps files for intermediate representations"
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

# Compare <outfile> <reffile> <difffile>
# Compares the outfile with reffile. Differences, if any, written to difffile
Compare() {
    if [ $error -eq 0 ] ; then
	generatedfiles="$generatedfiles $3"
	if [ -f "$2" ] ; then
	    echo diff -b $1 $2 ">" $3 1>&2
	    diff -b "$1" "$2" > "$3" 2>&1 || {
		echo "FAILED: $1 differs"
		echo "FAILED $1 differs from $2" 1>&2
		error=1
	    }
	else
	    echo "FAILED: $2 missing" 1>&2
	    echo "FAILED: $2 missing"
	    error=1
	fi
    fi
}

# Run <executable> <infile> <outfile>
Run() {
    if [ $error -eq 0 ] ; then
	echo $1 $2 ">" $3 1>&2
	generatedfiles="$generatedfiles $3"
	$1 "$2" > "$3" || {
	    echo "FAILED: $1 terminated"
	    error=1
	}
    fi
}

Check() {
    error=0
    basename=`echo $1 | sed 's/.*\\///
                             s/.blif//'`
    reffile=`echo $1 | sed 's/.blif//'`

    echo $ECHO_N "Testing $basename...$ECHO_C"

    echo 1>&2
    echo "###### Testing $basename" 1>&2

    generatedfiles=

    # Generate .dot file if requested

    if [ $dot -ne 0 ]; then
	Run "$BLIFDOT" $1 ${basename}.blif.dot
    fi
    if [ $ps -ne 0 ]; then
	echo dot  ${basename}.blif.dot 1>&2
	dot -Tps -o ${basename}.blif.ps  ${basename}.blif.dot
    fi

    # Run the simulation and compare the result

    Run "$BLIFSIM -c 20" $1 ${basename}.blif.simout
    Compare ${basename}.blif.simout ${reffile}.simout ${basename}.blif.diff

    # Report the status and clean up the generated files

    if [ $error -eq 0 ]; then
	if [ $keep -eq 0 ]; then
	    rm -f $generatedfiles
	fi
	generatedfiles=
	echo "OK"
	echo "###### SUCCESS" 1>&2
    else
	echo "###### FAILED" 1>&2
	globalerror=$error
	error=0
    fi

}

while getopts kdph c; do
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
    files=../tests/test*.blif
fi

for file in $files
do
  Check $file 2>> $globallog
done

exit $globalerror
