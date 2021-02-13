/* 	Sarah Khan 
	skhan102@jh.edu
*/
/* Assembly language implementation of hexdump functions */

	.section .rodata
	hexChars: .string "0123456789abcdef"

	/* TODO: add read-only data such as strings here */

	.section .text

	/* TODO: add functions here */
		.globl hex_to_printable
	hex_to_printable: 
		subq $8, %rsp					/* adjust stack pointer */
		cmpq $32, %rdi					/* compare byteVal to 32 */
		jl .LsetPeriod					/* if below lower bound, change to period */
		cmpq $126, %rdi					/* compare byteVal to 126 */
		jg .LsetPeriod					/* if above upper bound, change to period */
		jmp .LsetRax
	.LsetPeriod:
		movq $46, %rdi
	
	.LsetRax: 
		movq %rdi, %rax 				/* move printable byteVal to return register */
		addq $8, %rsp					/* adjust stack pointer */
		ret

		.globl hex_format_byte_as_hex
	hex_format_byte_as_hex:
		subq $8, %rsp
		/* push all of the callee saved onto stack */
		pushq %r12
		pushq %r13
		pushq %r14
		pushq %r15
		
		movq $2, %r12 				/* let r12 be the last index of buffer */
		movq $0, (%rsi, %r12) 		/* set last char as null terminator */
		subq $1, %r12				/* decrement the index for buffer */

		jmp .LcheckIndex
	
	.LfillBuf:
		movq %rdi, %r13 			/* Moves a copy of byteVal to r13 */
		and $15, %r13				/* Get last four bits of byteVal */
		movq $hexChars, %r14 		/* Move library of hexChars to r14 */
		movq (%r14, %r13), %r15		/* Set r15 to temp hex digit associated with value of r13 */
		movb %r15b, (%rsi, %r12) 	/* Fill the buffer with temporary hex digit */
		subq $1, %r12				/* Decrement the buf index to be filled next */
		shr $4, %rdi 				/* Right shift byteVal by 4 */

	.LcheckIndex:
		cmp $0, %r12
		jge .LfillBuf

		popq %r15					/* pop the callee-saved off stack */
		popq %r14
		popq %r13
		popq %r12

		addq $8, %rsp
		ret

		.globl hex_format_offset
	/* 	%rdi = offset value
		%rsi = sbuf
	*/
	hex_format_offset: 
		pushq %r12						/* Push callee-saved register */
		pushq %r13						/* Push callee-saved register */
		pushq %r14						/* Push callee-saved register */
		movq $0, %r10 					/* Set first index to 0 */
	.LoffsetLoop: 						/* Entering loop to fill sbuf */
		cmpq $8, %r10					/* Check condition %r10 < 8 */
		je .LoffsetDone					/* If %r10 has reached 8, finish method */
		movq $7, %r11					/* Set second index to 7 */
		subq %r10, %r11					/* Second index = 7 - first index */
		movq $0, %r12					/* Reset all the registers */
		movq $0, %r13
		movq $0, %r14
		movq %rdi, %r12					/* Move offset to %r12 */
		andq $15, %r12					/* Set %r12 to %r12 & 15 to get last 4 bits */ 	
		movq $hexChars, %r14			/* Set %r14 to the hexChar library */
		movb (%r14, %r12), %r13b		/* Put associated hex char into %r13 */						
		movb %r13b, (%rsi, %r11) 		/* Put proper hex char into second index of sbuf */
		shrq $4, %rdi					/* Shift offset to the right by 4 */
		incq %r10						/* Increment first index */
		jmp .LoffsetLoop				/* Back to top of loop */
	.LoffsetDone:	
		movb $0, (%rsi, %r10)		/* Put null terminator at end of sbuf */
		popq %r14						/* Restore value of %r14 */
		popq %r13						/* Restore value of %r13 */
		popq %r12 						/* Restore value of %r12 */
		ret

		.globl hex_read
		/* %rdi = pointer to sbuf 
		*/
	hex_read: 
		subq $8, %rsp 						/* Reserve some space on stack */
		movq %rdi, %rsi 					/* 2nd arg to readis buf ptr */
		movq $0, %rdi 						/* 1st arg to read is file desc */
								
		movq $16, %rdx						/* 3rd arg to read is num bytes */
		mov $0, %rax						
		syscall 
		/* number of bytes read is now in %rax */
		movb $0, (%rsi, %rax)  				/* Put null terminator at the end */
	.Ldone: 
		addq $8, %rsp
		ret

	
		.globl hex_write_string
/* %rdi = buffer */
	hex_write_string: 
		subq $8, %rsp							/* Stack alignment */
		/* Figure out string length wihout modifying %rdi*/
		movq $0, %r10							/* String length count = 0 */
		.LloopWriteLength:
			cmpb $0, (%rdi, %r10, 1)			/* Compare curr char to null term */
			jz .LwriteDone						/* Done counting if reached null term */
			incq %r10							/* Increment count */
			jmp .LloopWriteLength
	.LwriteDone:								/* Once %r10 has string length */
		movq %rdi, %rsi							/* 2nd arg is buffer ptr */
		movq $1, %rdi							/* 1st arg is file desc */
		movq %r10, %rdx							/* 3rd arg is num bytes */
		movq $1, %rax							/* write is sys call 1 */
		syscall 								/* Calling write function */
		addq $8, %rsp 							/* Stack alignment */
		ret
/* vim:ft=gas:
 */
