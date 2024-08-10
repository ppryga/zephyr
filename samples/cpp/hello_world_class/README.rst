.. _hello_cpp_world_class:

Hello C++ World Class
#####################

Overview
********

A simple :ref:`C++ <language_cpp>` sample that can be used with many supported board and prints
"Hello, C++ world!" to the console.

The sample application is a modified version of a samples/cpp/hello_wold sample. The changes done
are for undelining that C++ is not only STL and still can be used in embedded products.

One of the main points against C++ in embedded software is increased memory usage.
The sample provides additional configurations that can be used to check how much memory can be
saved when stop using some features from STL.

CONFIG_USE_STD_STRING enables use of std::string instread of raw char * for string sorage.
CONFIG_USE_STD_IOSTREAM enables use of std::iostream for printing output.

Memory usage with and without STL
*********************************

With options set to:
 - CONFIG_USE_STD_STRING=y
 - CONFIG_USE_STD_IOSTREAM=y
the simple application consumes pressy much memory:

Memory region      sed Size  Region Size  %age Used
       FLASH:      389680 B       512 KB     74.33%
         RAM:       16576 B       128 KB     12.65%
    IDT_LIST:           0 B        32 KB      0.00%

With options set to:
 - CONFIG_USE_STD_STRING=y
 - CONFIG_USE_STD_IOSTREAM=n
the simple application consumes way less memory:

Memory region     Used Size  Region Size  %age Used
       FLASH:      101404 B       512 KB     19.34%
         RAM:        7936 B       128 KB      6.05%
    IDT_LIST:           0 B        32 KB      0.00%

The FLASH difference is ~55%.

With options set to:
 - CONFIG_USE_STD_STRING=n
 - CONFIG_USE_STD_IOSTREAM=n
the simple application consumes way less memory:

Memory region     Used Size  Region Size  %age Used
       FLASH:       54532 B       512 KB     10.40%
         RAM:        7872 B       128 KB      6.01%
    IDT_LIST:           0 B        32 KB      0.00%

The FLASH difference is ~64%.

Memory usage with C++ exceptions use
************************************

When enabled C++ exceptions by CONFIG_CPP_EXCEPTIONS=y and CONFIG_CPP_RTTI=y
the memory footprint changes. This time it increases by about 9% in comparison
to FLASH usage without STL (10.40%).

Memory region     Used Size  Region Size  %age Used
       FLASH:      101860 B       512 KB     19.43%
         RAM:        7936 B       128 KB      6.05%
    IDT_LIST:           0 B        32 KB      0.00%

With added GNU compiler specific optimization, an overload of __gnu_cxx::__verbose_terminate_handler()
the memory footprint decreased again to ~12%.

Memory region     Used Size  Region Size  %age Used
       FLASH:       63908 B       512 KB     12.19%
         RAM:        7936 B       128 KB      6.05%
    IDT_LIST:           0 B        32 KB      0.00%

Custom exceptions memory allocator
**********************************

This is also GNU specific part, that is dengerous becasue it depends on particular version of libsupc++ library.
In case there is a need to use some custom exceptions memory allocation, then there are two additional functions
that need to be overloaded: __cxxabiv1::__cxa_allocate_exception and __cxxabiv1::__cxa_free_exception.

In the sample application default `malloc()` and `free()` call were substituted with Zephyr's `k_heap_alloc()`
and `k_heap_free()`. That caused increase in RAM usage by ~1KB, due to memory allocation for the local heap.

Current memory usage is:
Memory region     Used Size  Region Size  %age Used
       FLASH:       64032 B       512 KB     12.21%
         RAM:        8960 B       128 KB      6.84%
    IDT_LIST:           0 B        32 KB      0.00%

Building and Running
********************

This configuration can be built and executed on QEMU as follows:

.. zephyr-app-commands::
   :zephyr-app: samples/cpp/hello_world_class
   :host-os: unix
   :board: nrf52833dk/nrf52833
   :goals: run
   :compact:

To build for another board, change "nrf52833dk/nrf52833" above to that board's name.

Sample Output
=============

.. code-block:: console

    Hello C++, world! nrf52833dk

Exit QEMU by pressing :kbd:`CTRL+C`
