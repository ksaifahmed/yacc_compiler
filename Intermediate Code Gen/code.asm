.model small
.stack 100h
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
mov a1.1, ax
mov ax, 1
cmp ax, 5
jl L0
mov t3, 0
jmp L1
L0:
mov t3, 1
L1:
mov ax, t3
mov b1.1, ax
mov bx, 0
add bx, bx
mov ax, 2
mov  c1.1[0][bx], ax
exit:
mov ah,4ch
int 21h
main endp
