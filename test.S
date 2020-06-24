LEAF(write)

li $t0, 0xb0000000
sw $a0, 0($t0)
jr $ra
END(write)
