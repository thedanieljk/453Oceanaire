/* Authors:    Patrick Farrell
 *             Daniel Kirkpatrick
 * Course:     CPE 453
 * Asignment:  2
 * Instructor: Dr. NIco
 */

#include "lwp.h"

te(lwpfun,void *,size_t);
void  lwp_exit(void);
tid_t lwp_gettid(void);
void  lwp_yield(void);
void  lwp_start(void);
void  lwp_stop(void);
void  lwp_set_scheduler(scheduler fun);

scheduler lwp_get_scheduler(void) {
     

}

void initRR(void) {

}

void shutdownRR(void) {

}

void admitRR(thread new) {

}

void removeRR(thread victim) {

}

thread nextRR(void) {

}



thread tid2thread(tid_t tid);


