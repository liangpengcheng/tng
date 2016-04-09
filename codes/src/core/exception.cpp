

#include "core/exception.h"

#include "core/log.h"
namespace tng {


	Exception::Exception(int code) : exp_nested_(0), code_(code)
	{
	}


	Exception::Exception(const string& msg, int code) : str_msg_(msg), exp_nested_(0), code_(code)
	{
		Log::GetLog()->Printf(Log::ERROR_CHN, msg.c_str());
	}


	Exception::Exception(const string& msg, const string& arg, int code) : str_msg_(msg), exp_nested_(0), code_(code)
	{
		if (!arg.empty())
		{
			str_msg_.append(": ");
			str_msg_.append(arg);
		}
		Log::GetLog()->Printf(Log::ERROR_CHN, msg.c_str());
	}


	Exception::Exception(const string& msg, const Exception& nested, int code) : str_msg_(msg), exp_nested_(nested.clone()), code_(code)
	{
		Log::GetLog()->Printf(Log::ERROR_CHN, msg.c_str());
	}


	Exception::Exception(const Exception& exc) :
		std::exception(exc),
		str_msg_(exc.str_msg_),
		code_(exc.code_)
	{
			exp_nested_ = exc.exp_nested_ ? exc.exp_nested_->clone() : 0;
			Log::GetLog()->Printf(Log::ERROR_CHN, str_msg_.c_str());
		}


	Exception::~Exception() throw()
	{
		delete exp_nested_;
	}


	Exception& Exception::operator = (const Exception& exc)
	{
		if (&exc != this)
		{
			delete exp_nested_;
			str_msg_ = exc.str_msg_;
			exp_nested_ = exc.exp_nested_ ? exc.exp_nested_->clone() : 0;
			code_ = exc.code_;
		}
		return *this;
	}


	const char* Exception::name() const throw()
	{
		return "Exception";
	}


	const char* Exception::className() const throw()
	{
		return "Exception";
	}


	const char* Exception::what() const throw()
	{
		return name();
	}


	string Exception::displayText() const
	{
		string txt = name();
		if (!str_msg_.empty())
		{
			txt.append(": ");
			txt.append(str_msg_);
		}
		return txt;
	}


	void Exception::extendedMessage(const string& arg)
	{
		if (!arg.empty())
		{
			if (!str_msg_.empty()) str_msg_.append(": ");
			str_msg_.append(arg);
		}
	}


	Exception* Exception::clone() const
	{
		return new Exception(*this);
	}


	void Exception::rethrow() const
	{
		throw *this;
	}


	_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
	_IMPLEMENT_EXCEPTION(AssertionViolationException, LogicException, "Assertion violation")
	_IMPLEMENT_EXCEPTION(NullPointerException, LogicException, "Null pointer")
	_IMPLEMENT_EXCEPTION(NullValueException, LogicException, "Null value")
	_IMPLEMENT_EXCEPTION(BugcheckException, LogicException, "Bugcheck")
	_IMPLEMENT_EXCEPTION(IllegalStateException, LogicException, "Illegal state")
	_IMPLEMENT_EXCEPTION(InvalidAccessException, LogicException, "Invalid access")
	_IMPLEMENT_EXCEPTION(SignalException, LogicException, "Signal received")
	_IMPLEMENT_EXCEPTION(UnhandledException, LogicException, "Unhandled exception")

	_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
	_IMPLEMENT_EXCEPTION(NotFoundException, RuntimeException, "Not found")
	_IMPLEMENT_EXCEPTION(ExistsException, RuntimeException, "Exists")
	_IMPLEMENT_EXCEPTION(SystemException, RuntimeException, "System exception")
	_IMPLEMENT_EXCEPTION(LibraryLoadException, RuntimeException, "Cannot load library")
	_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException, "Library already loaded")
	_IMPLEMENT_EXCEPTION(NoThreadAvailableException, RuntimeException, "No thread available")
	_IMPLEMENT_EXCEPTION(PropertyNotSupportedException, RuntimeException, "Property not supported")
	_IMPLEMENT_EXCEPTION(PoolOverflowException, RuntimeException, "Pool overflow")
	_IMPLEMENT_EXCEPTION(NoPermissionException, RuntimeException, "No permission")
	_IMPLEMENT_EXCEPTION(OutOfMemoryException, RuntimeException, "Out of memory")
	_IMPLEMENT_EXCEPTION(DataException, RuntimeException, "Data error")

	_IMPLEMENT_EXCEPTION(SyntaxException, DataException, "Syntax error")
	_IMPLEMENT_EXCEPTION(PathSyntaxException, SyntaxException, "Bad path syntax")
	_IMPLEMENT_EXCEPTION(IOException, RuntimeException, "I/O error")
	_IMPLEMENT_EXCEPTION(FileException, IOException, "File access error")
	_IMPLEMENT_EXCEPTION(FileExistsException, FileException, "File exists")
	_IMPLEMENT_EXCEPTION(FileNotFoundException, FileException, "File not found")
	_IMPLEMENT_EXCEPTION(PathNotFoundException, FileException, "Path not found")
	_IMPLEMENT_EXCEPTION(FileReadOnlyException, FileException, "File is read-only")
	_IMPLEMENT_EXCEPTION(FileAccessDeniedException, FileException, "Access to file denied")
	_IMPLEMENT_EXCEPTION(CreateFileException, FileException, "Cannot create file")
	_IMPLEMENT_EXCEPTION(OpenFileException, FileException, "Cannot open file")
	_IMPLEMENT_EXCEPTION(WriteFileException, FileException, "Cannot write file")
	_IMPLEMENT_EXCEPTION(ReadFileException, FileException, "Cannot read file")



	_IMPLEMENT_EXCEPTION(ApplicationException, Exception, "Application exception")
	_IMPLEMENT_EXCEPTION(BadCastException, RuntimeException, "Bad cast exception")

} // 
