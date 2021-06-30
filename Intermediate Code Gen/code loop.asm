.model small
.stack 100h

.data
	print_var dw ?
	a11 dw ?
	b11 dw ?
	c11 dw ?
	i11 dw ?
	t0 dw ?
	t1 dw ?
	t2 dw ?
	t3 dw ?

.code

print PROC
	push ax
	push bx 
	push cx
	push dx
	mov ax, print_var
	mov bx, 10
	mov cx, 0
printLabel1:
	mov dx, 0
	div bx
	push dx
	inc cx
	cmp ax, 0
	jne printLabel1
printLabel2:
	mov ah, 2
	pop dx
	add dl, '0'
	int 21h
	dec cx
	cmp cx, 0
	jne printLabel2
	mov dl, 0Ah
	int 21h
	mov dl, 0Dh
	int 21h
	pop dx
	pop cx
	pop bx
	pop ax
	ret
print endp

main proc
mov ax,@data
mov ds,ax
mov ax, 0
mov  b11, ax
mov ax, 1
mov  c11, ax
mov ax, 0
mov  i11, ax
L4:
mov ax, i11
cmp ax, 4
jl L0
mov t0, 0
jmp L1
L0:
mov t0, 1
L1:
mov ax, t0
cmp ax, 0
je L5
mov ax, i11
mov t1, ax
inc i11
mov ax, 3
mov  a11, ax
L2:
mov ax, a11
mov t2, ax
dec a11
mov ax, t2
cmp ax, 0
je L3
mov ax, b11
mov t3, ax
inc b11
jmp L2
L3:
jmp L4
L5:
mov ax, a11
mov print_var, ax
call print
mov ax, b11
mov print_var, ax
call print
mov ax, c11
mov print_var, ax
call print
exit:
mov ah,4ch
int 21h
main endp
end main