#beq
#4
#QFSW
#rs!=rt
#
#

addi $2, $0, 1
sub $3, $0, $2
beq $0, $3 end
addi $2, $2, 1
addi $2, $2, 1
addi $2, $2, 1
end: jr $0
nop