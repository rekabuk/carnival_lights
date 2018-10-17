/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/


void TimeUpdate(void);         /* Keep track of time (100ms increments) */
void BitUpdate(void);          /* Keep track of bit timing */
void StartTx( void);           /* Sends a burst of data */
void SetUnitConfig(uint8_t Unit, uint8_t Address, uint8_t Lamps );
void SetUnitLamps(uint8_t Unit, uint8_t LampData );

