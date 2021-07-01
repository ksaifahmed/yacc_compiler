.model small
.stack 100h

.data
	print_var dw ?
	ret_temp dw ?
	t0 dw ?
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
push dx
push di
; return stmt of line: 2
mov ax, 2
mov bx, word ptr[bp-4]
mul bx
mov t0, ax
mov cx, t0
pop di
pop dx
pop bx
pop ax
ret
; expr from line: 3
mov ax, 9
mov  word ptr[bp-4], ax
f endp


g proc
push ax
push bx
push dx
push di
; expr from line: 8
; function call, line: 8
mov ax,bp
push bp
push word ptr[bp-4]
mov bp,ax
call f
pop word ptr[bp-4]
pop bp
mov ax, cx
add ax, word ptr[bp-4]
mov t1, ax
mov ax, t1
add ax, word ptr[bp-6]
mov t2, ax
mov ax, t2
mov  x12, ax
; return stmt of line: 9
mov cx, x12
pop di
pop dx
pop bx
pop ax
ret
g endp


main proc
mov ax,@data
mov ds,ax
mov bp,sp
; expr from line: 14
mov ax, 1
mov  a13, ax
; expr from line: 15
mov ax, 2
mov  b13, ax
; expr from line: 16
; function call, line: 16
push bp
push a13
push b13
call g
pop b13
pop a13
pop bp
mov ax, cx
mov  a13, ax
; println() of line: 17
mov ax, a13
mov print_var, ax
call print
exit:
mov ah,4ch
int 21h
main endp
end main