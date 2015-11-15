#ifndef HEAP_H 
#define HEAP_H

#define LEFT(i) ((i+1) << 1) - 1
#define PARENT(i) ((i+1) >> 1) - 1
#define RIGHT(i) (i+1) << 1

#include <memory>

template<typename T>
class Heap
{
public:
    enum Type {MAX, MIN};

    Heap() {}
	Heap(T *arr, int n, Type _type = MAX)
		: m_heap(arr), m_heapsize(n), m_type(_type)
	{
		buildHeap();		
	}

    T top() const { return m_heap[0]; }

    void pop()
    {
        --m_heapsize;
        m_heap[0] = m_heap[m_heapsize];
        heapify(0);
    }

	void heapify(int i)
	{
		if(m_type == MAX)
			maxHeapify(i);
		else minHeapify(i);
	}

    void updateKey(int i, const T &_new)
    {
        auto cmp = m_type == MAX
                ? [](const T &parent, const T &child) -> bool { return parent < child; }
                : [](const T &parent, const T &child) -> bool { return parent > child; };
        m_heap[i] = _new;
        while(i != 0 && cmp(m_heap[PARENT(i)], m_heap[i]))
            std::swap(m_heap[i], m_heap[PARENT(i)]);
    }

    void insert(const T &_new)
    {
        m_heap[m_heapsize] = _new;
        updateKey(m_heapsize, _new);
        ++m_heapsize;
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
		
        int l = LEFT(i);
        int r = RIGHT(i);
        int largest = i;
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
        if(i >= m_heapsize/2) return;
		
        int l = LEFT(i);
        int r = RIGHT(i);
        int smallest = i;
		if(m_heap[smallest] > m_heap[l])
			smallest = l;
        if(r < m_heapsize && m_heap[smallest] > m_heap[r])
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
