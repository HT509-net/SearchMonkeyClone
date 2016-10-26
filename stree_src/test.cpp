#include <iostream>
#include <map>
#include "sti/smap.h"
#include "sti/sset.h"
#include <boost/random.hpp>

#include <map>
#include <vector>

#ifdef __GNUC__
#include <time.h>
typedef uint64_t      U64;

inline U64 highres_clock()
{
    clock_t t = ::clock();
    return (U64)t;
}

#elif defined(_MSC_VER)
#include <windows.h>
typedef unsigned __int64 U64;

inline U64 highres_clock()
{
    LARGE_INTEGER t;
    if (!::QueryPerformanceCounter(&t))
        return 0;
    return (U64)t.QuadPart;
}

#else
#   error not implemented
#endif

using namespace sti;

namespace Benchmark
{

    template<class IT, class RAND_GEN>
    inline void shuffle_array(IT begin, IT end, RAND_GEN& rndgen, int limit = INT_MAX)
    {
        int count = 0;
        int size = (int)(end - begin);
        for (IT it = begin; it != end && count < limit; ++it, ++count)
        {
            int r = count + rndgen() % (size-count);
            IT rit = begin + r;
            std::swap(*it, *rit);
        }
    }

    template<class INTMAP>
        int do_shuffle_benchmark()
    {
         std::vector<int> vec;
         boost::mt19937 rng;
         const static int N = 4000000;
         boost::uniform_int<unsigned long> rng_dist(1, N);
         boost::variate_generator<boost::mt19937, boost::uniform_int<unsigned long> > myrand(rng, rng_dist);

         for (int i = 0; i < N/2; ++i)
             vec.push_back(i);

         shuffle_array(vec.begin(), vec.end(), myrand);

         INTMAP sm;  
         for (int i = 0; i < N/2; ++i)
             sm.insert(std::make_pair(i, i));

         std::vector<int> deleted;

         deleted.reserve(N);
         for (int i = (N/2) + 1; i < N/2; ++i)
             deleted.push_back(i);

         U64 mark = highres_clock();

         int sum = 0;  // We don't want the compiler to be too smart and optimize everything away...
         for (int i = 0; i < 100; ++i)
         {
            shuffle_array(vec.rbegin(), vec.rend(), myrand, 1000);
            for (int j = 0; j < 1000; ++j)
            {
                int k = vec.back();
                vec.pop_back();
                sm.erase(k);
                deleted.push_back(k);
            }
            shuffle_array(vec.begin(), vec.end(), myrand, 1000);
            for (int j = 0; j < 1000; ++j)
                sum += sm[vec[j]];

            shuffle_array(deleted.rbegin(), deleted.rend(), myrand, 1000);
            for (int j = 0; j < 1000; ++j)
            {
                int k = deleted.back();
                deleted.pop_back();
                sm.insert(std::make_pair(k, k));
                vec.push_back(k);
            }
        }
        U64 done = highres_clock();
        std::cout << " time: " << (done - mark) << std::endl;
        return sum;
    }
}

void benchmark()
{
    std::cout << "STL map";
    Benchmark::do_shuffle_benchmark<std::map<int, int> >();
    std::cout << "smap";
    Benchmark::do_shuffle_benchmark<smap<int, int> >();
}


int main()
{
    // Create a map
    typedef smap<char, int> Map;
    Map my_map;

    // Insert some values  
    my_map.insert(std::make_pair('a', 1));
    my_map.insert(std::make_pair('A', 1));
    my_map.insert(std::make_pair('b', 2));
    my_map.insert(std::make_pair('B', 2));

    // find an item
    std::cout << "Find 'a':'" << std::endl;
    Map::const_iterator it = my_map.find('a');
    std::cout << "  my_map[" << it->first << "]= " << it->second << std::endl;

    // Use operator []:
    my_map['a'] = 10;

    // Iterate over map:
    std::cout << "Iterate:'" << std::endl;
    for (it = my_map.begin(); it != my_map.end(); ++it)
    {
        std::cout << "  my_map[" << it->first << "]= " << it->second << std::endl; 
    }

    // Erase an item
    my_map.erase('a');

    // or we can use an iterator:
    it = my_map.find('b');
    my_map.erase(it);

    // Find out how many items are left in map:
    std::cout << "Items: " << my_map.size() << std::endl;

    typedef sset<int> Set;
    Set my_set;
    my_set.insert(1);
    my_set.insert(10);
    if (my_set.find(1) != my_set.end())
        std::cout << "Found 1 in set" << std::endl;
    else
        std::cout << "Couldn't find 1 in set" << std::endl;

    std::cout << std::endl << "Benchmark:" << std::endl;
    benchmark();

    return 0;
}
