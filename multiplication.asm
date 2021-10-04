    	lw 0 1 zero				
	lw 0 2 mcand				
	lw 0 3 mplier			
	lw 0 4 one                           
multi	beq 7 3 finish			
	nand 3 4 6	        
	nand 6 6 6	
	add 6 7 7   		      		
        beq 3  4  case								        
	beq 0 6 shift								
shift	add 1 2 1
	add 4 4 4			        
        add 2 2 2			        
        beq 0 0 multi		        
case 	add 1 2 1				
	beq 0 0 finish			
finish  halt
zero .fill 0
one .fill 1
mcand .fill 10000
mplier .fill 5000
