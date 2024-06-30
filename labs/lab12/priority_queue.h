#ifndef priority_queue_h_
#define priority_queue_h_

#include <iostream>
#include <vector>
#include <cassert>


template <class T>
class priority_queue {
private:
  std::vector<T> m_heap;

public:
  priority_queue() {}

  priority_queue( std::vector<T> const& values )
  {
    for (unsigned int i = 0; i < values.size(); ++i) {
      push(values[i]);
    }
  }

  const T& top() const 
  {
    assert( !m_heap.empty() );
    return m_heap[0]; 
  }

  void push( const T& entry )
  {
    m_heap.push_back(entry);
    if (m_heap.size() > 1) {
      int current_index = m_heap.size()-1;
      //while there is a parent and the parent is greater than entry, swap them
      while ((current_index-1)/2 >= 0 && m_heap[(current_index-1)/2] > entry) {
        T temp = m_heap[(current_index-1)/2];
        m_heap[(current_index-1)/2] = m_heap[current_index];
        m_heap[current_index] = temp;
        current_index = (current_index-1)/2;
      }
    }
  }

  void pop() 
  {
    assert( !m_heap.empty() );
    m_heap[0] = m_heap[m_heap.size()-1];
    m_heap.pop_back();
    percolate_down(0);
  }

  void percolate_down(int index) {
    while (index*2+1 < m_heap.size()) {
      int child_index;
      //determine which child is smaller
      if (index*2+2 < m_heap.size() && m_heap[index*2+2] < m_heap[index*2+1]) {
        child_index = index*2+2;
      } else {
        child_index = index*2+1;
      }
      //swap the values if the child is smaller than the parent
      if (m_heap[child_index] < m_heap[index]) {
        T temp = m_heap[child_index];
        m_heap[child_index] = m_heap[index];
        m_heap[index] = temp;
        index = child_index;
      } else {
        break;
      }
    }
  }

  int size() { return m_heap.size(); }
  bool empty() { return m_heap.empty(); }


  //  The following three functions are used for debugging.

  //  Check to see that internally the heap property is realized.
  bool check_heap( )
  {
    return this->check_heap( this->m_heap );
  }

  //  Check an external vector to see that the heap property is realized.
  bool check_heap( const std::vector<T>& heap )
  {
    if (heap.size() <= 1) {
      return true;
    }

    for (unsigned int i = 0; i < heap.size()-1; ++i) {
      //check left child
      if (2*i+1 < heap.size() && heap[i] > heap[2*i+1]) {
        return false;
      }
      //check right child
      if (2*i+2 < heap.size() && heap[i] > heap[2*i+2]) {
        return false;
      }
    }
    return true;
  }

  //  A utility to print the contents of the heap.  Use it for debugging.
  void print_heap( std::ostream & ostr )
  {
    for ( unsigned int i=0; i<m_heap.size(); ++i )
      ostr << i << ": " << m_heap[i] << std::endl;
  }
  
};


template <class T>
void heap_sort( std::vector<T> & v )
{
  if (v.size() <= 1) {
    return;
  }
  //turn the vector into a heap by percolating down each value
  for (int i = (v.size()-1)/2; i >= 0; --i) {
    int index = i;
    while (index*2+1 < v.size()) {
      int child_index;
      //determine which child is smaller
      if (index*2+2 < v.size() && v[index*2+2] < v[index*2+1]) {
        child_index = index*2+2;
      } else {
        child_index = index*2+1;
      }
      //swap the values if the child is smaller than the parent
      if (v[child_index] < v[index]) {
        T temp = v[child_index];
        v[child_index] = v[index];
        v[index] = temp;
        index = child_index;
      } else {
        break;
      }
    }
  }

  for (int i = 0; i < v.size(); i++) {
    T temp_val = v[0];
    v[0] = v[v.size()-1-i];
    v.erase(v.end()-1-i);
    v.push_back(temp_val);
    
    int index = 0;
    while (index*2+1 < v.size()-i-1) {
      int child_index;
      //determine which child is smaller
      if (index*2+2 < v.size()-i-1 && v[index*2+2] < v[index*2+1]) {
        child_index = index*2+2;
      } else {
        child_index = index*2+1;
      }
      //swap the values if the child is smaller than the parent
      if (v[child_index] < v[index]) {
        T temp = v[child_index];
        v[child_index] = v[index];
        v[index] = temp;
        index = child_index;
      } else {
        break;
      }
    }
  }
}

#endif
