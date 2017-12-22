#include "Thread.h"
#include "MsgQueue.h"
#include "_Thread.h"

Message new_message[100];
Qcb new_qcb[100];
Thread* returnThread;
	

void _InitMsgQueue(void)
{
	int i;
	for(i=0;i<MAX_QCB_SIZE;i++){
		qcbTblEntry[i].key = -1;
		qcbTblEntry[i].pQcb = NULL;
	}
	memset(new_message, 0, sizeof(new_message));
	memset(new_qcb, 0, sizeof(new_qcb));
}





int 	mymsgget(int key, int msgflg)
{
	int i;
	//if key is not in table or match parmeter key, break loop.
	for(i=0; (qcbTblEntry[i].key != -1) && (qcbTblEntry[i].key != key); i++); 

	qcbTblEntry[i].key = key;
	qcbTblEntry[i].pQcb = &new_qcb[i];
	return i;

}



int	mymsgsnd(int msqid, const void *msgp, int msgsz, int msgflg){

	int j;
	for(j=0;j<100;j++){
		if(new_message[j].type == 0)
			break;
	}

	new_message[j].type = ((Message *)msgp)->type;
	strcpy(new_message[j].data, ((Message *)msgp)->data);
	Message* msgTemp;

	if(qcbTblEntry[msqid].pQcb->pMsgHead == NULL){
		qcbTblEntry[msqid].pQcb->pMsgHead = &new_message[j];
		qcbTblEntry[msqid].pQcb->pMsgTail = qcbTblEntry[msqid].pQcb->pMsgHead;
		qcbTblEntry[msqid].pQcb->pMsgTail->pNext = NULL;
		qcbTblEntry[msqid].pQcb->pMsgTail->pPrev = NULL;
	}
	else{
		qcbTblEntry[msqid].pQcb->pMsgTail->pNext = &new_message[j];
		msgTemp = qcbTblEntry[msqid].pQcb->pMsgTail;
		qcbTblEntry[msqid].pQcb->pMsgTail = qcbTblEntry[msqid].pQcb->pMsgTail->pNext;
		qcbTblEntry[msqid].pQcb->pMsgTail->pPrev = msgTemp;
	}
	printf("msg list\n");
	for(msgTemp = qcbTblEntry[msqid].pQcb->pMsgHead; msgTemp != NULL; msgTemp = msgTemp->pNext){
		printf("(%d)[%d]: %s  ", thread_self(), msgTemp->type, msgTemp->data);
	}
	printf("\n");



	//check if something is in waiting list. 
	//if so, remove first met TCB out of the list and push in ready queue.
	if(qcbTblEntry[msqid].pQcb->pThreadHead != NULL){
		Thread *temp,*var,*t, *temp1;
		temp = qcbTblEntry[msqid].pQcb->pThreadHead;
		//if temp become null as a result of while,
		//just return with Message put in Message list.
			while(temp != NULL) 
			{
				if(temp->type == new_message[j].type)
				{
					if(temp->pPrev == NULL)
					{
						returnThread = temp;
						returnThread->status = THREAD_STATUS_READY;
						r_enqueue(rq, returnThread);
						temp = NULL;
						qcbTblEntry[msqid].pQcb->pThreadHead = NULL;
						qcbTblEntry[msqid].pQcb->pThreadTail = NULL;
						return new_message[j].size;
					}
					else
					{
						returnThread = temp;
						returnThread->status = THREAD_STATUS_READY;
						r_enqueue(rq, returnThread);
						var->pNext = temp1;
						temp->pPrev = NULL;
						temp = NULL;
						if(temp1 != NULL)
							temp1->pPrev = var;

						return new_message[j].size;
					}
				}
				else
				{
					var = temp;
					temp = temp->pNext;
					if(temp != NULL)
						temp1 = temp->pNext;

				}
			}
		
	}


}



int	mymsgrcv(int msqid, void *msgp, int msgsz, long msgtyp, int msgflg)
{  
	printf("(%d)rcv called\n", thread_self());
	//find target and if found, remove that.
	Message* copyMsgp = (Message*)msgp;
	Message *temp,*var,*t, *temp1;
	temp = qcbTblEntry[msqid].pQcb->pMsgHead;

	//something is in msg list.
	while(temp != NULL)
	{
		if(temp->type == msgtyp)
		{
			if(temp->pPrev == NULL)
			{
				memcpy(copyMsgp->data, temp->data, strlen(temp->data)+1);
				copyMsgp->type = temp->type;
				copyMsgp->size = msgsz;

				if(qcbTblEntry[msqid].pQcb->pMsgHead->pNext != NULL){
					Message* msgTemp = qcbTblEntry[msqid].pQcb->pMsgHead;
					qcbTblEntry[msqid].pQcb->pMsgHead = qcbTblEntry[msqid].pQcb->pMsgHead->pNext;
					msgTemp->pNext = NULL;
					qcbTblEntry[msqid].pQcb->pMsgHead->pPrev = NULL;
					return copyMsgp->size;
				}

				qcbTblEntry[msqid].pQcb->pMsgHead = NULL;
				qcbTblEntry[msqid].pQcb->pMsgTail = NULL;
				return copyMsgp->size;	
			}
			else
			{
				memcpy(copyMsgp->data, temp->data, strlen(temp->data)+1);
				var->pNext = temp1;
				if(temp1 != NULL)
					temp1->pPrev = var;

				copyMsgp->type = temp->type;
				copyMsgp->size = msgsz;
				temp->pPrev = NULL;
				temp = NULL;
				return copyMsgp->size;
			}
		}
		else
		{
			var = temp;
			temp = temp->pNext;
			if(temp != NULL)
				temp1 = temp->pNext;
		}
	}

	//nothing in list when mymsgrcv is called
	if(temp == NULL){

		int pid = thread_self();
		//global variable refered to get target address and push into waiting list. 
		Thread* target;
		while(current_thread == NULL);
		do{
			target = current_thread;
		}while(target->pid != pid);
		current_thread = NULL;
		target->status = THREAD_STATUS_BLOCKED;
		target->type = msgtyp;

		//insert waiting list
		Thread* Ttemp;
		
		if(qcbTblEntry[msqid].pQcb->pThreadHead==NULL)
		{
			qcbTblEntry[msqid].pQcb->pThreadHead=target;
			qcbTblEntry[msqid].pQcb->pThreadHead->pPrev=NULL;
			qcbTblEntry[msqid].pQcb->pThreadHead->pNext=NULL;
			qcbTblEntry[msqid].pQcb->pThreadTail=qcbTblEntry[msqid].pQcb->pThreadHead;
		}
		else
		{
			qcbTblEntry[msqid].pQcb->pThreadTail=qcbTblEntry[msqid].pQcb->pThreadHead;

			while(qcbTblEntry[msqid].pQcb->pThreadTail != NULL)
			{
				Ttemp=qcbTblEntry[msqid].pQcb->pThreadTail;
				qcbTblEntry[msqid].pQcb->pThreadTail=qcbTblEntry[msqid].pQcb->pThreadTail->pNext;
			}
			qcbTblEntry[msqid].pQcb->pThreadTail=target;
			Ttemp->pNext=qcbTblEntry[msqid].pQcb->pThreadTail;
			qcbTblEntry[msqid].pQcb->pThreadTail->pPrev=Ttemp;
			qcbTblEntry[msqid].pQcb->pThreadTail->pNext=NULL;
		}

		Thread* ThreadTemp;
		printf("wait list\n");
		for(ThreadTemp = qcbTblEntry[msqid].pQcb->pThreadHead; ThreadTemp != NULL; ThreadTemp = ThreadTemp->pNext){
			printf("[%d]  ", ThreadTemp->type);
		}
		printf("\n");
		while(1){
			while(temp == NULL){
				temp = qcbTblEntry[msqid].pQcb->pMsgHead;
				int i,j,k;
				for(i=0;i<10000;i++)
					for(j=0;j<10000;j++);
			}
			while(temp != NULL){		
				if(temp->type == msgtyp)
				{
					if(temp->pPrev == NULL)
					{
						memcpy(copyMsgp->data, temp->data, strlen(temp->data)+1); //modify MAX_MSG_SIZE to that.
						copyMsgp->type = temp->type;
						copyMsgp->size = msgsz;
						if(qcbTblEntry[msqid].pQcb->pMsgHead->pNext != NULL){
							Message* msgTemp = qcbTblEntry[msqid].pQcb->pMsgHead;
							qcbTblEntry[msqid].pQcb->pMsgHead = qcbTblEntry[msqid].pQcb->pMsgHead->pNext;
							msgTemp->pNext = NULL;
							qcbTblEntry[msqid].pQcb->pMsgHead->pPrev = NULL;
							return copyMsgp->size;
						}

						qcbTblEntry[msqid].pQcb->pMsgHead = NULL;
						qcbTblEntry[msqid].pQcb->pMsgTail = NULL;
						return copyMsgp->size;	
					}
					else
					{

						memcpy(copyMsgp->data, temp->data, strlen(temp->data)+1); //modify MAX_MSG_SIZE to that.
						copyMsgp->type = temp->type;
						copyMsgp->size = msgsz;
						temp->pPrev = NULL;
						temp = NULL;
						var->pNext = temp1;
						if(temp1 != NULL)
							temp1->pPrev = var;
						return copyMsgp->size;
					}

				}
				else
				{
					var = temp;
					temp = temp->pNext;
					if(temp != NULL)
						temp1 = temp->pNext;

				}
			}
		}
	}

	return 0;
}



int mymsgctl(int msqid, int cmd, void* buf)
{
	//entry reset, qcb deleted
	//cmd : IPC_RMID
	//buf : NULL

//	int pid = thread_self();
//	Thread* target;

//	do{
//		target = current_thread;
//	}while(target->pid != pid);
//	current_thread = r_dequeue(rq);
//	free(target);
	printf("mymsgctl is called\n");

	//free nodes of list and reset message.
	//only qcb matched at msqid is deleted.
	Message* tempHead;
	while(qcbTblEntry[msqid].pQcb->pMsgHead != NULL){
		tempHead = qcbTblEntry[msqid].pQcb->pMsgHead;
		qcbTblEntry[msqid].pQcb->pMsgHead = qcbTblEntry[msqid].pQcb->pMsgHead->pNext;
		memset(tempHead, 0, sizeof(Message));
		tempHead = NULL;
	}
	Thread* temp;
	while(qcbTblEntry[msqid].pQcb->pThreadHead != NULL){
		temp = qcbTblEntry[msqid].pQcb->pThreadHead;
		qcbTblEntry[msqid].pQcb->pThreadHead = qcbTblEntry[msqid].pQcb->pThreadHead->pNext;
		memset(temp, 0, sizeof(Thread));
		temp = NULL;
	}
	memset(qcbTblEntry[msqid].pQcb, 0, sizeof(Qcb));

	return 0;
}
