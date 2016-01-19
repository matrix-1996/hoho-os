/*
 *  Copyright 2016 Davide Pianca
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <hal/hal.h>
#include <hal/tss.h>
#include <lib/string.h>
#include <drivers/video.h>

tss_t tss;

void switch_usermode() {
    asm volatile("cli; \
                  mov $0x23, %ax; \
                  mov %ax, %ds;   \
                  mov %ax, %es;   \
                  mov %ax, %fs;   \
                  mov %ax, %gs;   \
                  mov %esp, %eax; \
                  pushl $0x23;    \
                  pushl %eax;     \
                  pushf;          \
                  pushl $0x1B;    \
                  push $1f;       \
                  iret;           \
                  1: ");
}

void switch_usermode_start(uint32_t stack, int entry) {
    /*asm volatile("mov $0x23, %%ax;  \
                  mov %%ax, %%ds;   \
                  mov %%ax, %%es;   \
                  mov %%ax, %%fs;   \
                  mov %%ax, %%gs;   \
                  push $0x23;       \
                  push %[stack];    \
                  push $0x202;      \
                  push $0x1B;       \
                  push %[entry];    \
                  iret" : : [stack] "g" (stack), [entry] "g" (entry));*/
    asm volatile("mov %%eax, %%esp" : : "a" (stack));
    asm volatile("pop %gs;          \
                  pop %fs;          \
                  pop %es;          \
                  pop %ds;          \
                  pop %ebp;         \
                  pop %edi;         \
                  pop %esi;         \
                  pop %eax;         \
                  pop %ebx;         \
                  pop %ecx;         \
                  pop %edx;         \
                  iret");
}

void flush_tss(uint16_t sel) {
    asm volatile("cli; \
                  ltr %0;        \
                  sti;" : : "r" (sel));
}

void install_tss(uint32_t index, uint16_t kernel_ss, uint16_t kernel_esp) {
    uint32_t base = (uint32_t) &tss;
    gdt_set_entry(index, base, base + sizeof(tss_t), 0xE9);
    memset((void *) &tss, 0, sizeof(tss_t));
    tss.ss0 = kernel_ss;
    tss.esp0 = kernel_esp;
    /*tss.cs = 0x0B;
    tss.ss = 0x10;
    tss.es = 0x10;
    tss.ds = 0x10;
    tss.fs = 0x10;
    tss.gs = 0x10;*/
    flush_tss(index * sizeof(struct gdt_info));
}
