/*
 * Copyright (c) 2023, Piotr Pryga
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#if defined(CONFIG_USE_STD_IOSTREAM)
#include <iostream>
#else
#include <cstdio>
#endif /* CONFIG_USE_STD_IOSTREAM */

#if defined(CONFIG_USE_STD_STRING)
#include <string>
#endif /* CONFIG_USE_STD_STRING */

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
};

int main(void)
{
	hello_world hw;

	hw.welcome();

	return 0;
}
