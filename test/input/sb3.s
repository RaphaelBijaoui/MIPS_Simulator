#sb
#-11
#QFSW
#above
#

addi $30, $0, 0x24
sll $30, $30, 24
addi $3, $0, -5
sb $3, 0($30)
lw $2, 0($30)
nop
jr $0
nop