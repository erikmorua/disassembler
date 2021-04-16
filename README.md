README
Program 2 Disassembler
CS 530
Fall 2020
Erik Morua
Matthew Schuiteman

File Manifest:
    dis.cpp
    dis.h
    makefile
    SDD
    

Compile instructions:
    On Edoras:
        Compile using g++ compiler with -o flag.
	    g++ -o dis dis.cpp
    Compile using the makefile
	make dis

Operating instructions:
    Required inputs:
	XE object file, <filename>.obj and its accompanying symbol file, <filename>.sym.
     Output:
	Generate an XE source file, <filename.sic> and an XE listing file,
	<filename>.lis using the disassembled code. The symbol file, <filename>.sym will
	contain the SYMTAB and LITTAB the assembler generated when assembling the object
	file.
    To test:
	dis 'file to be read from''second file to be read from'

List/description of novel/significant design decisions:
    One of the better decisions my partner and I made were using the instruction map as part of our program.
    there were multiple ways we could have done this part of the program but after trying out a few different things
    we decided this was the best and most efficient, and cleanest looking way of constructing the function.

List/description of all known deficiencies or bugs
    This program has not been tested or will account for files that are not in object code formats or symbol file formats.
    This program however has been tested and will recognize if not enough arguments have been input. 
    If the object code is off by any number of characters, the program will be thrown off as each space is column is 
    recognized to be responsible for part of the work.

Lessons Learned
    One of the most important lessons learned was the use of pair programming. Without pair programming, this program would have been
    dragged out longer than it would have needed to be. One of the benefits that comes with pair programming is a second pair of eyes.
    Something we realized was that we could bounce ideas off of each other and then give each other time to think of an idea that we could 
    progress with. Something we learned as part of the program was really how important each space was as each column is accounted for. If 
    something is off by 1 then the whole program is thrown off. It shows how computers really are amazing but also how error prone they could be 
    for a mistake so small.
