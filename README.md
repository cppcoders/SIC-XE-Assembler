# SIC/XE ASSEMBLER
##  What is SIC/XE Machhine
* SIC Machine's extended Edition
* you can read article at [SIC/XE](https://en.wikipedia.org/wiki/Simplified_Instructional_Computer) on wikipedia
## Function
* Control Section
* Addressing Mode (Flag bit)
  * Direct Addressing Mode
  * Indirect Addressing Mode
  * Simple Addressing Mode
  * Immediate Addressing Mode
  * Relative Addressing Mode
    *   1. Program Counter (PC Register)
    *   2. Base (Base Register)
* Extended Instruction (4bit Instruction)

## How it works :
Using two passes ...
* Pass one
  * Assign addresses to all statements in source code
  * Save values (addresses) assigned to labels for use in pass two
  * Process directives
* Pass two
  * Translate instructions
  * Convert labels to addresses
  * Generate values defined by BYTE and WORD
  * Process the directives not done in pass one
  * Write object code to output file
## Sample input :
````
COPY START 0
FIRST STL RETADR
  LDB #LENGTH
  BASE LENGTH
CLOOP +JSUB RDREC
  LDA LENGTH
  COMP #0
  JEQ ENDFIL
  +JSUB WRREC
  J CLOOP
ENDFIL LDA EOF
  STA BUFFER
  LDA #3
  STA LENGTH
  +JSUB WRREC
  J @RETADR
EOF BYTE C'EOF'
RETADR RESW 1
LENGTH RESW 1
BUFFER RESB 4096
RDREC CLEAR X
  CLEAR A
  CLEAR S
  +LDT #4096
RLOOP TD INPUT
  JEQ RLOOP
  RD INPUT
  COMPR A,S
  JEQ EXIT
  STCH BUFFER,X
  TIXR T
  JLT RLOOP
EXIT STX LENGTH
  RSUB   
INPUT BYTE X'F1'
WRREC CLEAR X
  LDT LENGTH
WLOOP TD OUTPUT
  JEQ WLOOP
  LDCH BUFFER,X
  WD OUTPUT
  TIXR T
  JLT WLOOP
  RSUB  
OUTPUT BYTE X'05'
  END FIRST   
````

Sample Output: 
````
H^COPY  ^000000^001077
T^000000^10^17202D^69202D^4B101036^032026^290000
T^000010^10^332007^4B10105D^3F2FEC^032010^0F2016
T^000020^10^010003^0F200D^4B10105D^3E2003^454F46
T^001036^0D^B410^B400^B440^75101000^E32019
T^001043^0E^332FFA^DB2013^A004^332008^57C003
T^001051^0C^B850^3B2FEA^134000^4F0000^F1
T^00105D^0E^B410^774000^E32011^332FFA^53C003
T^00106B^0C^DF2008^B850^3B2FEF^4F0000^05
M^000007^05
M^000014^05
M^000027^05
E^000000
````
