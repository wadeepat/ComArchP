    lw 0 5 pos1
    lw 0 1 n                
    lw 0 2 r                               
    lw 0 6 nCrAdr
    jalr 6 4         
    halt       
nCr beq 2 0 ans1
    beq 2 1 ans1
    beq 2 5 ans2    
    lw 0 6 neg1
    add 1 6 1     
    beq 2 1 ans3
    sw 7 4 stack   
    add 7 5 7      
    sw 7 1 stack   
    add 7 5 7
    sw 7 2 stack    
    add 7 5 7
    lw 0 6 nCrAdr
    jalr 6 4        
    lw 0 6 neg1       
    add 7 6 7         
    lw 7 2 stack     
    add 7 6 7
    lw 7 1 stack
    add 7 6 7
    lw 7 4 stack
    add 2 6 2          
    sw 7 4 stack      
    add 7 5 7        
    sw 7 1 stack      
    add 7 5 7
    sw 7 2 stack     
    add 7 5 7
    lw 0 6 nCrAdr	
    jalr 6 4          
    lw 0 6 neg1        
    add 7 6 7       
    lw 7 2 stack   
    add 7 6 7
    lw 7 1 stack
    add 7 6 7
    lw 7 4 stack
    jalr 4 6
ans1    add 3 5 3     
        jalr 4 6         
ans2    add 3 1 3          
        jalr 4 6
ans3    add 1 5 1     
        add 3 1 3
        jalr 4 6
        noop
        noop
pos1 .fill 1
neg1 .fill -1
nCrAdr .fill nCr
ans1Ad .fill ans1
ans2Ad .fill ans2
ans3Ad .fill ans3
n .fill 5
r .fill 2
stack .fill 0
