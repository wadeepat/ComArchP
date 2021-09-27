;main : 
    lw 0 5 pos1
    lw 0 1 n                ; reg1 = n
    lw 0 2 r                ; reg2 = r
    lw 0 3 0                ; reg3 = 0
    lw 0 6 nCrAdr
    jalr 6 4                ; go to reg6 , reg4 = pc+1 
    halt

;stack : 
      
    nCr beq 2 0 ans1Adr     ; r == 0 go to ans1 bc nC0 = 1
        beq 2 1 ans1Adr     ; r == n go to ans1 bc nCn = 1
        beq 2 pos1 ans2Adr  ; r == 1 go to ans2 bc nC1 = n
        ; case nC(n-1) if it is,the ans is n
        add 1 neg1 1        ; n--
        beq 2 1 ans3Adr     ; r == n-1 go to ans3 bc nC(n-1) = n
        ; another case
        ; com(n-1,r)
        sw 7 4 stack        ; push return address to stack
        add 7 5 7           ; down
        sw 7 1 stack        ; push n to stack
        add 7 5 7
        sw 7 2 stack        ; push r to stack
        add 7 5 7
        jalr 6 4            ; go to nCr , reg4 = pc+1 
        lw 0 6 neg1         ; reg6 = -1
        add 7 6 7           ; up stack aka pop 
        lw 7 2 stack        ; pop
        add 7 6 7
        lw 7 1 stack
        add 7 6 7
        lw 7 4 stack

        ; com(n-1,r-1)
        add 2 6 2           ; r--
        sw 7 4 stack        ; push return address to stack
        add 7 5 7           ; down
        sw 7 1 stack        ; push n to stack
        add 7 5 7
        sw 7 2 stack        ; push r to stack
        add 7 5 7
        lw 0 6 nCrAdr	    ; reg6 = nCrAdr
        jalr 6 4            ; go to nCr , reg4 = pc+1 
        lw 0 6 neg1         ; reg6 = -1
        add 7 6 7           ; up
        lw 7 2 stack        ; pop
        add 7 6 7
        lw 7 1 stack
        add 7 6 7
        lw 7 4 stack
        jalr 4 6


      
    ans1    add 3 pos1 3        ; reg3 = 1
            jalr 4 6            ; back to main, reg4 is return address
    
    ans2    add 3 1 3           ; reg3 = n
            jalr 4 6

    ans3    add 1 pos1 1        ; n++ for go back to original of n 
            add 3 1 3           ; reg3 = n
            jalr 4 6
    
;data
pos1 .fill 1
neg1 .fill -1
nCrAdr .fill nCr
ans1Adr .fill ans1
ans2Adr .fill ans2
ans3Adr .fill ans3
n .fill 5
r .fill 2
stack .fill 0

