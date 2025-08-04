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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include "coctx.h"
#include "co_routine.h"
#include "co_routine_inner.h"


stCoRoutine_t* co_a;
stCoRoutine_t* co_b;
stCoRoutine_t* co_c;

void* CoA(void* args) {
    printf("CoA Begin\n");
    co_resume(co_b);
    printf("CoA End\n");
}

void* CoB(void* args) {
    printf("CoB Begin\n");
    co_resume(co_c);
    printf("CoB End\n");
}

void* CoC(void* args) {
    printf("CoC Begin\n");
    co_resume(co_a);
    printf("CoC End\n");
}


int main()
{
	stCoRoutineAttr_t attr;
    co_create(&co_a, &attr, CoA, nullptr);
    co_create(&co_b, &attr, CoB, nullptr);
    co_create(&co_c, &attr, CoC, nullptr);

    printf("main before resume a\n");
    co_resume(co_a);
    printf("main after resume a\n");

    printf("main before loop\n");
	co_eventloop(co_get_epoll_ct(), NULL, NULL);
    printf("main after loop\n");
	return 0;
}
