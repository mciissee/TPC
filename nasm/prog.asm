section .data ; global variables 
  $iformat: db "%d", 0
  $cformat: db "%c", 0
  $lformat: db "%ld", 0
  C1: equ 10; const
  C2: equ 67; const
  a:  dd 0
  b:  dd 0

section .text
extern scanf
extern printf
global _start

next:
  push rbp
  mov rbp, rsp


  mov cl, 66
  mov rax, rcx
  mov rsp, rbp
  pop rbp
  ret

_start: 
  push rbp
  mov rbp, rsp

  mov ecx, 12
  mov [a], ecx
  mov ecx, 24
  mov [b], ecx
  mov cl, 65
  push rcx
  call  next
  pop rbx
  mov cl, al
  mov rsi, rcx
  mov rdi, $cformat
  mov rax, 0
  call printf
  mov cl, 65
  mov rsi, rcx
  mov rdi, $cformat
  mov rax, 0
  call printf
  mov ecx, [a]
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 0
  mov rax, rcx
  mov ecx, 0
  cmp rax, rcx
  sete cl
  and cl, 1
  movzx rax, cl
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 10
  mov rax, rcx
  mov ecx, 10
  cmp rax, rcx
  setg cl
  and cl, 1
  movzx rax, cl
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 10
  mov rax, rcx
  mov ecx, 10
  cmp rax, rcx
  setl cl
  and cl, 1
  movzx rax, cl
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 10
  mov rax, rcx
  mov ecx, 10
  cmp rax, rcx
  setge cl
  and cl, 1
  movzx rax, cl
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 10
  mov rax, rcx
  mov ecx, 10
  cmp rax, rcx
  setle cl
  and cl, 1
  movzx rax, cl
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 0
  mov rax, rcx
  mov ecx, [a]
  sub rax, rcx
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 0
  mov rax, rcx
  mov ecx, [a]
  cmp rax, rcx
  set cl
  and cl, 1
  movzx rax, cl
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 0
  mov rax, rcx
  mov ecx, 12
  cmp rax, rcx
  set cl
  and cl, 1
  movzx rax, cl
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov cl, 66
  mov rsi, rcx
  mov rdi, $cformat
  mov rax, 0
  call printf
  mov ecx, [b]
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, [b]
  mov rax, rcx
  mov ecx, 24
  cmp rax, rcx
  sete cl
  and cl, 1
  movzx rax, cl
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 2
  mov rax, rcx
  mov ecx, 3
  imul rax, rcx
  push rax
  mov ecx, [b]
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  mov ecx, [a]
  mov rax, rcx
  mov ecx, 2
  imul rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 2
  mov rax, rcx
  mov ecx, 3
  imul rax, rcx
  push rax
  mov ecx, [b]
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  mov ecx, [a]
  mov rax, rcx
  mov ecx, 2
  imul rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  mov ecx, 2
  mov rax, rcx
  mov ecx, 3
  imul rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 2
  mov rax, rcx
  mov ecx, 3
  imul rax, rcx
  push rax
  mov ecx, [b]
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  mov ecx, [a]
  mov rax, rcx
  mov ecx, 2
  imul rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  mov ecx, 2
  mov rax, rcx
  mov ecx, 3
  imul rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  mov ecx, 2
  imul rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 2
  mov rax, rcx
  mov ecx, 3
  imul rax, rcx
  push rax
  mov ecx, [b]
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  mov ecx, [a]
  mov rax, rcx
  mov ecx, 2
  imul rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  mov ecx, 2
  mov rax, rcx
  mov ecx, 3
  imul rax, rcx
  push rax
  mov ecx, [b]
  mov rax, rcx
  mov ecx, 14
  sub rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  pop rcx
  imul rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, [a]
  mov rax, rcx
  mov ecx, [b]
  add rax, rcx
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, [a]
  mov rax, rcx
  mov ecx, [b]
  sub rax, rcx
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, [b]
  mov rax, rcx
  mov ecx, [a]
  sub rax, rcx
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, [a]
  mov rax, rcx
  mov ecx, [b]
  imul rax, rcx
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 2
  mov rax, rcx
  mov ecx, 3
  imul rax, rcx
  push rax
  mov ecx, 24
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  mov ecx, 12
  mov rax, rcx
  mov ecx, 2
  imul rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 2
  mov rax, rcx
  mov ecx, 3
  imul rax, rcx
  push rax
  mov ecx, 24
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  mov ecx, 12
  mov rax, rcx
  mov ecx, 2
  imul rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  mov ecx, 2
  mov rdx, 0
  idiv ecx
  push rdx
  pop rcx
  mov rax, rcx
  mov ecx, 1
  add rax, rcx
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 2
  mov rax, rcx
  mov ecx, 3
  imul rax, rcx
  push rax
  mov ecx, 24
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  mov ecx, 12
  mov rax, rcx
  mov ecx, 2
  imul rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  pop rcx
  add rax, rcx
  push rax
  mov ecx, 2
  mov rax, rcx
  mov ecx, 1
  add rax, rcx
  push rax
  pop rcx
  mov rax, rcx
  pop rcx
  mov rdx, 0
  idiv ecx
  push rdx
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, [a]
  mov rax, rcx
  mov ecx, 2
  mov rdx, 0
  idiv ecx
  push rdx
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, [a]
  mov rax, rcx
  mov ecx, 3
  mov rdx, 0
  idiv ecx
  push rdx
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov ecx, 10
  mov rax, rcx
  mov ecx, 100
  imul rax, rcx
  push rax
  pop rcx
  mov rsi, rcx
  mov rdi, $iformat
  mov rax, 0
  call printf
  mov cl, 67
  mov rsi, rcx
  mov rdi, $cformat
  mov rax, 0
  call printf

  mov ecx, 0
  mov rax, rcx
  mov rsp, rbp
  pop rbp
  mov eax, 1
  mov ebx, eax
  int 80h
