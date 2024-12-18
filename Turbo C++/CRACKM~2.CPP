// crack this and... ill kill you






#include <CONIO.H>
#include <STRING.H>
#include <STDLIB.H>
#include <STDIO.H>
#include <DOS.H>

#define DBGF 0x00010000 // debug flag

/*void setvect(int num, void interrupt (* newIntHandler)(...));
void interrupt (* getvec(int num))(void); */

int isDebuggerPresent()
{
 unsigned int flags; // ;ref-1

 asm pushf;           // push the flags register onto the stack
 asm pop ax;          // pop the flags into AX
 asm mov flags, ax;   // move the flags into the `flags` variable (ref-1)

 if ( flags & DBGF )
 { 	/* check the 16th bit for the debug flag */
  return 1; // true
 }
 return 0; // no

// int found = 0;
/* void interrupt (* oldvector)(...) = getvect(1);
 void interrupt (* newvector)(...) = (void interrupt (*)(...))0x6666;
 setvec(1, newvector);

 asm int 1; // debug interrupt

 setvect(1, oldvector);

 if ( getvect(1) == newvector )
 {
  return 0;       // no
 }
 return 1; // true    */
/* asm
 {
  pushf
  pop ax
  test ax, DBGF
  jnz dbg_found
  jmp no_dbg

dbg_found:
  mov found, 1

no_dbg:
 }

 return found; */
}

//unsigned char lol = 0;

int generate()
{
 srand((unsigned int)time(NULL));
 return (rand() % 9000) + 1000;
}

int validate(const char * input, int key)
{
 char keyStr[20];
 sprintf(keyStr, "%d", key);
 return strcmp(input, keyStr) == 0;
}

void main()
{
 clrscr();
 int key = generate();
 char input[20];

 while ( 1 )
 {
  while ( isDebuggerPresent() )
  {
   asm int 0x13; // lolol
   asm int 3;
   asm iret;
   // goto die;
  }

  printf("Enter the key: ");
  gets(input);

  if ( validate(input, key) )
  {
   printf("ok");
   // lol = 1;
  }
  else
  {
   printf("wrong");
  }

//die:
  printf("\nPress any key to die . . . ");
  getch();
 // return 0 ? lol : -69;
  clrscr();
 }
}
