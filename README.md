# Linked List Implementation with Concurrent Operations

This repository contains implementations of a linked list data structure using three different approaches: serial programming, parallel programming with one mutex, and parallel programming with read-write locks. These implementations support Member(), Insert(), and Delete() functions and are designed to handle concurrent operations.

## Table of Contents

- [Getting Started](#getting-started)
- [Prerequisites](#prerequisites)
- [Building](#building)
- [Running the Programs](#running-the-programs)
- [Performance Evaluation](#performance-evaluation)
- [Contributing](#contributing)
- [License](#license)

## Getting Started

Follow the instructions below to set up and run the linked list implementations on your Windows system.

### Prerequisites

To run these programs on Windows, you need:

- A C/C++ compiler (e.g., MinGW, Visual Studio with C/C++ support)

### Building

Before running the programs, you may need to compile them. Use the provided Makefile or instructions below to build the programs:

#### Using MinGW (GCC)

1. Install MinGW with the MSYS2 package manager if you haven't already.

2. Open the command prompt.

3. Navigate to the project directory and run the following commands:
    for the serial one as the arguments you should give num_of_linked_list num_of_operations frac_of_member frac_of_insert frac_of_delete 

    for the mutex and read write codes as the arguments you should give num_of_linked_list num_of_operations num_of threads frac_of_member frac_of_insert frac_of_delete 

   gcc -o serial.out linked_list_with_serial.c -lpthread
   serial.out 1000 10000 0.99 0.005 0.005 
   

   gcc -o one_mutex.out linked_list_with_one_mutex.c -lpthread
   serial.out 1000 10000 0.99 0.005 0.005 


   gcc -o read_write.out linked_list_with_read_write_lock.c -lpthread
   read_write.out 1000 10000 0.99 0.005 0.005 