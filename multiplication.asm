	lw 0 1 zero				
	lw 0 2 mcand				
	lw 0 3 mplier				
	lw 0 4 one 				
multi 	beq 7 3 finish			
	nand 3 4 6			
	nand 6 6 6			
	add 6 7 7			
	beq 0 6 shift				
	add 1 2 1			
shift 	add 4 4 4			
	add 2 2 2				
	beq 0 0 multi			
finish  halt
zero .fill 0
one .fill 1
mcand .fill 32766
mplier .fill 10383
