#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *
 *
 * Hints:
 *  1. The variable which is for counting should be defined as 'static'.
 *  2. Use variable 'env_sched_list', which is a pointer array.
 *  3. CANNOT use `return` statement!
 */
/*** exercise 3.14 ***/
void sched_yield(void)
{
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
    
    /*  hint:
     *  1. if (count==0), insert `e` into `env_sched_list[1-point]`
     *     using LIST_REMOVE and LIST_INSERT_TAIL.
     *  2. if (env_sched_list[point] is empty), point = 1 - point;
     *     then search through `env_sched_list[point]` for a runnable env `e`, 
     *     and set count = e->env_pri
     *  3. count--
     *  4. env_run()
     * 
     *
     *  functions or macros below may be used (not all):
     *  LIST_INSERT_TAIL, LIST_REMOVE, LIST_FIRST, LIST_EMPTY
     */

    static struct Env *curenv = NULL;
    static int flag = 0;
    if(flag){
        curenv = NULL;
        flag = 0;
    }
    while(curenv == NULL || curenv->env_status != ENV_RUNNABLE){
        // struct Env *e = NULL;
        // LIST_FOREACH(e,&env_sched_list[point],env_sched_link){
        //     if(e->env_status == ENV_RUNNABLE && e->env_pri > 0){
        //         curenv = e;
        //         count = curenv->env_pri;
        //         break;
        //     }
        // }
        LIST_FOREACH(curenv,&env_sched_list[point],env_sched_link){
            if(curenv->env_status == ENV_RUNNABLE && curenv->env_pri > 0){
                count = curenv->env_pri;
                break;
            }
        }
        if(curenv!=NULL){
            break;
        }
        // if((LIST_FIRST(&env_sched_list[point]))==NULL){
        //     point = 1-point;
        // }
        point = 1 - point;
    }
    count--;
    if(count<=0){
        LIST_REMOVE(curenv,env_sched_link);
        LIST_INSERT_HEAD(&env_sched_list[1-point],curenv,env_sched_link);
        flag = 1;
        // curenv = NULL;
    }
    env_run(curenv);
}
