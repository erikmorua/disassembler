/*
Program 2 Disassembler
CS 530
Fall 2020
Erik Morua, cssc3051, Red ID: 816670615
Matthew Schuiteman, cssc3011, Red ID: 823022506
*/
#include <iomanip>
#include <iostream>
#include <string>
#include <cstdio>
#include <string.h>
#include <fstream> 
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <algorithm>
#define dis.h

FILE* readInput(char* argv[], const char* ptr);
void checkInput(int argc, char* argv[]);
void createInstructionMap(map<int, string>& imap);
void readSym(FILE**, vector<string>, vector<int>, vector<char>, vector<string>, vector<int>, vector<string>, vector<int>);
int getEndAddress(FILE**);
void readObj(FILE**, vector<string>, vector<int>, vector<char>, vector<string>, vector<int>, vector<string>, vector<int>, ofstream, ofstream);
void createOutFiles(char* argv[], ofstream& sic, ofstream& lis);


