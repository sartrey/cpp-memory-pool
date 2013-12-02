#include "test.h"

Tester::Tester()
{
	_MemPool = new MemoryPool();
}

Tester::~Tester()
{
	_Names.clear();
	_Funcs.clear();
	delete _MemPool;
}

void Tester::Run()
{
	LARGE_INTEGER freq,t1,t2;
	QueryPerformanceFrequency(&freq);

	for(int i=0; i != _Funcs.size(); i++)
	{
		QueryPerformanceCounter(&t1);
		(*((PFUNC)(_Funcs[i])))(_MemPool);
		QueryPerformanceCounter(&t2);
		cout<<"[Test: "<<_Names[i].c_str()<<"] ";
		cout<<(t2.QuadPart-t1.QuadPart) * 1000.0 / freq.QuadPart<<"(ms)"<<endl;
	}
}

void Tester::Add(string name, PFUNC func)
{
	_Names.push_back(name);
	_Funcs.push_back(func);
}

void Test_Optimize(MemoryPool* mp)
{
	mp->Optimize();
}

void Test_1_1(MemoryPool* mp)
{
	for(int i=0;i<10000;i++)
	{
		int* p1 = mp->Alloc<int>(3);
		double* p2 = mp->Alloc<double>(7);
		int* p3 = mp->Alloc<int>(15);
		double* p4 = mp->Alloc<double>(7);
		int* p5 = mp->Alloc<int>(290);
		double* p6 = mp->Alloc<double>(7);
		int* p7 = mp->Alloc<int>(3);
		double* p8 = mp->Alloc<double>(7);
		int* p9 = mp->Alloc<int>(3);
		double* p10 = mp->Alloc<double>(7);
		mp->Free(p1);
		mp->Free(p2);
		mp->Free(p3);
		mp->Free(p4);
		mp->Free(p5);
		mp->Free(p6);
		mp->Free(p7);
		mp->Free(p8);
		mp->Free(p9);
		mp->Free(p10);
	}
}

void Test_1_2(MemoryPool* mp)
{
	for(int i=0;i<10000;i++)
	{
		int* p1 = mp->Alloc<int>(3);
		double* p2 = mp->Alloc<double>(16);
		int* p3 = mp->Alloc<int>(5);
		double* p4 = mp->Alloc<double>(156);
		int* p5 = mp->Alloc<int>(10);
		mp->Free(p1);
		mp->Free(p2);
		mp->Free(p3);
		mp->Free(p4);
		mp->Free(p5);
	}
}

void Test_1_3(MemoryPool* mp)
{
	for(int i=0;i<10000;i++)
	{
		int* p1 = new int[3];
		double* p2 = new double[7];
		int* p3 = new int[15];
		double* p4 = new double[7];
		int* p5 = new int[290];
		double* p6 = new double[7];
		int* p7 = new int[3];
		double* p8 = new double[7];
		int* p9 = new int[3];
		double* p10 = new double[7];
		delete [] p10;
		delete [] p9;
		delete [] p8;
		delete [] p7;
		delete [] p6;
		delete [] p5;
		delete [] p4;
		delete [] p3;
		delete [] p2;
		delete [] p1;
	}
}

void Test_1_4(MemoryPool* mp)
{
	for(int i=0;i<10000;i++)
	{
		int* p1 = new int[3];
		double* p2 = new double[16];
		int* p3 = new int[5];
		double* p4 = new double[156];
		int* p5 = new int[10];
		delete [] p1;
		delete [] p2;
		delete [] p3;
		delete [] p4;
		delete [] p5;
	}
}

void Test_2_Expand(MemoryPool* mp)
{
	//预扩展内存本身将一定程度提高后续操作的效率
	//由于扩展内存会导致进程的堆发生目前未知的变异，导致系统内存分配效率显著下降
	//内存池本身效率的时变稳定特性得以体现
	bool ret = mp->Expand(GetMPBVol(8)); 
}

void Test_2_Sign(MemoryPool* mp)
{
	bool ret = mp->Sign(320);
}

void Test_2_Unsign(MemoryPool* mp)
{
	bool ret = mp->Unsign(320);
}

void Test_2_1(MemoryPool* mp)
{
	for(int i=0;i<10000;i++)
	{
		BYTE** pp = mp->Alloc<BYTE*>(80);
		for(int i=0;i<80;i++)
			pp[i] = mp->Alloc<BYTE>(320);
		for(int i=0;i<80;i++)
			mp->Free(pp[i]);
		mp->Free(pp);
	}
}

void Test_2_2(MemoryPool* mp)
{
	for(int i=0;i<10000;i++)
	{
		BYTE** pp = new BYTE*[80];
		for(int i=0;i<80;i++)
			pp[i] = new BYTE[320];
		for(int i=0;i<80;i++)
			delete [] pp[i];
		delete [] pp;
	}
}

void Test_3_1(MemoryPool* mp)
{
	A* obj = NULL;
	for(int i=0;i<10000;i++)
	{
		obj = mp->Alloc<A>(100, true);
		mp->Dispose(obj);
		mp->Free(obj);
	}
}

void Test_3_2(MemoryPool* mp)
{
	for(int i=0;i<10000;i++)
	{
		A* obj = new A[100];
		delete [] obj;
	}
}