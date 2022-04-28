REVIEW ON HOW THE HELLO KERNEL WAS CREATED IN LAB1

Detailed steps to be done in order to generate the HELLO new kernel configuration

1) Generate two new files:

kern/include/hello.h
kern/main/hello.c

Fill them properly and modify kern/main/main.c in order to call the
hello() function. Remember to insert:
 
#include "opt-hello.h"

where appropriate, and use:

#if OPT_HELLO 
#endif

statements in order to make new parts of code optional.

2) Go to kern/conf and create new OS branch/configuration.

Define option "hello" and add optional file "hello.c" in kern.conf
You have to add lines:

defoption hello
optfile hello main/hello.c

copy the existing DUMBVM file to HELLO, edit HELLO and add a line:

options hello

create the new compilation branch:

./config HELLO

4) Go to directpry compile/HELLO, compile and install the new branch:

bmake depend
bmake
bmake install

If you get errors, understand them, correct and restart, typically at "bmake"

-------------------------------------------------------------------

LAB2

Detailed steps to be done in order to make the provided solution running.

1) Expand the .zip file in a directory. 

You will find 5 files:
dumbvm.c
file_syscalls.c
proc_syscalls.c
syscall.c
syscall.h

2) Put files in the right directories

Some of the files are modified versions of existing ones, some of them
are new. You have to guess which ones are modified/new and where to put them.

Check under os161/os161-base-2.0.3/kern

You will find already existing files:

- arch/mips/vm/dumbvm.c
you need to replace it with the new version in the given solution.
- arch/mips/syscall/syscall.c
you need to replace it with the new version in the given solution.

- include/kern/syscall.h
- include/syscall.h
So there are two syscall.h files. Which one to replace? If you look
into syscall.c you will easily notice that both of them are
included, so no particular clue. Take a look into both .h files, and you'll immediately
notice that the provided syscall.h replaces include/syscall.h
(include/kern/syscall.h is fully different) 

You will NOT find file_syscalls.c and proc_syscalls.c. They are indeed
the new files implementing the new system calls.
As written in the text of the lab, you should put them into the syscall
directpry.

3) Go to kern/conf and create new OS branch/configuration.

You have to "guess" the name of the option that you need to add (the
equivalent of "dumbvm", "hello", etc.). By taking a look in sysscall.h you
can see 

#include "opt-syscalls.h"

whereas in syscall.c you can find

#if OPT_SYSCALLS

so the option is clearly "syscalls".
So define option "syscalls" and add two new files in conf.kern as optional.
You have to add lines:

defoption syscalls
optfile syscalls syscall/file_syscalls.c
optfile syscalls syscall/proc_syscalls.c

copy the existing DUMBVM or HELLO to SYSCALLS
edit SYSCALLS and add a line:

options syscalls

Create the new compilation branch/configuration

./config SYSCALLS

4) go to directpry compile/SYSCALLS, compile and install the new branch

bmake depend
bmake
bmake install
