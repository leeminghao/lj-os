#ifndef __APP_CFG_H
#define __APP_CFG_H

#define MainTaskStkLength	(1024 * 2)
#define Task0StkLength		1024
#define Task1StkLength		(1024 * 2)
#define Task2StkLength		1024


void MainTask( void *pdata );
void Task0( void *pdata );
void Task1( void *pdata );
void Task2( void *pdata );

#define NormalTaskPrio		5
#define MainTaskPrio		(NormalTaskPrio)
#define Task0Prio			(NormalTaskPrio + 1)
#define Task1Prio			(NormalTaskPrio + 2)
#define Task2Prio			(NormalTaskPrio + 3)

#endif