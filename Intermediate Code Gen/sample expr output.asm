.MODEL small 
.STACK 100h 
.DATA
	print_var dw ?
	ret_temp dw ?
	a_1 dw ?
	b_1 dw ?
	c_1 dw 3 dup(?)
	t0 dw ?
	t1 dw ?
	t2 dw ?
	t3 dw ?
	t4 dw ?
	t5 dw ?
	t6 dw ?
.CODE
//write in main regardless
main_proc PROC
	mov ax, @data
	mov ds, ax
	
	
	
	
	
	///start
	
	mov ax, 2
	add ax, 3
	mov t0, ax
	mov ax, 1
	mov bx, t0
	mul bx
	mov t1, ax
	mov ax, t1
	mov bx, 3
	xor dx, dx
	div bx
	mov t2, dx
	mov ax, t2
	mov a_1, ax
	mov ax, 1
	cmp ax, 5
	jge L0
	mov t3, 1
	jmp L1
L0:
	mov t3, 0
L1:
	mov ax, t3
	mov b_1, ax
	mov di, 0
	add di, di
	mov ax, 2
	mov c_1[di], ax
	cmp a_1, 0
	je L2
	cmp b_1, 0
	je L2
	mov t4, 1
	jmp L3
L2:
	mov t4, 0
L3:
	mov ax, t4
	cmp ax, 0
	je L4
	mov di, 0
	add di, di
	mov ax, c_1[di]
	mov t5, ax
	inc c_1[di]
	jmp L5
L4:
	mov di, 0
	add di, di
	mov ax, c_1[di]
	mov t6, ax
	mov di, 1
	add di, di
	mov ax, t6
	mov c_1[di], ax
L5:
	mov ax, a_1
	mov print_var, ax
	call print
	mov ax, b_1
	mov print_var, ax
	call print
main_proc ENDP
END main_proc
