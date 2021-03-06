## AMOT技术 ##
版本：2014080614  
修订：0  
语言：简体中文 zh-cn

适用版本：1.0.5.2

内存操作是重要的控制任务，影响整个应用程序性能。  
大多数应用程序会执行堆上内存操作，与栈内存相比开销更大。

C/C++的malloc/free和new/delete会调用系统API操作堆内存。  
出于通用性的考虑，还会在操作期间加锁。  
调用系统API和锁操作会显著降低性能。  
如果频繁操作小对象，会导致堆内存碎片化，降低寻址效率。

由于语言本身的设计，C/C++没有提供原生的内存重分配操作。  
对于需要动态调整大小的堆内存操作，必须使用分配、复制、清理的操作。  
这样效率较低而且中间过程会额外消耗内存资源。  

内存池是一种内存优化策略，主要通过预分配和模拟操作提高性能。  
内存池仅在初始化时执行一次系统API调用和一次锁操作申请堆内存。  
然后内存池会长期保持对整块内存的控制权。  
内存池使用标记、无锁等优化(简化)策略降低内存管理的性能开销。  
但是由于这些策略会影响通用性，所以内存池通常需要为应用程序量身定制。

AMOT也是内存池的一种实现。  
通过内存池进行用户内存操作，实际的内存操作交由实现了不同模拟算法的内存块。  
内存块是实际拥有内存控制权的对象。  

AMOT的内存池支持动态挂载内存块。  
用户可以自己设计实现内存操作策略，并挂载在AMOT内存池中。

内存块的大小由其级别限定，范围是[1,11].  
但是内存块级别只是容量的最大约束，不是实际容量。    
每个级别的容量是低级别的4倍。  
容量范围从1级1KB到11级1GB。  

级别与容量对照表：  
1-1KB 2-4KB 3-16KB 4-64KB 5-256KB
6-1MB 7-4MB 8-16MB 9-64MB 10-256MB 11-1GB  

AMOT认为分配单个对象超过1GB或数组超过2e30个对象都是不合理的，  
需要通过优化用户算法避免这类情况。

AMOT设计了3个内存元操作：  
* 分配(Allocate)，用于分配堆内存  
* 释放(Free)，用于释放由Allocate分配的堆内存  
* 重设(Resize)，用于重分配由Allocate分配的堆内存  

为了实现对象的构造和析构，AMOT的内存池提供2个补充元操作：  
* 创建(New)，用于自动类型识别的堆内存分配与构造函数调用  
* 销毁(Dispose)，用于析构函数调用与释放  

如果希望自动调用构造和析构，需要用户对象继承AMOT的IObject接口。  
由于没有严格遵循内存对齐，所以创建和销毁可能产生意想不到的问题，有待解决。  

目前AMOT内置实现了GP和FL两种内存块。

#### GP内存块 ####
GP内存块是通用目的(General Purpose)内存块。  
支持任意大小的内存分配、释放与重设操作。  

核心算法是使用标记链表模拟内存操作。  
分配操作对应创建新标记或零长标记重写。  
释放操作对应标记归零。  
重设操作对应标记重写或创建新标记。

GP内存块的操作算法的原则是：重用一切能重用标记。  

#### FL内存块 ####
FL内存块是定长(Fixed Length)内存块。  
支持特定大小的内存分配、释放操作。  

核心算法是位标记模拟内存操作。  
分配和释放都对应位操作。  

由于使用位操作，FL内存块的性能非常好。