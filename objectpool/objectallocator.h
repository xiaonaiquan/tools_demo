#pragma once
template <class T>
class ObjectAllocator
{
public:
	virtual ~ObjectAllocator() {}
	virtual T * Create() = 0;
	virtual void Destroy(T * pObject) = 0;
};