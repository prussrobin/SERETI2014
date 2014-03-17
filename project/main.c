/* 
 * File:   main.c
 * Author: Robin
 *
 * Created on 17 maart 2014, 13:39
 */

#include <stdio.h>
#include <stdlib.h>
#include "Rijbaan.h"
#include "Stoplicht.h"
#include "ntk.h"
/*
 * 
 */
int main(int argc, char** argv) {
    
    Rijbaan *R1;
    Rijbaan_Construct(&R1, 1, 2, 1, 2);
    Rijbaan_Print(R1);
    
    return (EXIT_SUCCESS);
}

