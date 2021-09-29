lw 0 1 zero				        ;$1=0 stores the product
	lw 0 2 mcand				;$2=mcand(binary)
	lw 0 3 mplier				;$3=mplier(binary)
	lw 0 4 one                  ;$4=1 the power of 2 (start at 0001)
multiplication :
	beq 7 3 finished			;$7 for check loop
	nand 3 4 6				    ;$6 = $3 nand $4 
	nand 6 6 6				    ;$6 = $6 nand $6 
                                = ~((~($3 and $4)) and (~($3 and $4))) =~(~($3 and $4)) 
                                = $3 and $4  (mplier&&$4)
	add 6 7 7				    ;$7=$7+$6 for check loop = mplier round
    beq 3 4  case				;if mplier is the number that is power of 2 
					            because x nand x = 1,1 nand 1 = 0 so always loop to shift 
	beq 0 6 shift				;if bit n of mplier = 0, don’t add
    add 1 2 1				    ;result = result + mcand 
shift :
	add 4 4 4				    ;shift left 1 $4=2 x $4
    add 2 2 2				    ;shift left mcand $2= 2 x $2
    beq 0 0 multiplication		;loop
case :
	add 1 2 1				    ;result = result + mcand
	beq 0 0 finished			
finished : halt
zero .fill 0
one .fill 1
mcand .fill 32766
mplier .fill 10383
