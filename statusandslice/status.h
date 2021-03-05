#pragma once
#include <utility>
#include "slice.h"

class Status
{
public:
	Status() noexcept:state_(nullptr) 
	{

	}
	~Status()
	{
		delete[] state_;
	}

	Status(const Status& rhs);
	Status& operator=(const Status& rhs);

	Status(Status&& rhs) noexcept : state_(rhs.state_) { rhs.state_ = nullptr; }
	Status& operator=(Status&& rhs) noexcept;

	static Status OK() { return Status(); }

	
	static Status NotFound(const Slice& msg, const Slice& msg2 = Slice()) 
	{
		return Status(Code::kNotFound, msg, msg2);
	}
	static Status Corruption(const Slice& msg, const Slice& msg2 = Slice()) 
	{
		return Status(Code::kCorruption, msg, msg2);
	}
	static Status NotSupported(const Slice& msg, const Slice& msg2 = Slice()) 
	{
		return Status(Code::kNotSupported, msg, msg2);
	}
	static Status InvalidArgument(const Slice& msg, const Slice& msg2 = Slice()) 
	{
		return Status(Code::kInvalidArgument, msg, msg2);
	}
	static Status IOError(const Slice& msg, const Slice& msg2 = Slice()) 
	{
		return Status(Code::kIOError, msg, msg2);
	}

	// Returns true iff the status indicates success.
	bool ok() const { return (state_ == nullptr); }

	// Returns true iff the status indicates a NotFound error.
	bool IsNotFound() const { return code() == Code::kNotFound; }

	// Returns true iff the status indicates a Corruption error.
	bool IsCorruption() const { return code() == Code::kCorruption; }

	// Returns true iff the status indicates an IOError.
	bool IsIOError() const { return code() == Code::kIOError; }

	// Returns true iff the status indicates a NotSupportedError.
	bool IsNotSupportedError() const { return code() == Code::kNotSupported; }

	// Returns true iff the status indicates an InvalidArgument.
	bool IsInvalidArgument() const { return code() == Code::kInvalidArgument; }

	std::string ToString() const;
private:
	enum class Code {   // 枚举类型不能隐式地转换为整数；也无法与整数数值做比较
		kOk = 0,
		kNotFound = 1,
		kCorruption = 2,
		kNotSupported = 3,
		kInvalidArgument = 4,
		kIOError = 5
	};

	Code code() const 
	{
		return (state_ == nullptr) ? Code::kOk : static_cast<Code>(state_[4]);
	}

	Status(Code code, const Slice& msg, const Slice& msg2);

	static const char* CopyState(const char* state);

	const char* state_;
};


inline Status::Status(const Status& rhs) 
{
	state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
}

inline Status& Status::operator=(const Status& rhs) 
{
	if (state_ != rhs.state_) 
	{
		delete[] state_;
		state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
	}
	return *this;
}

inline Status& Status::operator=(Status&& rhs) noexcept 
{
	std::swap(state_, rhs.state_);
	return *this;
}