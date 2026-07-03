# Bugbuster list



## Screen driver bugs

**Cursor placement and incorrect data type pointer**

On the screen driver there is a memory zone that controls what is display on the screen.
This memory zone is divided on cells. In each cell, there is the information of the character to display, 
and the format to display it, which take as a whole 2 bytes.

**Bug observation**

I observed the characters were displayed on the screen but the cursor didn't appear on the next case where the next character will be written.

Strangely I observed that if I placed the cursor in other case other than that one, it appeared. And while debugging, it appeared at the moment I set the character and its format on the cursor position, but once the cursor advanced, it didn't appear anymore.

**Explanation**

In my implementation, while writing the characters I was pointing at the memory zones with a uint32_t * pointer  to write the 16 bit value to cells of size of 16 bits. This created a situation where 

1. The correct char information was written on the current cell
2. The next adjacent cell was overwritten with zeros, since I have a uint32_t * pointer which indicates my data size is 32 bits.
3. At the end of my writing, the next cell, where the cursor placement should happen but was not seen, I had the cell as [0x00, 0x00].
4. The [0x00, 0x00] cell indicated that my background color was black and my text color was BLACK as well.
5. Doing some verification, I found out the color of the cursor is the same as the letter. And since everything was black, it was not being seen! (I played with gdb to confirm everything of the above and figure it out)


**Solution**

The solution is to only use a pointer of the size of your real data, not a bigger one, it can corrupt data even if that only means an invisible cursor.

PD: I discovered the memory multi-byte lecture is little-endian.


## Constants declaration on C

**Problem**

I tried to use a constant `SCREEN_WIDTH` that was originally declared on the file screen.c, into screen.h. 
For it to be included on time.c that could serve from it just by including screen.h.

I had an linkage error message that says that the `SCREEN_WIDTH` constant was being defined multiples times:
On the time.o and start.o files, the error messages says that the first declaration was on screen.h.


**Google it**

After an extensive google search I found that you dont define variables on headers files.

On quotes:
"If you put a definition of a global variable in a header file, then this definition will go to every .c file that includes this header, and you will get multiple definition error because a varible may be declared multiple times but can be defined only once."

If for some reason you need a constant that absolutely needs to be defined on the header, use the word static:
This guarantees that the constant is defined only one on your whole program and can be accessed multiple times.

Source:https://stackoverflow.com/questions/17764661/multiple-definition-of-linker-error
