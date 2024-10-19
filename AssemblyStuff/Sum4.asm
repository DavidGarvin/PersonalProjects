	.globl main
	.text
	
main:
	la $t0, array			#Load address for first element of array.
	li $t1, 4				#Load constant 4 for number of loops needed.
	li $s0, 0				#Initialize $s0 with 0.
	
loop:
	lw $t2, 0($t0)			#Load next word to $t2.
	add $s0, $s0, $t2		#Add word to sum total.
	addi $t1, $t1, -1		#Decrement loop counter.
	addi $t0, $t0, 4		#Increment address to next element in array.
	bne $t1, $0, loop		#Branch to 'loop' when loop counter not 0.
	
	move $a0, $s0			#Move sum total to argument.
	li $v0, 1				#Print integer.
	syscall
	
	li $v0, 10				#Terminate program.
	syscall
	
	
	.data
	
array:	.word 4, 5, 10, 12, 8, 3