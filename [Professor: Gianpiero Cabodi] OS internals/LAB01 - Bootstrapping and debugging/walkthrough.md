# OS161 - System and Device Programming

* **Author**: `Davide Arcolini`
* **Date**: `2022-03-21`

## Table of contents
1. [Getting started](#getting-started)
2. [Bootstrapping](#bootstrapping)
3. [Modifying the Kernel](#modifying-the-kernel)
4. [Creating and running a new version of the operating system](#creating-and-running-a-new-version-of-the-operating-system)



## Getting started
Once completed the installation following the `kit-for-installation-on-ubuntu20` of `os161`, the working directory is: 
```
$(HOME)/os161/
```
This folder hosts some important directories: 
1. `os161/os161-base-2.0.3/`: contains the **Operating System** itself. It holds source codes, configuration files, object and executable files, of *os161*; this is the folder to edit and re-compile *os161*.
2. `os161/tools/`: this is the folder of all tools used for compilation/link, debug and other tasks (customized for the MIPS processor). It contains the required software packages (`binutils`, `gcc` compiler, `gdb` debugger, MIPS emulator, etc...).
3. `os161/root/`: it is the folder used to boot the operating system and to execute user processes.


> Remember that OS161 is executed by a MIPS processor emulator, SYS161

Therefore, all tools must be intended to work on a MIPS processor, and are renamed as, for example: 
- `gcc` is `mips-harvard-os161-gcc`;
- `gdb` is `mips-harvard-os161-gdb`;
- etc...

Executables of these tools are located in 
```
os161/tools/bin
```
The reference website is: [www.os161.org](http://www.os161.org/)

## Bootstrapping
There are two main ways to bootstrap *os161* in the *sys161* emulated environment (from a shell, using the “Terminal” application):
1. Normal execution: 
```console
cd $HOME/os161/root
sys161 kernel
```

2. Execution with a **debugger**: `sys161` is an executable in the host machine. The command `sys161 kernel` runs `sys161`, which is a program acting as a virtual machine with a MIPS processor. For this MIPS virtual machine, `kernel` is an executable file, loaded and executed.
The goal of all these laboratories is to interact with the `kernel` (executed in a MIPS machine, called `sys161`), and NOT TO interact with `sys161` itself.

    **NB**: it is recommended to have two differents shells opened. 
```console
#  FIRST SHELL 
# =============
sys161 -w kernel
```
The `-w` option specifies to wait for a debugger connection.
```console
#  SECOND SHELL 
# ==============
mips-harvard-os161-gdb kernel
```

## Modifying the Kernel
Refers to [this website](https://student.cs.uwaterloo.ca/~cs350/common/WorkingWith161.html).

> Every new kernel version corresponds to a configuration file (written uppercase) in the directory: `os161/os161user/kern/conf`

Examples are: DUMBVM, DUMBVM-OPT, GENERIC, HELLO, etc...

> There is a common source environment but the possibility to have multiple kernel configurations.

Procedure to create a modified version of the kernel:
1. **Edit the source files**. 
    1. Declare the new files required. The `conf.kern` file declares the files, options, and facilities in the system and the various files it includes. Then a kernel config (such as ASST1, or GENERIC, or TEST, or whatever) is used to select options and facilities for a particular kernel build.
    2. Eventually, make the new features optional to that particular kernel version with the command `defoption`. From the `conf.kern` file: 
    ```
    #       "defoption" declares the name of a kernel option. These are
    #       then turned on by including "options optname" in a
    #       kernel config.
    #
    #       Source files added with optfile are compiled in if the option
    #       specified is enabled. Source files added with optofffile are
    #       compiled in if the option specified is not enabled.
    #
    #       Additionally, a file "opt-optname.h" is created in the compile
    #       directory, which defines a C preprocessor symbol OPT_OPTNAME.
    #       This symbol is #defined to either 0 or 1 in the logical way.
    #       Thus, you can have small bits of code that are enabled or
    #       disabled by particular options by writing constructs like
    #
    #            #include "opt-foo.h"
    #            #if OPT_FOO
    #               code();
    #            #else
    #               other_code();
    #            #endif
    ```
2. **Create a new configuration file**. The configuration file is used by the compiler to correctly link all dependences and install it on the running kernel. The file has a list of options that may (or may not) be enabled in order to include (or include not) particular assets:
```bash
#options net    # Network stack (not supported)
options semfs	# Semaphores for userland

options sfs	    # Always use the file system
#options netfs  # You might write this as a project.

options dumbvm	# Chewing gum and baling wire
```
Some my be commented (not included) while other not. In order to **configure** the kernel in according to that specific file, run the command:
```console
cd $HOME/os161/src/kern/config
ls -la
./config <NEW_CONFIGURATION_FILE>
```
This process generates a new directory containing all the headers files required in the path `$HOME/os161/src/kern/compile`. Inside the `compile` directory, there are all the possible versions of the kernel developed and ready to be compiled and installed on the kernel.
```console
drwxr-xr-x  3 os161user os161user 12288 Mar 21 15:04 HELLO
drwxr-xr-x  3 os161user os161user 12288 Mar 21 15:11 THREADS
```

3. **Compile the file** with the options specified in the configuration file

```console
cd $HOME/os161/src/kern/compile/<NEW_CONFIGURATION_FILE>
bmake depend
bmake
bmake install
```
4. **Run the Kernel** from the `root` directory. The `kernel` is a link to last compiled and installed version.
```console
cd $HOME/os161/root/
sys161 kernel
```

## Creating and running a new version of the operating system

The `DUMBVM` version of *os161* is a simple version which does not do anything in particular. Maybe, we want to add some spicy and salt to our operating system. Let's create another version, **our** version.

The **workflow** is the following: 
1. Copy the `DUMBVM` configuration file (path: `os161/os161-base-2.0.3/kern/conf`) inside the same folder, with the name you want to give to that particular version. If you don't want to start from `DUMBVM` but from some other version of the operating system you have already built, just copy that one instead of `DUMBVM`, *you fuc\*ing dumb*... \
> Remember to add the `options <name>` to the configuration file (refers to [Modifying the Kernel](#modifying-the-kernel), *create a new configuration file*).
```console
cp os161/os161-base-2.0.3/kern/conf/DUMBVM os161/os161-base-2.0.3/kern/conf/NEW_COOL_VERSION
```

2. **Configure the operating system using the configuration file previously generated**.\
In order to do so, based on the fact that you are using `VSCode` or not: 
    - Within `VSCode`:
        - open the command palette (`ctrl` + `shift` + `P` or, in MacOS, `CMD` + `shift` + ;
        `P`)
        - navigate to `Tasks: Run Task`;
        - execute `Run Config`, indicating, when asked, the proper name of the version you want to run.
    
    - Without `VSCode`: run the command (obv, inside `os161/os161-base-2.0.3/kern/conf`): 
        ```
        ./config NEW_COOL_VERSION
        ```

3. **Compile the new operating system**.\
In order to do so, based on the fact that you are using `VSCode` or not: 
    - Within `VSCode`:
        - open the command palette (`ctrl` + `shift` + `P` or, in MacOS, `CMD` + `shift` + ;
        `P`)
        - navigate to `Tasks: Run Task`;
        - execute `Make Depend`, indicating, when asked, the proper name of the version you want to run.
        - execute `Build and Install`, indicating, when asked, the proper name of the version you want to run.
    
    - Without `VSCode`: run the command (less obv, inside `os161/os161-base-2.0.3/kern/compile/NEW_COOL_VERSION`): 
        ```
        bmake depend
        bmake
        bmake install
        ```
 
4. **Run the new operating system** 
In order to do so, based on the fact that you are using `VSCode` or not: 
    - Within `VSCode`:
        - open the command palette (`ctrl` + `shift` + `P` or, in MacOS, `CMD` + `shift` + ;
        `P`)
        - navigate to `Tasks: Run Task`;
        - execute `Run OS161 (no debug)` or `Run OS161`.
    
    - Without `VSCode`: refers to [Bootstrapping](#bootstrapping).