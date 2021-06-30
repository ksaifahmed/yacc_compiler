.model small
.stack 100h

.data
	print_var dw ?
	ret_temp dw ?
	a11 dw ?
	t0 dw ?
	a12 dw ?
	b12 dw ?
	x12 dw ?
	t1 dw ?
	t2 dw ?
	a13 dw ?
	b13 dw ?

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

f proc
push ax
push bx
push cx
push dx
push di
mov ax, 2
mov bx, a11
mul bx
mov t0, ax
move ax, t0
mov ret_temp, ax
pop di
pop dx
pop cx
pop bx
pop ax
ret
mov ax, 9
mov  a11, ax
f endp
g proc
push ax
push bx
push cx
push dx
push di
mov ax, 
add ax, a12
mov t1, ax
mov ax, t1
add ax, b12
mov t2, ax
mov ax, t2
mov  x12, ax
move ax, x12
mov ret_temp, ax
pop di
pop dx
pop cx
pop bx
pop ax
ret
g endp
main proc
mov ax,@data
mov ds,ax
mov ax, 1
mov  a13, ax
mov ax, 2
mov  b13, ax
mov ax, 
mov  a13, ax
mov ax, a13
mov print_var, ax
call print
move ax, 0
mov ret_temp, ax
pop di
pop dx
pop cx
pop bx
pop ax
ret
exit:
mov ah,4ch
int 21h
main endp
end main
