	.data

prompt_string: .string "Enter an array index: "
format_string1: .string "%lx"
format_string2: .string "The value of the array element is: %x\n"
elements: .long 0x1, 0x3, 0x5, 0x7, 0x9, 0x11, 0x13, 0x15
short_elements: .word 0x23, 0x25, 0x27, 0x29, 0x31, 0x33, 0x35,0x37
byte_elements: .byte 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80,0x90
aindex:   .long 0

    .text
  	.globl main

getElement: 
    mov (%rdi,%rsi,4),%eax
    ret 

main:  	#setup
    sub    $0x8,%rsp  

    #prompt for index of element in array
    mov    $prompt_string,%edi  
	mov    $0x0,%eax
	call     printf

	#accept input 
    mov    $aindex,%esi 
	mov    $format_string1,%edi
	mov    $0x0,%eax
	call     scanf

	#call function to return specified element of array
    mov    aindex,%rsi        
	mov    $elements,%edi
	call    getElement

	#print specified array element
    mov    %eax,%esi
	mov    $format_string2,%edi
	mov    $0x0,%eax
 	call     printf
 	
	#cleanup and return from main
 	mov    $0x0,%eax
    add    $0x8,%rsp
 	ret
