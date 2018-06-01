/* 046267 Computer Architecture - Spring 2017 - HW #3 */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"
#include <stdio.h>

#define EMPTY -1
#define ROOT 'r'
#define TREE 't'  

unsigned int InstNum;

typedef struct progInst
{
	int index;
	InstInfo info;
	int depend1;
	int depend2;
	char type;
	int cycleNum;
	int InstLatency;
}ProgInst;
typedef ProgInst* pProgInst;

/*
Name: analyzeProg
Description: creates a holder to struct pointers. which holds the Instructions of a program,
calculates the max latnecy of each instruction and determines dependencies
Parameters: opsLatency- holds the latency for each opcode
			progtrace- holds the instructions of the program and its opcode,dst and src registers
			numOfInsts - total number of instructions in program
return value: returns a holder of the data structure
*/
ProgCtx analyzeProg(const unsigned int opsLatency[],  InstInfo progTrace[], unsigned int numOfInsts) {
	InstNum = numOfInsts;
	pProgInst ctx = (pProgInst)malloc(numOfInsts * sizeof(ProgInst));
	if (ctx == NULL)
	{
		return PROG_CTX_NULL;
	}
	for (unsigned int i = 0; i < numOfInsts; i++)
	{
		//initializing the ctx-data structure
		ctx[i].index = i;
		ctx[i].depend1 = EMPTY;
		ctx[i].depend2 = EMPTY;
		ctx[i].info = progTrace[i];
		ctx[i].type = ROOT;
		ctx[i].cycleNum = opsLatency[progTrace[i].opcode];
		ctx[i].InstLatency = 0;
	
		for (int j = i-1; j >=0; j--) //looking for dependencies
		{
			if (ctx[i].info.src1Idx == ctx[j].info.dstIdx && ctx[i].depend1 == EMPTY)
			{
				ctx[i].depend1 = j;
				ctx[i].type = TREE;
				if (ctx[i].depend2 == EMPTY)
				{
					ctx[i].InstLatency = ctx[j].InstLatency + ctx[j].cycleNum;
				}

			}
			if (ctx[i].info.src2Idx == ctx[j].info.dstIdx && ctx[i].depend2 == EMPTY)
			{ 
				ctx[i].depend2 = j;
				ctx[i].type = TREE;
				if (ctx[i].depend1 == EMPTY)
				{
					ctx[i].InstLatency = ctx[j].InstLatency + ctx[j].cycleNum;
				}
			}
			//update the instruction latency
			if (ctx[i].depend1 != EMPTY && ctx[i].depend2 != EMPTY)
			{
				if (ctx[ctx[i].depend1].InstLatency > ctx[ctx[i].depend2].InstLatency)
				{
					ctx[i].InstLatency = ctx[ctx[i].depend1].InstLatency + ctx[ctx[i].depend1].cycleNum;
				}
				else
				{
					ctx[i].InstLatency = ctx[ctx[i].depend2].InstLatency + ctx[ctx[i].depend2].cycleNum;
				}
			}
		}
		printf("latency %d \n", ctx[i].cycleNum);

	}
	return (void*)ctx;
}

/*
Name: freeProgCtx
Description: free dynamiccally allocated memory
Parameters: ctx - holds the data structure of the program
return value: None
*/
void freeProgCtx(ProgCtx ctx) {
	free((pProgInst)ctx);
}

/*
Name: getInstDepth
Description: return the lognest path to a certain instruction
Parameters: ctx - holds the data structure of the program
			theInst - the instruction that we are interested in its dependencies
return value: returns the latency of a certain instruction
*/
int getInstDepth(ProgCtx ctx, unsigned int theInst) {
	if(ctx==NULL) return -1;
	pProgInst temp = (pProgInst)ctx;
	return (temp[theInst].InstLatency);
}

/*
Name: getInstDeps
Description: returns the index of the instructions that the curr instruction depends on
parameters: ctx - holds the data structure of the program
			theInst - the instruction that we are interested in its dependencies
			src1DepInst - will hold the instruction index that has a dst register 
							that the current instructions src1 depends on  
			src2DepInst - will hold the instruction index that has a dst register
							that the current instructions src2 depends on
return value: -1 if unsuccessfull
			   0 if successfull
*/
int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst) {
	pProgInst temp = (pProgInst)ctx;
	if (theInst<0 || theInst> InstNum || ctx==NULL) return -1;
	*src1DepInst = temp[theInst].depend1;
	*src2DepInst = temp[theInst].depend2 ;
	return 0;
}

/*
Name: getPtofDepth
Description: return the depth of the program- the logest path of dependencies
Parameters: ctx- the data structure of the instructions
return value: returns the longest path of dependencies
*/
int getProgDepth(ProgCtx ctx) {
    if(ctx==NULL) return 0;
	pProgInst temp = (pProgInst)ctx;
	int maxLatency = 0;

	for (unsigned int i = 0; i < InstNum; i++)
	{
		if ((temp[i].InstLatency + temp[i].cycleNum) >= maxLatency)
		{
			maxLatency = temp[i].InstLatency + temp[i].cycleNum;
		}
	}
	return maxLatency;
}


