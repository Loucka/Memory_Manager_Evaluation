----------------------------------- INTRODUCTION -----------------------------------------

As stated in the description, this project was administered as a technical evaluation for an
embedded software engineer position. Original instructions with redacted employer info
can be found within _EvaluationInstructions.pdf_. In short, the goal of this assessment
is to produce a memory management implementation for an embedded system that also provides 
bluetooth accessibilty alongisde sound error handling and unit testing.

----------------------------------- BUILDING & RUNNING -----------------------------------

For ease of use, the recommended approach for compiling/running of project executables is
through vscode. As such, these instructions will provide links for procuring and setting up
the required extensions and other toolchain items. Alternatively, terminal-based compilation
and execution is also viable. This brief guide is provided in an attempt to streamline this process.

NOTE: These instructions assume use of a Windows-based machine.

1. Install/Verify GCC 
    https://code.visualstudio.com/docs/cpp/config-mingw
 
2. Install/Verify CMake
    https://cmake.org/download/

3. With vscode installed, install the following extensions:
    - C/C++ (https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
    - CMake (https://marketplace.visualstudio.com/items?itemName=twxs.cmake)

4. Upon opening the project folder within vscode and selecting a proper compiler, build
    and execution options *should* be available in the bottom toolbar of the window.

5. With a desired test target selected, launch or debug at will. 

----------------------------------- IMPLEMENTATION OVERVIEW ------------------------------

While much of the provided sample source was preserved, some restructuring and functionality
changes were applied. The intent of this section is to begin by providing an overview of file
structure along with a brief background of each file. Along the way, pertinent details will
be provided as needed. Otherwise, additional implementation documentation can be found within
the source.

\Bluetooth - Parent folder for all Bluetooth-related objects.

    BluetoothDevice.cpp
        Base class for representing a generic bluetooth device implementation.

    SimulatedBluetoothDevice.cpp
        Child class of BluetoothDevice. Overrides as needed to provide simulated functionality.

    BluetoothManager.cpp
        Singleton class intended to manage one (or perhaps) many BluetoothDevice instances.

\Memory - Parent folder for all Memory-related objects.

    NvmManager.cpp
        Singleton Base class for representing a generic non-volatile memory management interface.

    FakeNvmManager.cpp
        Child class of NvmManager. Overrides as needed to provide a fake nvm implementation.

    MemoryPool.cpp
        Implements a fixed-size memory pool offering allocation, deallocation, compaction, and reporting.

    MemoryManager.cpp
        Intended as the "glue" for negotiating functionality between the other components of the project.

\Reporting - Parent folder for all Reporting-related objects. This ended up only being one, however.

    EventLogger.cpp
        Singleton class to maintain event information as raised. Also provides logging as needed.

Utils.cpp
    Collection of common utility functions for use throughout the project.

----------------------------------- TESTS ------------------------------------------------

For this solution, tests were implemented as individual executable targets. Each executable
targeting a specific module's group of tests.

----------------------------------- TODOS ------------------------------------------------

Throughout development, a number of opportunities were identified for either refinement
or future implementation. These opportunities have been added as comments within the project
source. Prepended with a "//#TODO" describing the intended outcome, these comments are to 
communicate a more ideal way of implementation. In addition to these comments, some broader
notes have been provided below.

    Additional Testing - Not all individual modules possess associated tests. Furthermore,
    higher-level integration testing is not complete.

    Pure virtuals/Abstract types - Use of pure virtuals was attempted in base classes, but proved troublesome
    at compilation. In the interest of time, it was decided to persist virtual functions, but
    with a base implementation.

    Use of Factories - In the case of generic instances, concrete assignments are peppered
    throughout the project. Instead, use of a factory manager that accepts registration of
    concrete types would be preferred. This way, the manager can be called upon to produce
    a concrete instance of an abstract type when needed. This allows for ease of development.

    Async actions - Specifically regarding Bluetooth interactions, rather than polling for received
    commands, a proper async solution should be present. Ideally, this is done by event invocation/handling.

    Thread safety - Attempts were made to enforce thread safety, but additional time/testing
    should be spent ensuring this.

    Destruction/Memory - Attempts were made to successfully free memory as needed through regular
    operation and object destruction. However, additional scrutiny is to be placed on making sure this
    solution is leak free.
