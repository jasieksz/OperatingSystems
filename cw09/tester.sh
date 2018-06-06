#!/bin/bash      

# ./program PRODUCERS CONSUMERS BUFFER PATH VALUE MODE(> = <) OUT(v s) TIME

# TIME ==> STOP
# bin/zad1/program 2 4 6 in1.txt 0 = v 30
# bin/zad2/pprogram 2 2 6 in2.txt 0 = v 20

# EOF ==> STOP
# bin/zad1/program 2 2 4 in2.txt 0 = v 0
bin/zad2/pprogram 6 6 10 in1.txt 7 '>' s 0
#
