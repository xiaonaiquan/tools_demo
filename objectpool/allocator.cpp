#include "allocator.h"
#include <stdlib.h>
#include <new>
#include "stacktrace.h"

void * MallocAllocator::Allocate(size_t iSize, size_t * pRealSize)
{
	_AC_STACK_TRACE();
	if (pRealSize != NULL) {
		*pRealSize = iSize;
	}
	return malloc(iSize);
}

void MallocAllocator::Deallocate(void * pData)
{
	_AC_STACK_TRACE();
	return free(pData);
}

MallocAllocator * MallocAllocator::Instance()
{
	static MallocAllocator oMallocAllocator;
	return &oMallocAllocator;
}

void * NewAllocator::Allocate(size_t iSize, size_t * pRealSize)
{
	_AC_STACK_TRACE();
	if (pRealSize != NULL) {
		*pRealSize = iSize;
	}
	return ::operator new(iSize, ::std::nothrow_t());
}

void NewAllocator::Deallocate(void * pData)
{
	_AC_STACK_TRACE();
	return ::operator delete(pData, ::std::nothrow_t());
}

NewAllocator * NewAllocator::Instance()
{
	static NewAllocator oNewAllocator;
	return &oNewAllocator;
}


Allocator * Allocator::Instance()
{
	return NewAllocator::Instance();
}