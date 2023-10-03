#ifndef tdevH
#define tdevH

// Enable entire header or disable
#define TDEV_ENABLED		1
#if TDEV_ENABLED

// Enable different parts of the header.
#define TDEV_DEBUG		1
#define TDEV_MEMORY		0
#define TDEV_BENCHMARK	1

// ====================
// Stuff needed for the exposed functions
// ====================
#include <iostream>
#include <string>

#if TDEV_MEMORY
namespace tdev
{
	struct MemoryUsage
	{
		uint64_t alloc = 0;
		uint64_t freed = 0;
		inline uint64_t get_total() const		{ return alloc - freed; }
		inline std::string to_string() const
		{
			return std::string("ALLOC: ") + std::to_string(alloc) \
				+ "\nFREED: " + std::to_string(freed) \
				+ "\nTOTAL: " + std::to_string(get_total());
		}
	};
	static MemoryUsage memory_usage_g;
}
void* operator new(size_t n)
{
	tdev::memory_usage_g.alloc += n;
	return malloc(n);
}
void operator delete(void* ptr)
{
	tdev::memory_usage_g.freed += sizeof(ptr);
	free(ptr);
}
#endif

#if TDEV_BENCHMARK
#include <chrono>
#include <map>
namespace tdev
{
	enum BenchTimeType
	{
		BENCH_NANO_S_TYPE,
		BENCH_MICRO_S_TYPE,
		BENCH_MILLI_S_TYPE,
		BENCH_S_TYPE,
		BENCH_MIN_TYPE
	};
	struct Benchmark
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> current_start;
		double elapsed_time;
		uint64_t elapsed_counter;
	};
	static std::map<std::string, Benchmark> benchmarks;
	inline std::chrono::duration<double> delta_time(
		const std::chrono::time_point<std::chrono::high_resolution_clock>& start,
		const std::chrono::time_point<std::chrono::high_resolution_clock>& end)
	{
		return end - start;
	}
	inline void _reg_bench(const std::string& name)
	{
		auto it = benchmarks.find(name);
		if (it != benchmarks.end()) {
			std::cout << "(BENCH) \'" << name << "\': Benchmark already registered!" << std::endl;
		} else {
			benchmarks[name] = {std::chrono::high_resolution_clock::now(), 0, 0};
		}
	}
	inline void _set_bench(const std::string& name)
	{
		auto it = benchmarks.find(name);
		if (it != benchmarks.end()) {
			benchmarks[name].current_start	= std::chrono::high_resolution_clock::now();
			benchmarks[name].elapsed_time		= 0;
			benchmarks[name].elapsed_counter	= 0;
		} else {
			benchmarks[name] = {std::chrono::high_resolution_clock::now(), 0, 0};
		}
	}
	inline void _add_bench(const std::string& name)
	{
		auto it = benchmarks.find(name);
		if (it != benchmarks.end()) {
			auto end									= std::chrono::high_resolution_clock::now();
			benchmarks[name].elapsed_time		+= delta_time(benchmarks[name].current_start, end).count();
			benchmarks[name].elapsed_counter	+= 1;
			benchmarks[name].current_start	= end;
		} else {
			std::cout << "Couldn't find benchmark! Registered with different name?" << std::endl;
		}
	}
	inline void _log_bench(const std::string& name)
	{
		auto it = benchmarks.find(name);
		std::cout << "(BENCH) \'" << name << "\': ";
		if (it != benchmarks.end()) {
			std::cout << delta_time(it->second.current_start, std::chrono::high_resolution_clock::now()).count() << "s" << std::endl;
		} else {
			std::cout << "Couldn't find benchmark! Registered with different name?" << std::endl;
		}
	}
	inline void _log_bench_avg(const std::string& name)
	{
		auto it = benchmarks.find(name);
		std::cout << "(BENCH) \'" << name << "\': ";
		if (it != benchmarks.end()) {
			std::cout << "Average time " << it->second.elapsed_time/(double)it->second.elapsed_counter << "s, with n = " << it->second.elapsed_counter << std::endl;
		} else {
			std::cout << "Couldn't find benchmark! Registered with different name?" << std::endl;
		}
	}
	inline void _log_bench_elapsed(const std::string& name)
	{
		auto it = benchmarks.find(name);
		std::cout << "(BENCH) \'" << name << "\': ";
		if (it != benchmarks.end()) {
			std::cout << "Total elapsed time " << it->second.elapsed_time << "s" << std::endl;
		} else {
			std::cout << "Couldn't find benchmark! Registered with different name?" << std::endl;
		}
	}
	inline double _get_bench(const std::string& name)
	{
		auto it = benchmarks.find(name);
		std::cout << "(BENCH) \'" << name << "\': ";
		if (it != benchmarks.end()) {
			return delta_time(it->second.current_start, std::chrono::high_resolution_clock::now()).count();
		}
		return 0;
	}
	inline double _get_bench_avg(const std::string& name)
	{
		auto it = benchmarks.find(name);
		std::cout << "(BENCH) \'" << name << "\': ";
		if (it != benchmarks.end()) {
			return it->second.elapsed_time/(double)it->second.elapsed_counter;
		}
		return 0;
	}
	inline double _get_bench_elapsed(const std::string& name)
	{
		auto it = benchmarks.find(name);
		std::cout << "(BENCH) \'" << name << "\': ";
		if (it != benchmarks.end()) {
			return it->second.elapsed_time;
		}
		return 0;
	}
}
#endif

// ====================
// EXPOSED FUNCTIONS
// ====================
// Used to display debug information.
namespace tdev
{
#if TDEV_DEBUG
	inline void DELOG(const std::string& msg)	{ std::cout << "(DELOG) " << msg << std::endl; }
	inline void PAUSE()									{ std::cin.get(); }
	inline void LINE()                        	{ std::cout << "==================================================" << std::endl; }
	inline void ENDL()                        	{ std::cout << "\n" << std::endl; }
	inline void BLOCK()                       	{ ENDL();LINE();LINE();ENDL(); }
#else
	inline void DELOG(const std::string& msg)	{}
	inline void PAUSE()									{}
	inline void LINE()                        	{}
	inline void ENDL()                        	{}
	inline void BLOCK()                       	{}
#endif

// Used to display information about global memory usage.
#if TDEV_MEMORY
	inline void LOG_MEM_G()	{ std::cout << memory_usage_g.to_string() << std::endl; }
#else
	inline void LOG_MEM_G()	{}
#endif

// Used to benchmark. Usage example:
/**
 * Let's assume the following code block:
 * ```
 * void my_function()
 * {
 *		for (int i=0; i<1000; ++i) {
 *			dostuff();
 *		}
 * }
 * ```
 * We have the following approach:
 * 1. Choose a name, e.g. `name = "my_function/forloop"`,
 * 2. register or set benchmark: `REG_BENCH(name)` or `SET_BENCH(name)`,
 * 3. Benchmark:
 * ```
 * void my_function()
 * {
 *		tdev::REG_BENCH("my_function/forloop");
 *		for (int i=0; i<1000; ++i) {
 *			dostuff();
 *			tdev::LOG_BENCH("my_function/forloop");		// log each iteration
 *			tdev::ADD_BENCH("my_function/forloop"); 		// add one iteration to sum, also resets it (but not the sum)
 *		}
 *		tdev::LOG_BENCH_ELAPSED("my_function/forloop");	// log the total time it took for all recorded iterations
 *		tdev::LOG_BENCH_AVG("my_function/forloop");		// log the average time each iteration took
 *		tdev::DEL_BENCH("my_function/forloop");			// clean up
 * }
 */
#if TDEV_BENCHMARK
	inline void REG_BENCH(const std::string& name)					{ _reg_bench(name); }						/* ensures creation of a new, independent from existing benchmark */
	inline void DEL_BENCH(const std::string& name)					{ benchmarks.erase(name); }				/* clean up after using a benchmark */
	inline void SET_BENCH(const std::string& name)					{ _set_bench(name); }						/* registers/sets a new benchmark or "resets" existing benchmark */
	inline void ADD_BENCH(const std::string& name)					{ _add_bench(name); } 						/* adds elapsed time for benchmark to total and resets benchmark start clock */
	inline void LOG_BENCH(const std::string& name)					{ _log_bench(name); }						/* logs the current time from start/reset of benchmark */
	inline void LOG_BENCH_AVG(const std::string& name)			{ _log_bench_avg(name); }					/* the average of all the add benchs */
	inline void LOG_BENCH_ELAPSED(const std::string& name)		{ _log_bench_elapsed(name); }				/* elapsed time total of all the add benchs */
	inline double GET_BENCH(const std::string& name)				{ return _get_bench(name); }
	inline double GET_BENCH_AVG(const std::string& name)			{ return _get_bench_avg(name); }
	inline double GET_BENCH_ELAPSED(const std::string& name)	{ return _get_bench_elapsed(name); }
#else
	inline void REG_BENCH(const std::string& name)					{}
	inline void DEL_BENCH(const std::string& name)					{}
	inline void SET_BENCH(const std::string& name)					{}
	inline void ADD_BENCH(const std::string& name)					{}
	inline void LOG_BENCH(const std::string& name)					{}
	inline void LOG_BENCH_AVG(const std::string& name)			{}
	inline void LOG_BENCH_ELAPSED(const std::string& name)		{}
	inline double GET_BENCH(const std::string& name)				{ return 0; }
	inline double GET_BENCH_AVG(const std::string& name)			{ return 0; }
	inline double GET_BENCH_ELAPSED(const std::string& name)	{ return 0; }
#endif
}

#endif /* TDEV_ENABLED */
#endif /* tdevH */

