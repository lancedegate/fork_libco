/*
* Tencent is pleased to support the open source community by making Libco available.

* Copyright (C) 2014 THL A29 Limited, a Tencent company. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/

.globl coctx_swap
#if !defined( __APPLE__ )
.type  coctx_swap, @function
#endif
coctx_swap:

#if defined(__i386__)
    movl 4(%esp), %eax
    movl %esp,  28(%eax)
    movl %ebp, 24(%eax)
    movl %esi, 20(%eax)
    movl %edi, 16(%eax)
    movl %edx, 12(%eax)
    movl %ecx, 8(%eax)
    movl %ebx, 4(%eax)


    movl 8(%esp), %eax
    movl 4(%eax), %ebx
    movl 8(%eax), %ecx
    movl 12(%eax), %edx
    movl 16(%eax), %edi
    movl 20(%eax), %esi
    movl 24(%eax), %ebp
    movl 28(%eax), %esp

	ret

#elif defined(__x86_64__)  ; fengwen: rdi: from_co_ctx, rsi: to_co_ctx
	leaq (%rsp),%rax        ; %rax = %rsp
    movq %rax, 104(%rdi)  ; from_co_ctx.regs[13] = %rsp
    movq %rbx, 96(%rdi)   ; from_co_ctx.regs[12] = %rbx
    movq %rcx, 88(%rdi)   ; from_co_ctx.regs[11] = %rcx
    movq %rdx, 80(%rdi)   ; from_co_ctx.regs[10] = %rdx
	  movq 0(%rax), %rax    ; %rax = *%rsp 栈底位置为函数返回位置
	  movq %rax, 72(%rdi)   ; from_co_ctx.regs[9] = *%rsp (函数返回位置)
    movq %rsi, 64(%rdi)   ; from_co_ctx.regs[8] = %rsi
	  movq %rdi, 56(%rdi)   ; from_co_ctx.regs[7] = %rdi
    movq %rbp, 48(%rdi)   ; from_co_ctx.regs[6] = %rbp
    movq %r8, 40(%rdi)    ; from_co_ctx.regs[5] = %r8
    movq %r9, 32(%rdi)    ; from_co_ctx.regs[4] = %r9
    movq %r12, 24(%rdi)   ; from_co_ctx.regs[3] = %r12
    movq %r13, 16(%rdi)   ; from_co_ctx.regs[2] = %r13
    movq %r14, 8(%rdi)    ; from_co_ctx.regs[1] = %r14
    movq %r15, (%rdi)     ; from_co_ctx.regs[0] = %r15
	  xorq %rax, %rax       ; %rax = 0

    movq 48(%rsi), %rbp   ; %rbp = to_co_ctx.regs[6]
    movq 104(%rsi), %rsp  ; %rsp = to_co_ctx.regs[13]
    movq (%rsi), %r15     ; %r15 = to_co_ctx.regs[0]
    movq 8(%rsi), %r14    ; %r14 = to_co_ctx.regs[1]
    movq 16(%rsi), %r13   ; %r13 = to_co_ctx.regs[2]
    movq 24(%rsi), %r12
    movq 32(%rsi), %r9
    movq 40(%rsi), %r8
    movq 56(%rsi), %rdi
    movq 80(%rsi), %rdx
    movq 88(%rsi), %rcx
    movq 96(%rsi), %rbx   ; %rbx = to_co_ctx.regs[12]
		leaq 8(%rsp), %rsp    ; %rsp += 8 ?? 为什么要重新push函数返回位置？为了避免内存错误时仍能返回到正确的函数位置？
		pushq 72(%rsi)        ; push(to_co_ctx.regs[9]) 函数返回位置

    movq 64(%rsi), %rsi   ; %rsi = to_co_ctx.regs[8] 被调用者需要恢复rsi(,rdi,rbx,rbp)
	ret
#endif
