#sh
#-11
#QFSW
#instr
#
#

lui $30, 0x1000
addi $30, $30, 4
addi $15, $0, 73
sh $15, 2($30)
jr $0
nop