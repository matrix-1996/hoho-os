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

#ifndef PROC_H
#define PROC_H

#include <hal/tss.h>
#include <mm/memory.h>
#include <types.h>

#define PROC_NULL       -1

#define PROC_STOPPED    0
#define PROC_ACTIVE     1
#define PROC_NEW        2

typedef struct regs {
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    uint32_t edi;
    uint32_t esi;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t flags;
} regs_t;

typedef struct proc {
    char name[16];
    int id;
    int priority;
    struct page_directory *pdir;
    int state;
    uint32_t eip;
    uint32_t esp;
    uint32_t stack_limit;
    uint32_t image_base;
    uint32_t image_size;
    struct proc *next;
    struct proc *prec;
} process_t;

int start_proc(char *name);
void end_proc(int ret);
int proc_state(int id);

#endif
