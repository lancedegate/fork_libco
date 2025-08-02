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


#ifndef __CO_ROUTINE_INNER_H__

#include "co_routine.h"
#include "coctx.h"
struct stCoRoutineEnv_t;
struct stCoSpec_t
{
	void *value;
};

struct stStackMem_t
{
	stCoRoutine_t* occupy_co;
	int stack_size;
	char* stack_bp; //stack_buffer + stack_size  // fengwen: 相当于stack的上界（即栈底）
	char* stack_buffer;  // fengwen: stack空间的起始地址

};

// fengwen: 共享栈池
//			外界通过co_alloc_sharestack创建，来决定共享栈池里有多少个共享栈。
//			创建协调时，轮流使用池中的栈。
//
//			目的是尽量少的进行SaveBuffer内存拷贝。
//			只有在共享栈被共享时，才会触发SaveBuffer内存拷贝。
//			比如，一个协程在切换出去时，其他协调用的正好的池中的其他栈，那这个协程就不需要SaveBuffer。
//			相关代码：co_swap
struct stShareStack_t
{
	unsigned int alloc_idx;
	int stack_size;
	int count;
	stStackMem_t** stack_array;
};



struct stCoRoutine_t  // fengwen: 协程对象。
{
	stCoRoutineEnv_t *env;
	pfn_co_routine_t pfn;  // fengwen: 入口函数
	void *arg;  // fengwen: 入口参数
	coctx_t ctx;  // ?fengwen: 可能是切换的上下文，包括寄存器和stack地址和大小。

	char cStart;  // fengwen: 是否启动过，没启动过的话在resume时用coctx_make初始化协调的ctx和入口函数等。
	char cEnd;  // fengwen: cEnd == 1 表示用户指定的入口函数已经执行完返回了。
	char cIsMain;
	char cEnableSysHook;
	char cIsShareStack;  // fengwen: 是否使用共享栈

	void *pvEnv;

	//char sRunStack[ 1024 * 128 ];
	stStackMem_t* stack_mem;  // fengwen: 栈内存。根据co_create的attr，决定使用共享栈，或单独开辟空间。


	//save satck buffer while confilct on same stack_buffer;
	char* stack_sp; 
	unsigned int save_size;
	char* save_buffer;

	stCoSpec_t aSpec[1024];

};



//1.env
void 				co_init_curr_thread_env();
stCoRoutineEnv_t *	co_get_curr_thread_env();

//2.coroutine
void    co_free( stCoRoutine_t * co );
void    co_yield_env(  stCoRoutineEnv_t *env );

//3.func



//-----------------------------------------------------------------------------------------------

struct stTimeout_t;
struct stTimeoutItem_t ;

stTimeout_t *AllocTimeout( int iSize );
void 	FreeTimeout( stTimeout_t *apTimeout );
int  	AddTimeout( stTimeout_t *apTimeout,stTimeoutItem_t *apItem ,uint64_t allNow );

struct stCoEpoll_t;
stCoEpoll_t * AllocEpoll();
void 		FreeEpoll( stCoEpoll_t *ctx );

stCoRoutine_t *		GetCurrThreadCo();
void 				SetEpoll( stCoRoutineEnv_t *env,stCoEpoll_t *ev );

typedef void (*pfnCoRoutineFunc_t)();

#endif

#define __CO_ROUTINE_INNER_H__
