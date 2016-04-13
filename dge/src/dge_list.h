#ifndef DGE_UNORDERED_LIST_H
#define DGE_UNORDERED_LIST_H

#include <string.h>
#include <stdio.h>

namespace dge
{
    template<typename T>
    class UnorderedList
    {
    public:
        UnorderedList()
            : m_capacity(8), m_count(0), m_data(0)
        {
            // Initialise all the new objects with empty parameters (should be supported)
            m_data = new T[m_capacity]();
        }

        UnorderedList(int initialCapacity)
            : m_capacity(initialCapacity), m_count(0), m_data(0)
        {
            // Initialise all the new objects with empty parameters (should be supported)
            m_data = new T[m_capacity]();
        }

        ~UnorderedList()
        {
            if(m_data != nullptr)
            {
                // Automatically calls destructor on each object
                delete[] m_data;
            }
        }

        void insert(T val)
        {
            if(m_count >= m_capacity)
            {
                int newCapacity = m_capacity*2;
                T* newData = new T[newCapacity]();
                // Can't use memcpy as some classes keep pointers that
                // may be desroyed when the classes is destroyed as part
                // of getting rid of m_data
                for (int i = 0;i<m_count;++i)
                  newData[i] = m_data[i];
                // Clean up all the old data
                delete[] m_data;
                m_data = newData;
                m_capacity = newCapacity;
            }

            m_data[m_count] = val;
            ++m_count;
        }

        void removeAt(int index)
        {
            // TODO: Debug flags here so we don't always do this check?
            if((index < 0) || (index >= m_count))
            {
                printf("Attempt to remove invalid index %i, valid range is [0-%i]\n", index, m_count-1);
            }
            if(m_count > 1)
            {
                m_data[index] = m_data[m_count-1];
            }
            --m_count;
        }

        // TODO: Regarding calling of destructors, what happens if I have obj A and obj B and I set B = A,
        //       does it call B's destructor before overwriting constructing the new value?
        void clear()
        {
            m_count = 0;
        }

        // NOTE: This does the same thing as clear but it runs delete on each element first
        //       As a result this should only be called when the contents of the list is type*
        //
        //       Also note that this runs "delete item", not "delete[] item", so we get undefined
        //       behaviour if your pointer stores an array
        // TODO: Is this a reasonable thing to do even?
        void pointerClear()
        {
            for(int i=0; i<m_count; ++i)
            {
                delete m_data[i];
            }
            m_count = 0;
        }

        T operator [](int index)
        {
            // TODO: Debug flags here so we don't always do this check?
            if((index < 0) || (index >= m_count))
            {
                printf("Invalid index %i requested, valid range is [0-%i]\n", index, m_count-1);
            }
            return m_data[index];
        }

        int size()
        {
            return m_count;
        }

    private:
        int m_capacity;
        int m_count;
        T* m_data;
    };
}

#endif
