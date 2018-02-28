#include "stdafx.h"
#include "ThreadPool.h"
#include <process.h>


ThreadPool::ThreadPool(size_t minNumOfThread, size_t maxNumOfThread)
{
	if (minNumOfThread < 2)
		this->minNumOfThread = 2;
	else
		this->minNumOfThread = minNumOfThread;
	if (maxNumOfThread < this->minNumOfThread * 2)
		this->maxNumOfThread = this->minNumOfThread * 2;
	else
		this->maxNumOfThread = maxNumOfThread;
	stopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);

	idleThreadList.clear();
	CreateIdleThread(this->minNumOfThread);
	busyThreadList.clear();

	dispatchThrad = (HANDLE)_beginthreadex(0, 0, GetTaskThreadProc, this, 0, 0);
	numOfLongFun = 0;
}

ThreadPool::~ThreadPool()
{
	SetEvent(stopEvent);
	PostQueuedCompletionStatus(completionPort, 0, (DWORD)EXIT, NULL);

	CloseHandle(stopEvent);
}

BOOL ThreadPool::QueueTaskItem(TaskFun task, PVOID param, TaskCallbackFun taskCb, BOOL longFun)
{
	waitTaskLock.Lock();
	WaitTask *waitTask = new WaitTask(task, param, taskCb, longFun);
	waitTaskList.push_back(waitTask);
	waitTaskLock.UnLock();
	PostQueuedCompletionStatus(completionPort, 0, (DWORD)GET_TASK, NULL);
	return TRUE;
}

void ThreadPool::CreateIdleThread(size_t size)
{
	idleThreadLock.Lock();
	for (size_t i = 0; i < size; i++)
	{
		idleThreadList.push_back(new Thread(this));
	}
	idleThreadLock.UnLock();
}

void ThreadPool::DeleteIdleThread(size_t size)
{
	idleThreadLock.Lock();
	size_t t = idleThreadList.size();
	if (t >= size)
	{
		for (size_t i = 0; i < size; i++)
		{
			auto thread = idleThreadList.back();
			delete thread;
			idleThreadList.pop_back();
		}
	}
	else
	{
		for (size_t i = 0; i < t; i++)
		{
			auto thread = idleThreadList.back();
			delete thread;
			idleThreadList.pop_back();
		}
	}
	idleThreadLock.UnLock();
}

ThreadPool::Thread *ThreadPool::GetIdleThread()
{
	Thread *thread = NULL;
	idleThreadLock.Lock();
	if (idleThreadList.size() > 0)
	{
		thread = idleThreadList.front();
		idleThreadList.pop_front();
	}
	idleThreadLock.UnLock();

	if (thread == NULL && getCurNumOfThread() < maxNumOfThread)
	{
		thread = new Thread(this);
	}

	if (thread == NULL && waitTaskList.size() > THRESHOLE_OF_WAIT_TASK)
	{
		thread = new Thread(this);
		InterlockedIncrement(&maxNumOfThread);
	}
	return thread;
}

void ThreadPool::MoveBusyThreadToIdleList(Thread * busyThread)
{
	idleThreadLock.Lock();
	idleThreadList.push_back(busyThread);
	idleThreadLock.UnLock();

	busyThreadLock.Lock();
	for (auto it = busyThreadList.begin(); it != busyThreadList.end(); it++)
	{
		if (*it == busyThread)
		{
			busyThreadList.erase(it);
			break;
		}
	}
	busyThreadLock.UnLock();

	if (maxNumOfThread != 0 && idleThreadList.size() > maxNumOfThread * 0.8)
	{
		DeleteIdleThread(idleThreadList.size() / 2);
	}

	PostQueuedCompletionStatus(completionPort, 0, (DWORD)GET_TASK, NULL);
}

void ThreadPool::MoveThreadToBusyList(Thread * thread)
{
	busyThreadLock.Lock();
	busyThreadList.push_back(thread);
	busyThreadLock.UnLock();
}

void ThreadPool::GetTaskExcute()
{
	Thread *thread = NULL;
	WaitTask *waitTask = NULL;

	waitTask = GetTask();
	if (waitTask == NULL)
	{
		return;
	}

	if (waitTask->bLong)
	{
		if (idleThreadList.size() > minNumOfThread)
		{
			thread = GetIdleThread();
		}
		else
		{
			thread = new Thread(this);
			InterlockedIncrement(&numOfLongFun);
			InterlockedIncrement(&maxNumOfThread);
		}
	}
	else
	{
		thread = GetIdleThread();
	}

	if (thread != NULL)
	{
		thread->ExecuteTask(waitTask->task, waitTask->param, waitTask->taskCb);
		delete waitTask;
		MoveThreadToBusyList(thread);
	}
	else
	{
		waitTaskLock.Lock();
		waitTaskList.push_front(waitTask);
		waitTaskLock.UnLock();
	}
	
}

ThreadPool::WaitTask *ThreadPool::GetTask()
{
	WaitTask *waitTask = NULL;
	waitTaskLock.Lock();
	if (waitTaskList.size() > 0)
	{
		waitTask = waitTaskList.front();
		waitTaskList.pop_front();
	}
	waitTaskLock.UnLock();
	return waitTask;
}


ThreadPool::Thread::Thread(ThreadPool *threadPool) :
	busy(FALSE),
	thread(INVALID_HANDLE_VALUE),
	task(NULL),
	taskCb(NULL),
	exit(FALSE),
	threadPool(threadPool)
{
	thread = (HANDLE)_beginthreadex(0, 0, ThreadProc, this, CREATE_SUSPENDED, 0);
}

ThreadPool::Thread::~Thread()
{
	exit = TRUE;
	task = NULL;
	taskCb = NULL;
	ResumeThread(thread);
	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
}

BOOL ThreadPool::Thread::isBusy()
{
	return busy;
}

void ThreadPool::Thread::ExecuteTask(TaskFun task, PVOID param, TaskCallbackFun taskCallback)
{
	busy = TRUE;
	this->task = task;
	this->param = param;
	this->taskCb = taskCallback;
	ResumeThread(thread);
}

unsigned int ThreadPool::Thread::ThreadProc(PVOID pM)
{
	Thread *pThread = (Thread*)pM;

	while (true)
	{
		if (pThread->exit)
			break; //Ïß³ÌÍË³ö

		if (pThread->task == NULL && pThread->taskCb == NULL)
		{
			pThread->busy = FALSE;
			pThread->threadPool->MoveBusyThreadToIdleList(pThread);
			SuspendThread(pThread->thread);
			continue;
		}

		int resulst = pThread->task(pThread->param);
		if(pThread->taskCb)
			pThread->taskCb(resulst);
		WaitTask *waitTask = pThread->threadPool->GetTask();
		if (waitTask != NULL)
		{
			pThread->task = waitTask->task;
			pThread->taskCb = waitTask->taskCb;
			delete waitTask;
			continue;
		}
		else
		{
			pThread->task = NULL;
			pThread->param = NULL;
			pThread->taskCb = NULL;
			pThread->busy = FALSE;
			pThread->threadPool->MoveBusyThreadToIdleList(pThread);
			SuspendThread(pThread->thread);
		}
	}

	return 0;
}
