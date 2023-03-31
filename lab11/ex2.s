		.data
twodarr: 	.long 0x1,0x2,0x3,0x4
 	 	.long 0x5,0x6,0x7,0x8
 	 	.long 0x9,0x10,0x11,0x12
 	 	.long 0x13,0x14,0x15,0x16

row:  		.long 0
col:  		.long 0
sizerow:	.long 4

prompt1: 	.string "Enter row: "
prompt2: 	.string "Enter column:"
result:  	.string "The indexed element is: %x\n"
fstr: 	 	.string "%x"
 	
 	 	.text
 	 	.globl main

getElement:
    imul %esi, %edx 
    add %edx, %ecx
    mov (%rdi, %rcx, 4), %eax
    ret 

main:      
		sub     $0x8, %rsp
 
      	#prompt and read in row index
      	mov  $prompt1,%edi
		mov $0,%eax
      	call    printf

      	mov  $row,%esi
      	mov   $fstr,%edi
		mov $0,%eax
      	call    scanf
 
		#prompt and read in column index
      	mov  $prompt2, %edi
		mov $0,%eax
      	call    printf

      	mov  $col,%esi
      	movl  $fstr,%edi
		mov $0,%eax
      	call     scanf
 
 		#getElement parameters = base address of array, size of row,row index, and column index 
		mov  col,%ecx
      	mov  row,%edx
 		mov  sizerow,%esi
		mov $twodarr,%edi
		call getElement 			# uncomment this line after you write getElement
 	
      	mov   %eax,%esi
      	mov   $result,%edi
		mov $0,%eax
  		call    printf
 
      	add $0x8,%rsp
		ret
