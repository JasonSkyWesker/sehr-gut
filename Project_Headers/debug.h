#ifndef DEBUG_H_
#define DEBUG_H_

#define key1 SIU.GPDI[52].R
#define key2 SIU.GPDI[53].R
#define key3 SIU.GPDI[54].R
#define key4 SIU.GPDI[55].R

extern void set_key(void);
extern int flagkey1;
extern int flagkey2;
extern int flagkey3;
extern int flagkey4;

#endif /* DEBUG_H_ */
