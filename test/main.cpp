#include <bl/sort/bubble.h>
#include <bl/sort/cocktail.h>
#include <bl/sort/insertion.h>
#include <bl/sort/shell.h>
#include <bl/sort/quick.h>
#include <bl/sort/heap.h>

#include <bl/util/in_out.h>
#include <bl/util/random.h>
#include <bl/util/timer.h>

#include <algorithm>

// global configuration
static int g_seed = 13;
static int g_numIter = 10;
static int g_maxArraySize = 1e9;
static int g_testSize = 1e4;
static int* g_arrayInt = new int[g_maxArraySize];
static unsigned int* g_arrayUInt = new unsigned int[g_maxArraySize];
static float* g_arrayFloat = new float[g_maxArraySize];
static double* g_arrayDouble = new double[g_maxArraySize];

// helper
template<typename t_value, typename t_size>
bool checkOrdered(t_value* a, t_size size)
{
	for(t_size i = 1; i < size; ++i)
	{
		if(a[i] < a[i-1])
		{
			return false;
		}
	}
	return true;
}

// case 1: random
template<typename t_value, typename t_size, typename sort_t>
double testRandom(t_value* a, t_size size, sort_t sortFunc)
{
	auto rand = bl::make_random<t_value>(0, size, g_seed);
	for(t_size i = 0; i < size; ++i)
	{
		a[i] = rand();
	}
	bl::timer t;
	sortFunc(a, size);
	double e = t.milliseconds();
	return e;
}

// case 2: ordered
template<typename t_value, typename t_size, typename sort_t>
double testOrdered(t_value* a, t_size size, sort_t sortFunc)
{
	for(t_size i = 0; i < size; ++i)
	{
		a[i] = (t_value)i;
	}
	bl::timer t;
	sortFunc(a, size);
	double e = t.milliseconds();
	return e;
}

// case 3: reverse ordered
template<typename t_value, typename t_size, typename sort_t>
double testReverseOrdered(t_value* a, t_size size, sort_t sortFunc)
{
	for(t_size i = 0; i < size; ++i)
	{
		a[i] = (t_value)size-i;
	}
	bl::timer t;
	sortFunc(a, size);
	double e = t.milliseconds();
	return e;
}

// case 4: random nearby disorder
template<typename t_value, typename t_size, typename sort_t>
double testNearDisorder(t_value* a, t_size size, sort_t sortFunc)
{
	for(t_size i = 0; i < size; ++i)
	{
		a[i] = (t_value)i;
	}
	auto rand = bl::make_random<float>(g_seed);
	for(int k = 0; k < 5; ++k)
	{
		for(t_size i = 1; i < size; ++i)
		{
			if(rand() < 0.1f)
			{
				std::swap(a[i], a[i-1]);
			}
		}
	}
	bl::timer t;
	sortFunc(a, size);
	double e = t.milliseconds();
	return e;
}

// case 5: random far disorder
template<typename t_value, typename t_size, typename sort_t>
double testFarDisorder(t_value* a, t_size size, sort_t sortFunc)
{
	auto rand = bl::make_random<t_value>(0, size, g_seed);
	auto randf = bl::make_random<float>(g_seed);
	for(t_size i = 0; i < size; ++i)
	{
		if(randf() < 0.1f)
		{
			a[i] = rand();
		}
		else
		{
			a[i] = (t_value)i;
		}
	}
	bl::timer t;
	sortFunc(a, size);
	double e = t.milliseconds();
	return e;
}

// case 6: repeated values
template<typename t_value, typename t_size, typename sort_t>
double testRandomDuplicate(t_value* a, t_size size, sort_t sortFunc)
{
	auto rand = bl::make_random<t_value>(0, size/100, g_seed);
	for(t_size i = 0; i < size; ++i)
	{
		a[i] = rand();
	}
	bl::timer t;
	sortFunc(a, size);
	double e = t.milliseconds();
	return e;
}

// case 7: contiguous ordered and disordered
template<typename t_value, typename t_size, typename sort_t>
double testContiguous(t_value* a, t_size size, sort_t sortFunc)
{
	auto rand = bl::make_random<t_value>(0, size, g_seed);
	t_value v = 0;
	t_size k = 0;
	bool ordered = true;
	for(t_size i = 0; i < size; ++i)
	{
		if(ordered)
		{
			a[i] = v++;
		}
		else
		{
			a[i] = rand();
		}
		if(k++ > size/100)
		{
			ordered = !ordered;
			k = 0;
		}
	}
	bl::timer t;
	sortFunc(a, size);
	double e = t.milliseconds();
	return e;
}

template<typename t_value, typename t_size, typename test_t>
void runTest(const char* name, t_value* a, t_size size, test_t testCase)
{
	double total = 0.0;
	for(int i = 0; i < g_numIter; ++i)
	{
		double dt = testCase(a, size);
		if(!checkOrdered(a, size))
		{
			bl::print("not sorted!");
			exit(1);
		}
		//print("step:", i, "dt:", dt);
		total += dt;
	}
	double avg = total / g_numIter;
	std::cout << std::fixed;
	bl::print(name, "-", "average time (ms):", avg, "| million elem/s:", size / 1000 / avg);
}

int main()
{
	bl::print(); bl::print("----- random -", g_testSize, "elements -----");
	runTest("std", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandom(a1, s1, [](int* a2, int s2){std::sort(a2, a2 + s2);});});
	runTest("bubble", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandom(a1, s1, [](int* a2, int s2){bl::bubble_sort(a2, s2);});});
	runTest("cocktail", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandom(a1, s1, [](int* a2, int s2){bl::cocktail_sort(a2, s2);});});
	runTest("insertion", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandom(a1, s1, [](int* a2, int s2){bl::insertion_sort(a2, s2);});});
	runTest("insertion binary", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandom(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary(a2, s2);});});
	runTest("insertion binary move", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandom(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary_move(a2, s2);});});
	runTest("shell", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandom(a1, s1, [](int* a2, int s2){bl::shell_sort(a2, s2);});});
	runTest("quick", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandom(a1, s1, [](int* a2, int s2){bl::quick_sort(a2, 0, s2-1);});});
	runTest("heap", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandom(a1, s1, [](int* a2, int s2){bl::heap_sort(a2, s2);});});

	bl::print(); bl::print("----- ordered -", g_testSize, "elements -----");
	runTest("std", g_arrayInt, g_testSize, [](int* a1, int s1){return testOrdered(a1, s1, [](int* a2, int s2){std::sort(a2, a2 + s2);});});
	runTest("bubble", g_arrayInt, g_testSize, [](int* a1, int s1){return testOrdered(a1, s1, [](int* a2, int s2){bl::bubble_sort(a2, s2);});});
	runTest("cocktail", g_arrayInt, g_testSize, [](int* a1, int s1){return testOrdered(a1, s1, [](int* a2, int s2){bl::cocktail_sort(a2, s2);});});
	runTest("insertion", g_arrayInt, g_testSize, [](int* a1, int s1){return testOrdered(a1, s1, [](int* a2, int s2){bl::insertion_sort(a2, s2);});});
	runTest("insertion binary", g_arrayInt, g_testSize, [](int* a1, int s1){return testOrdered(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary(a2, s2);});});
	runTest("insertion binary move", g_arrayInt, g_testSize, [](int* a1, int s1){return testOrdered(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary_move(a2, s2);});});
	runTest("shell", g_arrayInt, g_testSize, [](int* a1, int s1){return testOrdered(a1, s1, [](int* a2, int s2){bl::shell_sort(a2, s2);});});
	runTest("quick", g_arrayInt, g_testSize, [](int* a1, int s1){return testOrdered(a1, s1, [](int* a2, int s2){bl::quick_sort(a2, 0, s2-1);});});
	runTest("heap", g_arrayInt, g_testSize, [](int* a1, int s1){return testOrdered(a1, s1, [](int* a2, int s2){bl::heap_sort(a2, s2);});});

	bl::print(); bl::print("----- reverse -", g_testSize, "elements -----");
	runTest("std", g_arrayInt, g_testSize, [](int* a1, int s1){return testReverseOrdered(a1, s1, [](int* a2, int s2){std::sort(a2, a2 + s2);});});
	runTest("bubble", g_arrayInt, g_testSize, [](int* a1, int s1){return testReverseOrdered(a1, s1, [](int* a2, int s2){bl::bubble_sort(a2, s2);});});
	runTest("cocktail", g_arrayInt, g_testSize, [](int* a1, int s1){return testReverseOrdered(a1, s1, [](int* a2, int s2){bl::cocktail_sort(a2, s2);});});
	runTest("insertion", g_arrayInt, g_testSize, [](int* a1, int s1){return testReverseOrdered(a1, s1, [](int* a2, int s2){bl::insertion_sort(a2, s2);});});
	runTest("insertion binary", g_arrayInt, g_testSize, [](int* a1, int s1){return testReverseOrdered(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary(a2, s2);});});
	runTest("insertion binary move", g_arrayInt, g_testSize, [](int* a1, int s1){return testReverseOrdered(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary_move(a2, s2);});});
	runTest("shell", g_arrayInt, g_testSize, [](int* a1, int s1){return testReverseOrdered(a1, s1, [](int* a2, int s2){bl::shell_sort(a2, s2);});});
	runTest("quick", g_arrayInt, g_testSize, [](int* a1, int s1){return testReverseOrdered(a1, s1, [](int* a2, int s2){bl::quick_sort(a2, 0, s2-1);});});
	runTest("heap", g_arrayInt, g_testSize, [](int* a1, int s1){return testReverseOrdered(a1, s1, [](int* a2, int s2){bl::heap_sort(a2, s2);});});

	bl::print(); bl::print("----- near disorder -", g_testSize, "elements -----");
	runTest("std", g_arrayInt, g_testSize, [](int* a1, int s1){return testNearDisorder(a1, s1, [](int* a2, int s2){std::sort(a2, a2 + s2);});});
	runTest("bubble", g_arrayInt, g_testSize, [](int* a1, int s1){return testNearDisorder(a1, s1, [](int* a2, int s2){bl::bubble_sort(a2, s2);});});
	runTest("cocktail", g_arrayInt, g_testSize, [](int* a1, int s1){return testNearDisorder(a1, s1, [](int* a2, int s2){bl::cocktail_sort(a2, s2);});});
	runTest("insertion", g_arrayInt, g_testSize, [](int* a1, int s1){return testNearDisorder(a1, s1, [](int* a2, int s2){bl::insertion_sort(a2, s2);});});
	runTest("insertion binary", g_arrayInt, g_testSize, [](int* a1, int s1){return testNearDisorder(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary(a2, s2);});});
	runTest("insertion binary move", g_arrayInt, g_testSize, [](int* a1, int s1){return testNearDisorder(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary_move(a2, s2);});});
	runTest("shell", g_arrayInt, g_testSize, [](int* a1, int s1){return testNearDisorder(a1, s1, [](int* a2, int s2){bl::shell_sort(a2, s2);});});
	runTest("quick", g_arrayInt, g_testSize, [](int* a1, int s1){return testNearDisorder(a1, s1, [](int* a2, int s2){bl::quick_sort(a2, 0, s2-1);});});
	runTest("heap", g_arrayInt, g_testSize, [](int* a1, int s1){return testNearDisorder(a1, s1, [](int* a2, int s2){bl::heap_sort(a2, s2);});});

	bl::print(); bl::print("----- far disorder -", g_testSize, "elements -----");
	runTest("std", g_arrayInt, g_testSize, [](int* a1, int s1){return testFarDisorder(a1, s1, [](int* a2, int s2){std::sort(a2, a2 + s2);});});
	runTest("bubble", g_arrayInt, g_testSize, [](int* a1, int s1){return testFarDisorder(a1, s1, [](int* a2, int s2){bl::bubble_sort(a2, s2);});});
	runTest("cocktail", g_arrayInt, g_testSize, [](int* a1, int s1){return testFarDisorder(a1, s1, [](int* a2, int s2){bl::cocktail_sort(a2, s2);});});
	runTest("insertion", g_arrayInt, g_testSize, [](int* a1, int s1){return testFarDisorder(a1, s1, [](int* a2, int s2){bl::insertion_sort(a2, s2);});});
	runTest("insertion binary", g_arrayInt, g_testSize, [](int* a1, int s1){return testFarDisorder(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary(a2, s2);});});
	runTest("insertion binary move", g_arrayInt, g_testSize, [](int* a1, int s1){return testFarDisorder(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary_move(a2, s2);});});
	runTest("shell", g_arrayInt, g_testSize, [](int* a1, int s1){return testFarDisorder(a1, s1, [](int* a2, int s2){bl::shell_sort(a2, s2);});});
	runTest("quick", g_arrayInt, g_testSize, [](int* a1, int s1){return testFarDisorder(a1, s1, [](int* a2, int s2){bl::quick_sort(a2, 0, s2-1);});});
	runTest("heap", g_arrayInt, g_testSize, [](int* a1, int s1){return testFarDisorder(a1, s1, [](int* a2, int s2){bl::heap_sort(a2, s2);});});

	bl::print(); bl::print("----- random duplicate -", g_testSize, "elements -----");
	runTest("std", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandomDuplicate(a1, s1, [](int* a2, int s2){std::sort(a2, a2 + s2);});});
	runTest("bubble", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandomDuplicate(a1, s1, [](int* a2, int s2){bl::bubble_sort(a2, s2);});});
	runTest("cocktail", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandomDuplicate(a1, s1, [](int* a2, int s2){bl::cocktail_sort(a2, s2);});});
	runTest("insertion", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandomDuplicate(a1, s1, [](int* a2, int s2){bl::insertion_sort(a2, s2);});});
	runTest("insertion binary", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandomDuplicate(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary(a2, s2);});});
	runTest("insertion binary move", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandomDuplicate(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary_move(a2, s2);});});
	runTest("shell", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandomDuplicate(a1, s1, [](int* a2, int s2){bl::shell_sort(a2, s2);});});
	runTest("quick", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandomDuplicate(a1, s1, [](int* a2, int s2){bl::quick_sort(a2, 0, s2-1);});});
	runTest("heap", g_arrayInt, g_testSize, [](int* a1, int s1){return testRandomDuplicate(a1, s1, [](int* a2, int s2){bl::heap_sort(a2, s2);});});

	bl::print(); bl::print("----- contiguous -", g_testSize, "elements -----");
	runTest("std", g_arrayInt, g_testSize, [](int* a1, int s1){return testContiguous(a1, s1, [](int* a2, int s2){std::sort(a2, a2 + s2);});});
	runTest("bubble", g_arrayInt, g_testSize, [](int* a1, int s1){return testContiguous(a1, s1, [](int* a2, int s2){bl::bubble_sort(a2, s2);});});
	runTest("cocktail", g_arrayInt, g_testSize, [](int* a1, int s1){return testContiguous(a1, s1, [](int* a2, int s2){bl::cocktail_sort(a2, s2);});});
	runTest("insertion", g_arrayInt, g_testSize, [](int* a1, int s1){return testContiguous(a1, s1, [](int* a2, int s2){bl::insertion_sort(a2, s2);});});
	runTest("insertion binary", g_arrayInt, g_testSize, [](int* a1, int s1){return testContiguous(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary(a2, s2);});});
	runTest("insertion binary move", g_arrayInt, g_testSize, [](int* a1, int s1){return testContiguous(a1, s1, [](int* a2, int s2){bl::insertion_sort_binary_move(a2, s2);});});
	runTest("shell", g_arrayInt, g_testSize, [](int* a1, int s1){return testContiguous(a1, s1, [](int* a2, int s2){bl::shell_sort(a2, s2);});});
	runTest("quick", g_arrayInt, g_testSize, [](int* a1, int s1){return testContiguous(a1, s1, [](int* a2, int s2){bl::quick_sort(a2, 0, s2-1);});});
	runTest("heap", g_arrayInt, g_testSize, [](int* a1, int s1){return testContiguous(a1, s1, [](int* a2, int s2){bl::heap_sort(a2, s2);});});

	return 0;
}
