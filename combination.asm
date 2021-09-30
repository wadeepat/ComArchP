    lw 0 5 pos1
    lw 0 1 n                
    lw 0 2 r                               
    lw 0 6 nCrAdr
    jalr 6 4         
    halt       
nCr beq 2 0 ans1Ad
    beq 2 1 ans1Ad
    beq 2 5 ans2Ad
    lw 0 6 neg1
    add 1 6 1     
    beq 2 1 ans3Ad
    sw 7 4 stack   
    add 7 5 7      
    sw 7 1 stack   
    add 7 5 7
    sw 7 2 stack    
    add 7 5 7
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
ans1    add 3 pos1 3     
        jalr 4 6         
ans2    add 3 1 3          
        jalr 4 6
ans3    add 1 pos1 1     
        add 3 1 3
        jalr 4 6
pos1 .fill 1
neg1 .fill -1
nCrAdr .fill nCr
ans1Ad .fill ans1
ans2Ad .fill ans2
ans3Ad .fill ans3
n .fill 2
r .fill 1
stack .fill 0
