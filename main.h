#define START_ADDR_DATA                     0x0CC0                                        /* The 5 bits LSB represents the address in the page specified for the 12 bits upper the 5 bits LSB.
                                                                                             this 5 bits LSB will be incremented internally by eeprom after writing a byte (the pointer incremented)
                                                                                             . After writing in the last address of the specifed page, the pointer will be rolled over to the first
                                                                                             address in the page
                                                                                          */