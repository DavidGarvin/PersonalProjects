	.globl main
	.text

### t0=Temporary N
### t1=First Fib number, Characters for output
### t2=Second (eventually final) Fib number
### t3=Temporary holding for calculations.
### t9=Printout address

### s0=Permament N
	
main:

	###Get proper value.###
	j input_msg			#Jumps past error message the first time.
e_msg:
	la $a0, emsg		#Prepare to print error message. Only used when incorrect input is given.
	li $v0, 4
	syscall				#Print error message.
input_msg:
	la $a0, msg			#Prepare to print input message.
	li $v0, 4
	syscall				#Print input message.
	li $v0, 5			#Prepare to read integer.
	syscall				#Read integer.
	bltz $v0, e_msg		#Less than 0. Error.
	bgt $v0, 46, e_msg	#Greater than 46. Error.
	add $s0, $v0, $0	#Set $s0 to N for final printout.
	addi $t0, $s0, -1	#Set $t0 to N-1. The first loop is taken up by the 0/1 check.
	
	###Print if 0 or 1.###
	li $t2, 0
	beq $s0, $t2, done	#If 0, go to done.
	li $t2, 1
	beq $s0, $t2, done	#If 1, go to done.
	li $t1, 0			#Set $t1 to 0 to begin sequence.
	
	###The magic.###
loop:
	add $t3, $t1, $t2	#Sum of fib numbers to $t3.
	move $t1, $t2		#Move second fib number to be first fib number.
	move $t2, $t3		#Move sum of fib numbers to be second fib number.
	addi $t0, $t0, -1	#Decrement the loop counter.
	beq $t0, $0, done	#If loop counter is at 0, we are done.
	j loop				#If loop counter was not at 0, go through loop again.

	###Final printout.###
done:
	li $a0, 'F'
	li $v0, 11
	syscall				#Print out 'F'.
	move $a0, $s0
	li $v0, 1
	syscall				#Print out N.
	li $v0, 11
	li $a0, ' '
	syscall
	li $a0, '='
	syscall
	li $a0, ' '
	syscall				#Print out ' = '.
	move $a0, $t2
	li $v0, 1
	syscall				#Print out final result.
	
	li $a0, '\n'
	li $v0, 11
	syscall				#Next line.
	
	li $v0, 10
	syscall				#End program.
	
	.data

msg:	.asciiz "Input value for fibonacci sequence between 0 and 46: "
emsg:	.asciiz "Learn to read, fool!\n\n"