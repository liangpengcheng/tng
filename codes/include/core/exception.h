#pragma once
#include <string>
#include <stdexcept>
#include "core/core_macros.h"
#include "memobj.h"
#include "core/stl_rebind.h"
namespace tng {

//from poco
class CORE_API Exception: public std::exception,public MemObj
    /// This is the base class for all exceptions defined
    /// in the TNG class library.
{
public:
    Exception(const string& msg, int code = 0);
    /// Creates an exception.

    Exception(const string& msg, const string& arg, int code = 0);
    /// Creates an exception.

    Exception(const string& msg, const Exception& nested, int code = 0);
    /// Creates an exception and stores a clone
    /// of the nested exception.

    Exception(const Exception& exc);
    /// Copy constructor.

    ~Exception() throw();
    /// Destroys the exception and deletes the nested exception.

    Exception& operator = (const Exception& exc);
    /// Assignment operator.

    virtual const char* name() const throw();
    /// Returns a static string describing the exception.

    virtual const char* className() const throw();
    /// Returns the name of the exception class.

    virtual const char* what() const throw();
    /// Returns a static string describing the exception.
    ///
    /// Same as name(), but for compatibility with std::exception.

    const Exception* nested() const;
    /// Returns a pointer to the nested exception, or
    /// null if no nested exception exists.

    const string& message() const;
    /// Returns the message text.

    int code() const;
    /// Returns the exception code if defined.

    string displayText() const;
    /// Returns a string consisting of the
    /// message name and the message text.

    virtual Exception* clone() const;
    /// Creates an exact copy of the exception.
    ///
    /// The copy can later be thrown again by
    /// invoking rethrow() on it.

    virtual void rethrow() const;
    /// (Re)Throws the exception.
    ///
    /// This is useful for temporarily storing a
    /// copy of an exception (see clone()), then
    /// throwing it again.

protected:
    Exception(int code = 0);
    /// Standard constructor.

    void message(const string& msg);
    /// Sets the message for the exception.

    void extendedMessage(const string& arg);
    /// Sets the extended message for the exception.

private:
    string str_msg_;
    Exception*  exp_nested_;
    int			code_;
};


//
// inlines
//
inline const Exception* Exception::nested() const
{
    return exp_nested_;
}


inline const string& Exception::message() const
{
    return str_msg_;
}


inline void Exception::message(const string& msg)
{
    str_msg_ = msg;
}


inline int Exception::code() const
{
    return code_;
}


//
// Macros for quickly declaring and implementing exception classes.
// Unfortunately, we cannot use a template here because character
// pointers (which we need for specifying the exception name)
// are not allowed as template arguments.
//
#define _DECLARE_EXCEPTION_CODE(API, CLS, BASE, CODE) \
	class API CLS: public BASE														\
	{																				\
	public:																			\
		CLS(int code = CODE);														\
		CLS(const string& msg, int code = CODE);								\
		CLS(const string& msg, const string& arg, int code = CODE);		\
		CLS(const string& msg, const tng::Exception& exc, int code = CODE);	\
		CLS(const CLS& exc);														\
		~CLS() throw();																\
		CLS& operator = (const CLS& exc);											\
		const char* name() const throw();											\
		const char* className() const throw();										\
		tng::Exception* clone() const;												\
		void rethrow() const;														\
	};

#define _DECLARE_EXCEPTION(API, CLS, BASE) \
	_DECLARE_EXCEPTION_CODE(API, CLS, BASE, 0)

#define _IMPLEMENT_EXCEPTION(CLS, BASE, NAME)													\
	CLS::CLS(int code): BASE(code)																	\
	{																								\
	}																								\
	CLS::CLS(const string& msg, int code): BASE(msg, code)										\
	{																								\
	}																								\
	CLS::CLS(const string& msg, const string& arg, int code): BASE(msg, arg, code)		\
	{																								\
	}																								\
	CLS::CLS(const string& msg, const tng::Exception& exc, int code): BASE(msg, exc, code)	\
	{																								\
	}																								\
	CLS::CLS(const CLS& exc): BASE(exc)																\
	{																								\
	}																								\
	CLS::~CLS() throw()																				\
	{																								\
	}																								\
	CLS& CLS::operator = (const CLS& exc)															\
	{																								\
		BASE::operator = (exc);																		\
		return *this;																				\
	}																								\
	const char* CLS::name() const throw()															\
	{																								\
		return NAME;																				\
	}																								\
	const char* CLS::className() const throw()														\
	{																								\
		return #BASE;																\
	}																								\
	tng::Exception* CLS::clone() const																\
	{																								\
		return new CLS(*this);																		\
	}																								\
	void CLS::rethrow() const																		\
	{																								\
		throw *this;																				\
	}


//
// Standard exception classes
//
_DECLARE_EXCEPTION(CORE_API, LogicException, Exception)
_DECLARE_EXCEPTION(CORE_API, AssertionViolationException, LogicException)
_DECLARE_EXCEPTION(CORE_API, NullPointerException, LogicException)
_DECLARE_EXCEPTION(CORE_API, NullValueException, LogicException)
_DECLARE_EXCEPTION(CORE_API, BugcheckException, LogicException)
_DECLARE_EXCEPTION(CORE_API, IllegalStateException, LogicException)
_DECLARE_EXCEPTION(CORE_API, InvalidAccessException, LogicException)
_DECLARE_EXCEPTION(CORE_API, SignalException, LogicException)
_DECLARE_EXCEPTION(CORE_API, UnhandledException, LogicException)

_DECLARE_EXCEPTION(CORE_API, RuntimeException, Exception)
_DECLARE_EXCEPTION(CORE_API, NotFoundException, RuntimeException)
_DECLARE_EXCEPTION(CORE_API, ExistsException, RuntimeException)
_DECLARE_EXCEPTION(CORE_API, SystemException, RuntimeException)
_DECLARE_EXCEPTION(CORE_API, LibraryLoadException, RuntimeException)
_DECLARE_EXCEPTION(CORE_API, LibraryAlreadyLoadedException, RuntimeException)
_DECLARE_EXCEPTION(CORE_API, NoThreadAvailableException, RuntimeException)
_DECLARE_EXCEPTION(CORE_API, PropertyNotSupportedException, RuntimeException)
_DECLARE_EXCEPTION(CORE_API, PoolOverflowException, RuntimeException)
_DECLARE_EXCEPTION(CORE_API, NoPermissionException, RuntimeException)
_DECLARE_EXCEPTION(CORE_API, OutOfMemoryException, RuntimeException)
_DECLARE_EXCEPTION(CORE_API, DataException, RuntimeException)
_DECLARE_EXCEPTION(CORE_API, SyntaxException, DataException)
_DECLARE_EXCEPTION(CORE_API, PathSyntaxException, SyntaxException)
_DECLARE_EXCEPTION(CORE_API, IOException, RuntimeException)
_DECLARE_EXCEPTION(CORE_API, FileException, IOException)
_DECLARE_EXCEPTION(CORE_API, FileExistsException, FileException)
_DECLARE_EXCEPTION(CORE_API, FileNotFoundException, FileException)
_DECLARE_EXCEPTION(CORE_API, PathNotFoundException, FileException)
_DECLARE_EXCEPTION(CORE_API, FileReadOnlyException, FileException)
_DECLARE_EXCEPTION(CORE_API, FileAccessDeniedException, FileException)
_DECLARE_EXCEPTION(CORE_API, CreateFileException, FileException)
_DECLARE_EXCEPTION(CORE_API, OpenFileException, FileException)
_DECLARE_EXCEPTION(CORE_API, WriteFileException, FileException)
_DECLARE_EXCEPTION(CORE_API, ReadFileException, FileException)

_DECLARE_EXCEPTION(CORE_API, ApplicationException, Exception)
_DECLARE_EXCEPTION(CORE_API, BadCastException, RuntimeException)


} //


