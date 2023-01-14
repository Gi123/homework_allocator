#include <map>
#include <iostream>

#include "allocator.h"
#include "customlist.h"

template <typename Map>
static void PrintMap(const Map& map)
{
    for (auto pair : map)
        std::cout << pair.first << " " << pair.second << std::endl;
}

static unsigned long factorial(int i)
{
  if (i==0) 
    return 1;
  else 
    return i*factorial(i-1);
}


static void MapStandartAllocator()
{
    std::map<int, unsigned long> standartMap;
    for (int i = 0; i < 10; ++i)
        standartMap.insert({i, factorial(i)});
    PrintMap(standartMap);
}

static void MapCustomAllocator()
{
   std::map<int, unsigned long, std::less<int>, ChunkedAllocator<unsigned long>> CustomMap;
    for (int i = 0; i < 10; ++i)
        CustomMap.insert({i, factorial(i)});
    PrintMap(CustomMap);
}

static void CustomListStandardAllocator()
{
    SinglyLinkedList<int> list;
    for (int i = 0; i < 10; ++i)
        list.Add(i);
    std::cout << list;
}

static void CustomListWithCustomAllocator()
{
    SinglyLinkedList<int, ChunkedAllocator<int>> list;
    for (int i = 0; i < 10; ++i)
        list.Add(i);
    std::cout << list;
}

int main(int, char const **)
{
    try
    {
        MapStandartAllocator();
        MapCustomAllocator();
        CustomListStandardAllocator();
        CustomListWithCustomAllocator();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
