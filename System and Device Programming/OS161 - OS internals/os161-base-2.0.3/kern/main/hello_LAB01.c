/**
 * @file hello_LAB01.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief LAB01: create a new Kernel version
 * @version 0.1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <types.h>
#include <lib.h>

#include "hello_LAB01.h"

#if OPT_LAB01
void hello_LAB01(void) {
    kprintf("[!] Included LAB01 (create a new Kernel version)\n");
    return;
}
#endif