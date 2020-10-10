#pragma once

class Destroyable;
struct DestroyCallback
{
	virtual ~DestroyCallback() {};
	virtual void call(Destroyable* p) = 0;
};

struct DeleteDestroyCallback : public DestroyCallback
{
	static DeleteDestroyCallback* Instance();

	virtual void call(Destroyable* p);
};

class Destroyable
{
public:

	Destroyable() : pCB_(0) {}
	virtual ~Destroyable() {}

	Destroyable(const Destroyable&) : pCB_(0) {}
	Destroyable& operator=(const Destroyable&) { return *this; }

	void Destroy()
	{
		if (pCB_) {
			pCB_->call(this);
		}
	}

	inline void SetDestroyCallback(DestroyCallback * pCB)
	{
		pCB_ = pCB;
	}

	inline DestroyCallback * GetDestroyCallback() const
	{
		return pCB_;
	}

private:
	DestroyCallback * pCB_;
};