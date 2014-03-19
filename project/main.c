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
    
    Rijbaan *R1, *R2, *R3, *R4;
    Rijbaan_Construct(&R1, 1, 2, 1, 2, 3, 4);
    Rijbaan_Construct(&R2, 3, 4, 1, 2, 3, 4);
    Rijbaan_Construct(&R3, 5, 6, 1, 2, 3, 4);
    Rijbaan_Construct(&R4, 7, 8, 1, 2, 3, 4);
    Rijbaan_Print(R1);
    Rijbaan_Print(R2);
    Rijbaan_Print(R3);
    Rijbaan_Print(R4);
    
    return (EXIT_SUCCESS);
}

