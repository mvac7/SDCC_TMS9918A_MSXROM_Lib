/* =============================================================================
   Test VDP TMS9918A MSX ROM SDCC Lib
   v1.2 (13/04/2018)
   Description:
    test SDCC TMS9918A MSX ROM Lib (VDP_TMS9918A)
    source for ROM 32k and MSX-DOS (requires the corresponding CRT)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxBIOS.h"
#include "../include/msxSystemVars.h"

#include "../include/VDP_TMS9918A_MSXROM.h"



#define  HALT __asm halt __endasm   //wait for the next interrupt



// Definitions of Labels -------------------------------------------------------




//  definition of functions  ---------------------------------------------------
void WAIT(uint cicles);
char INKEY();
void LOCATE(char x, char y);
char PEEK(uint address);
void POKE(uint address, char value);

void CLS();

void VPRINT(char column, char line, char* text);  //print in screen 1 or 2
void VPRINT0(char column, char line, char* text);
void VPOKEARRAY(uint vaddr, char* text);

void testSCREEN0();
void testSCREEN1();
void testSCREEN2();
void testSCREEN3();

void testFill();
void testColor();
void testVpeekVpoke();

void showTestCard();

void fillOrdered();



// constants  ------------------------------------------------------------------

const char text01[] = "Test SDCC TMS9918A ROM Lib"; 
const char text02[] = "v1.2 (13/04/2018)";
const char text03[] = "Press any key to continue.";


// Project: tMSgFX_def_tilesets
// tileset pattern
// Tiles range: 32 to 127
// Size= 768
const char TILESET_B0[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x0C,0x0C,0x00,0x0C,0x00,
0x00,0x14,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x3E,0x14,0x3E,0x14,0x00,0x00,
0x08,0x3E,0x68,0x3E,0x0B,0x3E,0x08,0x00,0x00,0x33,0x36,0x0C,0x1B,0x33,0x00,0x00,
0x00,0x38,0x6C,0x3D,0x66,0x3D,0x00,0x00,0x00,0x18,0x30,0x00,0x00,0x00,0x00,0x00,
0x06,0x0C,0x0C,0x0C,0x0C,0x0C,0x06,0x00,0x30,0x18,0x18,0x18,0x18,0x18,0x30,0x00,
0x00,0x5A,0x3C,0x7E,0x3C,0x5A,0x00,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x18,0x10,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,0x00,0x00,
0x00,0x3E,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x18,0x38,0x18,0x18,0x3C,0x00,0x00,
0x00,0x7E,0x03,0x3E,0x60,0x7F,0x00,0x00,0x00,0x7E,0x03,0x1E,0x03,0x7E,0x00,0x00,
0x00,0x63,0x63,0x7F,0x03,0x03,0x00,0x00,0x00,0x7F,0x60,0x7E,0x03,0x7E,0x00,0x00,
0x00,0x3E,0x60,0x7E,0x63,0x3E,0x00,0x00,0x00,0x7F,0x03,0x06,0x0C,0x0C,0x00,0x00,
0x00,0x3E,0x63,0x3E,0x63,0x3E,0x00,0x00,0x00,0x3E,0x63,0x3F,0x03,0x3E,0x00,0x00,
0x00,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x00,0x18,0x18,0x10,0x00,
0x00,0x0C,0x18,0x30,0x18,0x0C,0x00,0x00,0x00,0x00,0x3C,0x00,0x3C,0x00,0x00,0x00,
0x00,0x30,0x18,0x0C,0x18,0x30,0x00,0x00,0x00,0x1E,0x33,0x06,0x0C,0x00,0x0C,0x00,
0x3E,0x41,0x5D,0x55,0x5F,0x40,0x3E,0x00,0x00,0x3E,0x63,0x7F,0x63,0x63,0x00,0x00,
0x00,0x7E,0x63,0x7E,0x63,0x7E,0x00,0x00,0x00,0x3F,0x60,0x60,0x60,0x3F,0x00,0x00,
0x00,0x7E,0x63,0x63,0x63,0x7E,0x00,0x00,0x00,0x7F,0x60,0x7C,0x60,0x7F,0x00,0x00,
0x00,0x7F,0x60,0x7C,0x60,0x60,0x00,0x00,0x00,0x3F,0x60,0x67,0x63,0x3F,0x00,0x00,
0x00,0x63,0x63,0x7F,0x63,0x63,0x00,0x00,0x00,0x0C,0x0C,0x0C,0x0C,0x0C,0x00,0x00,
0x00,0x0C,0x0C,0x0C,0x0C,0x38,0x00,0x00,0x00,0x63,0x66,0x7C,0x66,0x63,0x00,0x00,
0x00,0x60,0x60,0x60,0x60,0x7F,0x00,0x00,0x00,0x63,0x77,0x7F,0x6B,0x63,0x00,0x00,
0x00,0x63,0x73,0x7B,0x6F,0x67,0x00,0x00,0x00,0x3E,0x63,0x63,0x63,0x3E,0x00,0x00,
0x00,0x7E,0x63,0x63,0x7E,0x60,0x00,0x00,0x00,0x3E,0x63,0x6B,0x66,0x3D,0x00,0x00,
0x00,0x7E,0x63,0x63,0x7E,0x63,0x00,0x00,0x00,0x3F,0x60,0x3E,0x03,0x7E,0x00,0x00,
0x00,0x7E,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,
0x00,0x63,0x63,0x63,0x36,0x1C,0x00,0x00,0x00,0x63,0x6B,0x7F,0x77,0x63,0x00,0x00,
0x00,0x63,0x36,0x1C,0x36,0x63,0x00,0x00,0x00,0x66,0x66,0x3C,0x18,0x18,0x00,0x00,
0x00,0x7E,0x0C,0x18,0x30,0x7E,0x00,0x00,0x0E,0x0C,0x0C,0x0C,0x0C,0x0C,0x0E,0x00,
0x00,0x60,0x30,0x18,0x0C,0x06,0x00,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x38,0x00,
0x10,0x38,0x6C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x03,0x3F,0x63,0x7F,0x00,0x00,
0x60,0x7E,0x63,0x63,0x63,0x7E,0x00,0x00,0x00,0x3F,0x60,0x60,0x60,0x3F,0x00,0x00,
0x03,0x3F,0x63,0x63,0x63,0x3F,0x00,0x00,0x00,0x3E,0x63,0x7F,0x60,0x3E,0x00,0x00,
0x00,0x3E,0x60,0x78,0x60,0x60,0x00,0x00,0x00,0x3E,0x63,0x63,0x3F,0x03,0x3E,0x00,
0x60,0x7E,0x63,0x63,0x63,0x63,0x00,0x00,0x0C,0x00,0x0C,0x0C,0x0C,0x0C,0x00,0x00,
0x0C,0x00,0x0C,0x0C,0x0C,0x0C,0x38,0x00,0x00,0x60,0x66,0x7C,0x66,0x63,0x00,0x00,
0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x7E,0x6B,0x6B,0x6B,0x6B,0x00,0x00,
0x00,0x7E,0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x3E,0x63,0x63,0x63,0x3E,0x00,0x00,
0x00,0x7E,0x63,0x63,0x63,0x7E,0x60,0x00,0x00,0x3F,0x63,0x63,0x63,0x3F,0x03,0x00,
0x00,0x3F,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x3F,0x60,0x3E,0x03,0x7E,0x00,0x00,
0x18,0x3E,0x18,0x18,0x18,0x0E,0x00,0x00,0x00,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,
0x00,0x63,0x63,0x63,0x36,0x1C,0x00,0x00,0x00,0x63,0x6B,0x6B,0x6B,0x36,0x00,0x00,
0x00,0x66,0x3C,0x18,0x3C,0x66,0x00,0x00,0x00,0x63,0x63,0x63,0x3F,0x03,0x3E,0x00,
0x00,0x7E,0x0C,0x18,0x30,0x7E,0x00,0x00,0x06,0x0C,0x0C,0x18,0x0C,0x0C,0x06,0x00,
0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00,0x30,0x18,0x18,0x0C,0x18,0x18,0x30,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};



// Functions -------------------------------------------------------------------


//
void main(void)
{
  POKE(LINL32,32);
  
  COLOR(15,4,4);
  SCREEN(1);

  VPRINT(0,1,text01);
  VPRINT(0,2,text02);
  VPRINT(0,3,text03);
  
  LOCATE(0,4);
  INKEY();
  
//TEST -------------------------------------------------------------------------   
  
  testColor();
  WAIT(100);
  
  testSCREEN0();  
  WAIT(100);

  testSCREEN1();  
  WAIT(100);
  
  testSCREEN2();  
  WAIT(100);
  
  testSCREEN3();  
  WAIT(100);

//END --------------------------------------------------------------------------  
  
  COLOR(15,4,4);
  SCREEN(1);
  
  VPRINT(0,0,"END");
  WAIT(30*10);
   
}



/* =============================================================================
One character input (waiting)
============================================================================= */
char INKEY() __naked
{
__asm 
  
  call CHGET
  ld   L,A

  ret
__endasm;
}



/* =============================================================================
   Set a position the cursor in to the specified location
   Posiciona el cursor hasta la ubicacion especificada
   x(byte) - column (0 to 31 or 39)
   y(byte) - line   (0 to 23)
============================================================================= */
void LOCATE(char x, char y)
{
x;y;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX) ;x
  inc  A       ;incrementa las posiciones para que se situen correctamente en la pantalla
  ld   H,A
  ld   A,5(IX) ;y
  inc  A
  ld   L,A
     
  call POSIT
  
  pop  IX
__endasm;

}



// Generates a pause in the execution of n interruptions.
// PAL: 50=1second. ; NTSC: 60=1second. 
void WAIT(uint cicles)
{
  uint i;
  for(i=0;i<cicles;i++) HALT;
}



char PEEK(uint address) __naked
{
address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
    
  ld   L,4(ix)
  ld   H,5(ix)
  ld   A,(hl)

  ld   L,A

  pop  IX
  ret
__endasm;
}



void POKE(uint address, char value)
{
address;value;
__asm
  push IX
  ld   IX,#0
  add  IX,SP 
    
  ld   L,4(ix)
  ld   H,5(ix)
  ld   A,6(ix)
  ld   (HL),A

  pop  IX  
__endasm;
}



void CLS()
{
  FillVRAM(BASE10, 0x300, 32);
}





//print in screen 1 or 2
void VPRINT(char column, char line, char* text)
{
  uint vaddr = BASE10 + (line*32)+column; // calcula la posicion en la VRAM
  VPOKEARRAY(vaddr, text);
}



//print in screen 0
void VPRINT0(char column, char line, char* text)
{
  uint vaddr = BASE0 + (line*40)+column; // calcula la posicion en la VRAM
  VPOKEARRAY(vaddr, text);
}



void VPOKEARRAY(uint vaddr, char* text)
{
  while(*(text)) VPOKE(vaddr++,*(text++));
}





// TEST SCREEN 0 ###############################################################
void testSCREEN0()
{
  char i;
  
  COLOR(LIGHT_GREEN,DARK_GREEN,DARK_GREEN);    
  SCREEN(0);

  VPRINT0(31,23," SCREEN 0");
  WAIT(50);
  
  //fillOrdered();
  for(i=0;i<255;i++) VPOKE(BASE0+i,i);
  WAIT(50);
  
  testVpeekVpoke();
  WAIT(50);
  
  //testFill
  VPRINT0(0,0,"FillVRAM()");
  for(i=32;i<95;i++)
  {
    HALT;
    HALT;
    HALT;
    HALT;
    
    FillVRAM (BASE0+10, 942, i);    
  }
  
}



// TEST SCREEN 1 ###############################################################
void testSCREEN1()
{
  COLOR(15,5,1);    
  SCREEN(1);

  VPRINT(23,23," SCREEN 1");
  WAIT(50);
  
  fillOrdered();
  WAIT(50);
  
  VPRINT(0,10,"Test CopyToVRAM()");  
  WAIT(50);
  
  //copy to VRAM tileset, only gfx patterns
  CopyToVRAM((uint) TILESET_B0,BASE12+(32*8),96*8);
  WAIT(50);
   
  /* colors
  The BG Colors array defines 32 colors (each 4 bit background, and 4 bit 
  foreground, as in VDP register 7). The colors are assigned to the BG Tiles as
  follows: Tiles 00..07 share the first color, tiles 08..0F share the second 
  color, etc, and tiles F8..FF share the last color.*/
  FillVRAM(BASE6,32,0x75);
  
  WAIT(50);
  
  testFill();
  
}



// TEST SCREEN 2 ###############################################################
void testSCREEN2()
{
  COLOR(15,4,1);    
  SCREEN(2);

  VPRINT(23,23," SCREEN 2");
  
  //copy to VRAM tileset, only gfx patterns and fill colors
  //bank 0
  HALT;
  CopyToVRAM((uint) TILESET_B0,BASE12+(32*8),96*8);
  FillVRAM(BASE11+(32*8),96*8,0x96); //colors (Red)
    
  //bank 1
  CopyToVRAM((uint) TILESET_B0,BASE12+0x800+(32*8),96*8);
  FillVRAM(BASE11+BANK1+(32*8),96*8,0x3C); //colors (green)
  
  //bank 2
  CopyToVRAM((uint) TILESET_B0,BASE12+0x1000+(32*8),96*8);
  FillVRAM(BASE11+BANK2+(32*8),96*8,0x54); //colors (blue)
  WAIT(50);
  //END copy
  
  
  //test CopyFromVRAM
  VPRINT(0,5,"Test CopyFromVRAM()");
  //copy VRAM to RAM
  CopyFromVRAM(BASE12+(32*8),0xE000,64*8);
  WAIT(50);
  
  //copy RAM to VRAM
  CopyToVRAM(0xE000,BASE12+0x600,64*8);   
  FillVRAM(BASE11+0x600,64*8,0xFE); //colors
  WAIT(120);
  //END CopyFromVRAM
  
  
  //test fill VRAM  
  testFill();

}



// TEST SCREEN 3 ###############################################################
void testSCREEN3()
{
  char value=0;
  uint i;
  uint vaddr = BASE17;
  
  COLOR(15,4,4);
  SCREEN(1);
  
  VPRINT(0,0,"Test SCREEN 3");  
  WAIT(200);
  
     
  SCREEN(3);
  
  for(i=0;i<0x600;i++)
  {
    VPOKE(vaddr++,value);
    if(value<255) value++;
    else value=0; 
  }
  WAIT(250);

}





// show tileset
void fillOrdered()
{
  char i;
  for(i=0;i<255;i++) VPOKE(BASE5+i,i);
}



// TEST COLOR  #################################################################
void testColor()
{
  char i;
  char inkcol=15;

  COLOR(15,0,4);
  SCREEN(0);

  VPRINT0(15,10,"TEST color");
  WAIT(50);
  
  for(i=0;i<16;i++)
  { 
    COLOR(inkcol--,i,i);
        
    WAIT(15);
  }  
}



// TEST VPEEK & VPOKE  #########################################################
// copy a bloq using vpeek and vpoke
void testVpeekVpoke()
{
  uint vaddr = BASE0;
  char i;
  char value;
  
  VPRINT0(0,9,"TEST VPEEK & VPOKE:");
  
  for(i=0;i<255;i++)
  {
    HALT;
    value = VPEEK(vaddr);
    VPOKE(vaddr+440,value);
    vaddr++;  
  }  

}



// TEST FILL  ##################################################################
//fill screen map with a characters range 
void testFill()
{
  char i;

  VPRINT(0,0,"FillVRAM()");
  for(i=32;i<95;i++)
  {
    HALT;
    
    FillVRAM (BASE10+10, 750, i);
    
    HALT;
    HALT;
    HALT;
  }  
  
}


