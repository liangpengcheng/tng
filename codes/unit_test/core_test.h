#pragma once

#include "core/types.h"
#include "core/refobject.h"
#include "core/os.h"
#include "core/stringfun.h"
#include "core/threadfun.h"
#include "core/autoptr.h"
#include "core/filesystem.h"
#include "core/filestream.h"
#include "core/binary_reader_writer.h"
#include "core/path.h"
#include "core/variant.h"
using namespace tng;
TEST(core, os) {
//     tng::cout<<TNG::OS::GetCPUInfo()<<tng::endl;
//     tng::cout<<TNG::OS::GetOSType()<<tng::endl;
//     tng::cout<<TNG::OS::GetOSRelease()<<tng::endl;
// 	tng::cout<<tng::string("total memory :")<<TNG::OS::GetTotalMemory()<<tng::endl;
// 	tng::cout<<tng::string("free memory :")<<TNG::OS::GetFreeMemory()<<tng::endl;
}
TEST(core, type) {
    ASSERT_EQ(sizeof(s64),		8);
    ASSERT_EQ(sizeof(s32),		4);
    ASSERT_EQ(sizeof(u64),		8);
    ASSERT_EQ(sizeof(u32),		4);

    ASSERT_EQ(sizeof(f64),		8);
    ASSERT_EQ(sizeof(f32),		4);

    ASSERT_EQ(sizeof(u16),		2);
    ASSERT_EQ(sizeof(s16),		2);
    ASSERT_EQ(sizeof(s8),		1);
    ASSERT_EQ(sizeof(u8),		1);
    ASSERT_EQ(sizeof(ubyte),	1);
}
TEST(core, RefCountedObject) {
    RefCountedObject* obj = new RefCountedObject;
    ASSERT_EQ(obj->ReferenceCount(), 1);
    obj->Duplicate();
    ASSERT_EQ(obj->ReferenceCount(), 2);
    obj->Release();
    obj->Release();
    AutoPtr<RefCountedObject> ptr(new RefCountedObject);
}

TEST(core, stringfun) {

	ASSERT_STREQ((str(format("%d,%f")%2%1.1f)).c_str(),"2,1.1");
	

	tng::wstring ustr = L"teststring";
	tng::string str;
	ToUTF8(ustr,str);
	ASSERT_STREQ(str.c_str(),"teststring");

	tng::string utf8str;
	utf8str.push_back(-26);
	utf8str.push_back(-75);
	utf8str.push_back(-117);
	utf8str.push_back(-24);
	utf8str.push_back(-81);
	utf8str.push_back(-107);
	
	utf8str.push_back('\0');

	tng::wstring utf16;
	ToWideString(utf8str,utf16);
	tng::wstring rutf16;
	rutf16.push_back(27979);
	rutf16.push_back(35797);
	ASSERT_STREQ(utf16.c_str(),rutf16.c_str());

}
TEST(core, mutex) {
	Mutex mt;
	mt.lock();
	mt.unlock();
#if !defined(NO_LIBUV)&&!defined(EMCC)
	RWLock rwl;
	rwl.rdlock();
	rwl.rdunlock();
	rwl.wrlock();
	rwl.wrunlock();

	Semaphore sem(3);
	sem.wait();
	sem.wait();
	ASSERT_EQ(sem.trywait(),0);
	ASSERT_EQ(sem.trywait(),-1);
	sem.post();
	ASSERT_EQ(sem.trywait(),0);
	ASSERT_EQ(sem.trywait(),-1);
#endif
}
class test_thread
{
public:
    test_thread() {

    }
    void doWork(void*)
    {
        mutex.lock();
        addval =0;
        for (int i =0; i <1000; i++)
        {
            addval+=i;
			couner++;
        }
        mutex.unlock();
    }
    int addval;
    Mutex mutex;
	static AtomicCounter couner;
};
AtomicCounter test_thread::couner;
TEST(core, threadfun) {

    test_thread t1;
    test_thread t2;
    int addval =0;
    for (int i =0; i <1000; i++)
    {
        addval+=i;
    }
    Thread::ThreadFun fun1(&t1,&test_thread::doWork);
    Thread* pthread1 = new Thread(fun1,NULL);
    Thread::ThreadFun fun2(&t2,&test_thread::doWork);
    Thread* pthread2 = new Thread(fun2,NULL);

    delete pthread2;
    delete pthread1;

    t1.mutex.lock();
    t2.mutex.lock();

    ASSERT_EQ(t1.addval,addval);
    ASSERT_EQ(t2.addval,addval);
	
    t1.mutex.unlock();
    t2.mutex.unlock();

	ASSERT_EQ(test_thread::couner --,2000);
	ASSERT_EQ(test_thread::couner,1999);
	ASSERT_EQ(--test_thread::couner,1998);

	ASSERT_EQ(test_thread::couner ++,1998);
	ASSERT_EQ(test_thread::couner,1999);
	ASSERT_EQ(++test_thread::couner,2000);;

	test_thread::couner = 0;
	ASSERT_EQ(test_thread::couner,0);


}

TEST(core, filesystem) {
	Path::GetExePath();
	Path p1("testDir");

	tng::wstring utf16str;
	tng::string  utf8str(p1);
	ToWideString(utf8str,utf16str);
#ifdef TNG_OS_FAMILY_WINDOWS
	ASSERT_STREQ(utf16str.c_str(),(tng::wstring(p1)).c_str());
#endif // TNG_OS_FAMILY_WINDOWS

	
	if(FileSystem::IsFileExist("test"))
		ASSERT_EQ(true,FileSystem::DeleteDir(tng::string("test")));
    ASSERT_EQ(true,FileSystem::CreateDir(tng::string("test")));
	ASSERT_EQ(true,FileSystem::IsFileExist(tng::string("test")));
    ASSERT_EQ(true,FileSystem::DeleteDir(tng::string("test")));
	FileSystem::CreateDirs(tng::string("1\\2\\3"));
	ASSERT_EQ(true,FileSystem::IsFileExist(tng::string("1/2/3")));
	tng::vector<Path> ret;
	FileSystem::FindFiles("1",ret,FileSystem::FFile|FileSystem::FDir,1);
	ASSERT_EQ(2,ret.size());
	for (size_t i = 0 ;i < ret.size();i ++)
	{
		ASSERT_EQ(true,FileSystem::DeleteDir(ret[i]));
	}
	ASSERT_EQ(true,FileSystem::DeleteDir("1"));

	Path splittest("../test/test.txt");
	tng::string dir,fn,ext;
	splittest.Split(dir,fn,ext);
	ASSERT_STREQ(dir.c_str(),"../test/");
	ASSERT_STREQ(fn.c_str(),"test");
	ASSERT_STREQ(ext.c_str(),"txt");

	FileOutputStream stream("test.bin");
	BinaryWriter bw(stream);
	bw<<tng::string("Hello_world!");
	stream.close();
	ASSERT_EQ(true,FileSystem::IsFileExist("test.bin"));

	FileInputStream instream("test.bin");
	BinaryReader br(instream);
	tng::string val;
	br>>val;
	ASSERT_STREQ(val.c_str(),"Hello_world!");
}
class factoryBase
{
public:
	virtual const char* get() =0;
};

class factoryA:public factoryBase
{
public:
	virtual const char* get(){return "A";}
};

class factoryB:public factoryBase
{
public:
	virtual const char* get(){return "B";}
};
#include "core/dynamic_factory.h"
TEST(core, dynamicfactory) {
	DynamicFactory<factoryBase,tng::string> factory;
	factory.RegisterClass<factoryA>("factoryA");
	factory.RegisterClass<factoryB>("factoryB");

	factoryBase* p1 = factory.CreateObject("factoryA");
	ASSERT_STREQ(p1->get(),"A");
	factoryBase* p2 = factory.CreateObject("factoryB");
	ASSERT_STREQ(p2->get(),"B");
	delete p1;
	delete p2;
}
#include "core/object.h"
class Obj2:public Object
{
public:
	ENGINE_CLASS_DEF
};
START_SUB_CLASS(Obj2,Object)
END_CLASS
#ifdef USE_RTTI
TEST(core, rtti) {
	Obj2::InitStatic();
	Object* obj = Object::CreateObject("Object");
	ASSERT_EQ(obj->IsType(Object::GetRtti()),true);
	Object* obj2 = Object::CreateObject("Obj2");
	ASSERT_EQ(obj2->IsType(Object::GetRtti()),false);
	ASSERT_EQ(obj2->IsDerivedFrom(Object::GetRtti()),true);
	ASSERT_EQ(obj2->IsType<Obj2>(),true);
	ASSERT_EQ(obj->DynamicCast<Obj2>(),(Object*)(NULL));
	ASSERT_EQ(obj2->DynamicCast<Obj2>(),obj2);
	
	obj->Release();
	obj2->Release();
}
#endif
class accesser_test
{
public:
	void	SetData(s32 d){data_ = d;}
	s32		GetData()const{return data_;}
private:
	s32		data_;
};
TEST(core, accesser) {
	accesser_test testobj;
	Accesser<accesser_test,s32> acc(&accesser_test::GetData,&accesser_test::SetData);
	acc.Set(&testobj,"100");
	ASSERT_STREQ(acc.Get(&testobj).c_str(),"100");
}
TEST(core, variant) {
	//Variant<int,bool,float,tng::string> variant;
	//variant.set<tng::string>("string variant");
	//ASSERT_EQ(variant.is<tng::string>(),true);
	

// 	Variant v(1000);
// 	s32 ret;
// 	v.Get(&ret);
// 	ASSERT_EQ(ret,1000);
// 	ASSERT_EQ(v.Set<s32>(1),true);
// 	ASSERT_EQ(v.Set<float>(1.0f),false);
// 	ASSERT_EQ(v.Get(&ret),true);
// 	ASSERT_EQ(ret,1);
// 	v.FromString("333");
// 	v.Get(&ret);
// 	ASSERT_EQ(ret,333);
// 	ASSERT_STREQ(v.ToString().c_str(),"333");
}