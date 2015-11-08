#ifndef HEAP_H 
#define HEAP_H

#define LEFT(i) ((i+1) << 1) - 1
#define PARENT(i) ((i+1) >> 1) - 1
#define RIGHT(i) (i+1) << 1

#include <memory>

template<typename T>
class Heap
{
	enum Type {MAX, MIN};
public:
	Heap() {}
	Heap(T *arr, int n, Type _type = MAX)
		: m_heap(arr), m_heapsize(n), m_type(_type)
	{
		buildHeap();		
	}

	void heapify(int i)
	{
		if(m_type == MAX)
			maxHeapify(i);
		else minHeapify(i);
	}

	void sort()
	{
		while(m_heapsize > 1)
			heapSortStep();
	}
	int size() const { return m_heapsize; }

private:
	void heapSortStep()
	{
		std::swap(m_heap[0], m_heap[m_heapsize-1]);
		--m_heapsize;
		heapify(0);
	}
	
	void maxHeapify(int i)
	{
		if(i >= m_heapsize/2) return;
		
		T l = LEFT(i);
		T r = RIGHT(i);
		T largest = i;
		if(m_heap[largest] < m_heap[l])
			largest = l;
		if(r < m_heapsize && m_heap[largest] < m_heap[r])
			largest = r;
		if(largest != i)
		{
			std::swap(m_heap[largest], m_heap[i]);
			maxHeapify(largest);
		}
	}
	
	void minHeapify(int i)
	{
		if(i < 0 || i >= m_heapsize) return;
		
		T l = LEFT(i);
		T r = RIGHT(i);
		T smallest = i;
		if(m_heap[smallest] > m_heap[l])
			smallest = l;
		if(m_heap[smallest] > m_heap[r])
			smallest = r;
		if(smallest != i)
		{
			std::swap(m_heap[smallest], m_heap[i]);
			minHeapify(smallest);
		}
	}

	void buildHeap()
	{
		for(int i = m_heapsize / 2; i >= 0; --i)
			heapify(i);
	}

	int m_heapsize;
	T *m_heap;
	Type m_type;
};

#endif
