
#include <xc.h>
#include "system.h"
#include "user.h"


void main(void) {
    
    Initialise();
        
    // Enable interrupt and away we go
    INTCONbits.GIE = 1;
    
    while(1==1)
    {
        ;
    }
    
    return;
}
