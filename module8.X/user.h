/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);                /* I/O and Peripheral Initialization */
void SetLamps( uint8_t Lamps);     /* Set lamp state */
void SequenceUpdate( void);        /* Move to next pattern in sequence */
