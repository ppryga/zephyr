/*
 * Copyright (c) 2023, Piotr Pryga
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <exception>
#include <stdexcept>

#if defined(CONFIG_USE_STD_IOSTREAM)
#include <iostream>
#else
#include <cstdio>
#endif /* CONFIG_USE_STD_IOSTREAM */

#if defined(CONFIG_USE_STD_STRING)
#include <string>
#endif /* CONFIG_USE_STD_STRING */

#include <cstring>
#include <zephyr/kernel.h>

#if defined __GNUC__
#include "unwind-cxx.h"

namespace __gnu_cxx
{
  /** @brief An overload of for the GNU implementation of terminate handler
   *         that does quite a lot of printing of information about exception
   *         being thrown.
   */
  void __verbose_terminate_handler()
  {
	/* Arbitrary decision to terminate. An application specific terminate should
	 * be implemented here to save memory. The GNU implementation is assigned to
	 * std::terminate_handler __cxxabiv1::__terminate_handler global variable hence
	 * is available in memory after liniking.
	 */
	std::terminate();
  }
};

K_HEAP_DEFINE(exception_heap, 1024);

namespace __cxxabiv1 {
	extern "C" void *__cxa_allocate_exception(std::size_t thrown_size) noexcept
	{
		thrown_size += sizeof(__cxxabiv1::__cxa_refcounted_exception);

		void *ret = k_heap_alloc(&exception_heap, thrown_size, K_NO_WAIT);

		if (!ret)
			std::terminate();

		memset(ret, 0, sizeof(__cxxabiv1::__cxa_refcounted_exception));

		return (void *)((char *)ret + sizeof(__cxxabiv1::__cxa_refcounted_exception));
	}


	extern "C" void __cxa_free_exception(void *vptr) noexcept
	{
		char *ptr = (char *)vptr - sizeof(__cxxabiv1::__cxa_refcounted_exception);
	
		k_heap_free(&exception_heap, ptr);
	}
}
#endif /* __GNUC__ */

class hello_world
{
private:
#if defined(CONFIG_USE_STD_STRING)
	const std::string hello_str;
#else
	const char * hello_str;
#endif /* CONFIG_USE_STD_STRING */
public:
	hello_world() : hello_str("Hello C++, world!")
	{
	}

	void welcome()
	{
#if defined(CONFIG_USE_STD_IOSTREAM)
		std::cout << hello_str << " " << CONFIG_BOARD << std::endl;
#else
#if defined(CONFIG_USE_STD_STRING)
		printf("%s %s\n", hello_str.c_str(), CONFIG_BOARD);
#else
		printf("%s %s\n", hello_str, CONFIG_BOARD);
#endif /* CONFIG_USE_STD_STRING */
#endif /* CONFIG_USE_STD_IOSTREAM */
	}

	void throw_exception()
	{
		throw std::runtime_error("The exception");
	}

};

int main(void)
{
	hello_world hw;

	hw.welcome();
	try {
		hw.throw_exception();
	} catch (const std::runtime_error* ex)
	{
		// Just re-throw it.
		throw ex;
	}

	return 0;
}
