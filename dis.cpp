/*
Erik Morua, cssc3051, Red ID: 816670615
Matthew Schuiteman, cssc3011, Red ID: 823022506
CS530 Systems Programming
Fall 2020, Prof. Guy
Program 2 Disassembler
Description: An XE Disassembler program. Given an object file and a symbol file,
the XE Disassembler program will generate an XE source file and a listing file.
*/
#include "dis.h"
using namespace std;

int main(int argc, char* argv[]) {

    // check files passed in
    checkInput(argc, argv);

    // open and create pointers 
    FILE* fptr[2] = { readInput(argv, ".sym"), readInput(argv, ".obj") };

    // memory to store symbols and literals from sym file
    vector<string> symbols;
    vector<int> addresses;
    vector<char> flags;
    // literals
    vector<string> literals;
    vector<int> lens;
    vector<string> names;
    vector<int> litAddr;

    // read symbol file, populate vectors
    readSym(fptr, symbols, addresses, flags, literals, lens, names, litAddr);

    // create output files
    ofstream sic, lis;
    createOutFiles(argv, sic, lis);

    // read object file
    readObj(fptr, symbols, addresses, flags, literals, lens, names, litAddr, sic, lis);

    // get end address
    int finalAddress = getEndAddress(fptr);

    // close files
    sic.close();
    lis.close();

    // close pointers
    fclose(fptr[0]);
    fclose(fptr[1]);
    return 0;
}

// IN: copy of a file, used for object and symbol file
// OUT: returns a pointer to the open file
FILE* readInput(char* argv[], const char* ptr) {
    char buffer[30];
    FILE* inFile;
    strcpy(buffer, argv[1]);
    strcat(buffer, ptr);
    inFile = fopen(buffer, "r");
    return inFile;
}

/*
check input function checks inputs to make sure the correct files
and number of arguments are being entered.
*/
void checkInput(int argc, char* argv[]) {
    //need two files to run program, else exit.
    if (argc > 2 || argc < 2) {
        cout << "Must enter two arguments." << endl;
        exit(1);
    }

    // uses read input function to read object and symbol files.
    FILE* fptr[2] = { readInput(argv, ".sym"), readInput(argv, ".obj") };

    // error checking for number of files
    if ((fptr[0] == NULL) || (fptr[1] == NULL)) {
        cout << "Could not read files. " << endl;
        fclose(fptr[0]); // close file pointers
        fclose(fptr[1]);
        exit(1);
    }
    fclose(fptr[0]);
    fclose(fptr[1]);
}

/*
IN: File pointer
Function accepts a pointer to a file and runs it through a buffer to find the end address
OUT: returns the last address of the file.
*/
int getEndAddress(FILE** fptr) {
    char buffer[120];
    rewind(fptr[1]);
    fgets(buffer, 120, fptr[1]);
    string res = buffer;
    int result = strtol(res.substr(13, 6).c_str(), 0, 16);
    return result;
} 
void createInstructionMap(map<int, string>& imap) {
    imap.insert(pair<int, string>(0x18, "ADD3"));
    imap.insert(pair<int, string>(0x90, "ADDR2"));
    imap.insert(pair<int, string>(0x58, "ADDF3"));
    imap.insert(pair<int, string>(0x40, "AND3"));
    imap.insert(pair<int, string>(0xB4, "CLEAR2"));
    imap.insert(pair<int, string>(0x28, "COMP3"));
    imap.insert(pair<int, string>(0x88, "COMPF3"));
    imap.insert(pair<int, string>(0xA0, "COMPR2"));
    imap.insert(pair<int, string>(0x24, "DIV3"));
    imap.insert(pair<int, string>(0x9C, "DIVR2"));
    imap.insert(pair<int, string>(0x64, "DIVF3"));
    imap.insert(pair<int, string>(0xC4, "FIX1"));
    imap.insert(pair<int, string>(0xC0, "FLOAT1"));
    imap.insert(pair<int, string>(0xF4, "HIO1"));
    imap.insert(pair<int, string>(0x3C, "J3"));
    imap.insert(pair<int, string>(0x30, "JEQ3"));
    imap.insert(pair<int, string>(0x34, "JGT3"));
    imap.insert(pair<int, string>(0x38, "JLT3"));
    imap.insert(pair<int, string>(0x48, "JSUB3"));
    imap.insert(pair<int, string>(0x00, "LDA3"));
    imap.insert(pair<int, string>(0x68, "LDB3"));
    imap.insert(pair<int, string>(0x50, "LDCH3"));
    imap.insert(pair<int, string>(0x08, "LDL3"));
    imap.insert(pair<int, string>(0x70, "LDF3"));
    imap.insert(pair<int, string>(0x6C, "LDS3"));
    imap.insert(pair<int, string>(0x74, "LDT3"));
    imap.insert(pair<int, string>(0x04, "LDX3"));
    imap.insert(pair<int, string>(0xD0, "LPS3"));
    imap.insert(pair<int, string>(0x20, "MUL3"));
    imap.insert(pair<int, string>(0x60, "MULF3"));
    imap.insert(pair<int, string>(0x98, "MULR2"));
    imap.insert(pair<int, string>(0xC8, "NORM1"));
    imap.insert(pair<int, string>(0x44, "OR3"));
    imap.insert(pair<int, string>(0xD8, "RD3"));
    imap.insert(pair<int, string>(0xAC, "RMO2"));
    imap.insert(pair<int, string>(0x4C, "RSUB3"));
    imap.insert(pair<int, string>(0xA4, "SHIFTL2"));
    imap.insert(pair<int, string>(0xA8, "SHIFTR2"));
    imap.insert(pair<int, string>(0xF0, "SIO1"));
    imap.insert(pair<int, string>(0xEC, "SSK3"));
    imap.insert(pair<int, string>(0x0C, "STA3"));
    imap.insert(pair<int, string>(0x78, "STB3"));
    imap.insert(pair<int, string>(0x54, "STCH3"));
    imap.insert(pair<int, string>(0x80, "STF3"));
    imap.insert(pair<int, string>(0xD4, "STI3"));
    imap.insert(pair<int, string>(0x14, "STL3"));
    imap.insert(pair<int, string>(0x7C, "STS3"));
    imap.insert(pair<int, string>(0xE8, "STSW3"));
    imap.insert(pair<int, string>(0x84, "STT3"));
    imap.insert(pair<int, string>(0x10, "STX3"));
    imap.insert(pair<int, string>(0x1C, "SUB3"));
    imap.insert(pair<int, string>(0x5C, "SUBF3"));
    imap.insert(pair<int, string>(0x94, "SUBR2"));
    imap.insert(pair<int, string>(0xB0, "SVC2"));
    imap.insert(pair<int, string>(0xE0, "TD3"));
    imap.insert(pair<int, string>(0xF8, "TIO1"));
    imap.insert(pair<int, string>(0x2C, "TIX3"));
    imap.insert(pair<int, string>(0xB8, "TIXR2"));
    imap.insert(pair<int, string>(0xDC, "WD3"));

}

/*
IN: pointers to our files and addresses flags etc.
    lit tab table stored
*/
void readSym(FILE** fptr, vector<string>& symbols, vector<int>& addresses,
    vector<char>& flags, vector<string>& literals, vector<int>& lens,
    vector<string>& names, vector<int>& litAddr) {

    char lines[160];
    char res[10];
    int value;
    bool check = true;

    // file pointer over symbol file header
    fgets(lines, 50, fptr[0]);

    //file pointer over symbol data
    fgets(lines, 80, fptr[0]);

    while (check) {

        fscanf(fptr[0], "%s", res);

        // if we come across a name, start reading lit table
        if (strcmp(res, "Name") == 0) {
            check = false;
            break;
        }

        /*
        Since we got past the If check above, res is already a symbol, so just add it to symtab
        */
        symbols.push_back(res);

        // get hex address and push into addresses
        fscanf(fptr[0], "%x", &value);
        addresses.push_back(value);

        fscanf(fptr[0], "%s", res);
        flags.push_back(res[0]);
    }

    // skip header
    fgets(lines, 70, fptr[0]);
    fscanf(fptr[0], "%s", res);

    // reading from the symbol file
    int j = 0;
    while (fgets(lines, 80, fptr[0])) {
        fscanf(fptr[0], "%s", res);

        //if char is = it doesnt have a name
        if (strncmp(&res[0], "=", 1) == 0) {
            names.push_back("");
            literals.push_back(res);

            // store length in value and push store it in lens
            fscanf(fptr[0], "%x", &value);
            lens.push_back(value);

            // store lit address into value and store in litAddr<int>
            fscanf(fptr[0], "%x", &value);
            litAddr.push_back(value);
            
            //if theres multiples
            litAddr.erase(unique(litAddr.begin(), litAddr.end()), litAddr.end());
        }

        //if literal has a name, store it with res
        else {

            names.push_back(res);

            //multiples
            names.erase(unique(names.begin(), names.end()), names.end());

            // store in res and push it to literals
            fscanf(fptr[0], "%s", res);
            literals.push_back(res);

            // store in val push to lens
            fscanf(fptr[0], "%x", &value);
            lens.push_back(value);
            // store lit addressses into val and push onto litAddr
            fscanf(fptr[0], "%x", &value);
            litAddr.push_back(value);
            litAddr.erase(unique(litAddr.begin(), litAddr.end()), litAddr.end());
        }
        j++;
    }
}



// Takes in object file, creates map checks opcodes
void readObj(FILE** fptr, vector<string>& symbols, vector<int>& addresses, vector<char>& flags,
    vector<string>& literals, vector<int>& lens, vector<string>& names, vector<int>& litAddr, ofstream& sic, ofstream& lis) {
    map<int, string> opTab;
    createInstructionMap(opTab);
    int address = 000000;
    // records could only be 150 max 
    int maxSize = 150;
    string opFormat;
    string match;
    //flags set to false, change later on to decide the mode
    bool n, i, x, b, p, e = false;
    char line[150];
    bool immediate = false;
    int position = 0;
    //if base directive change to true
    bool base = false;
    bool litOp = false;
    string objCode;

    // object file loop
    while (fgets(line, maxSize, fptr[1])) {
        string current = line; 
        //position resets after each loop
        position = 0;

        // header record
        if (line[0] == 'H') {
            string programName = current.substr(1, 6);
            position += 7;
            address = strtol(current.substr(position, 6).c_str(), 0, 16);

            lis << setw(4) << setfill('0') << uppercase << hex << address << " ";
            lis << setw(8) << setfill(' ') << left << programName;
            lis << " START   " << address << endl;
            sic << setw(8) << setfill(' ') << left << programName;
            sic << " START   " << address << endl;

        }

        // text record
        if (line[0] == 'T') {
            address = strtol(current.substr(1, 6).c_str(), 0, 12);
            position += 9;

            // loop until end of line 
            while (line[position] != '\n') {
                // reset flag bits to false because new line
                n, i, x, b, p, e = false; 

                // check for symbol
                for (int i = 0; i < addresses.size(); i++) {
                    if (addresses[i] == address) match = symbols[i];
                }

                for (int b = 0; b < literals.size(); b++) {

                    string literalT = literals[b].substr(1, 1);
                    string lit = literals[b].substr(3, lens[b]);
                    string strLit;

                    if (strcmp(&literalT[0], "X") == 0) {

                    }
                    else if (strcmp(&literalT[0], "C") == 0) {

                    }
                    else {

                    }
                    break;
                }
                int opcode = strtol(current.substr(position, 2).c_str(), 0, 12);
                int remainder = opcode % 4;
                opcode -= remainder;


                // match instruction to opcode
                string opInstructionFormat = opTab[opcode];

                string opInstruction = opInstructionFormat.substr(0, opInstructionFormat.size() - 1);
                string opF = opInstructionFormat.substr(opInstructionFormat.size() - 1, 1);
                int opFormat = stoi(opF);


                // set n & i bits 
                if (remainder == 1) {
                    n = false;
                    i = true;
                    immediate = true;
                }
                else if (remainder == 2) {
                    n = true;
                    i = false;
                }
                else if (remainder == 3) {
                    n, i = true;
                }
                else {
                    n, i = false;
                }

                // cases for different instruction formats
                if (opFormat == 1) {
                    // prints location
                    lis << hex << address << " ";

                }
                else if (opFormat == 2) {
                    lis << setw(4) << setfill('0') << hex << address;
                    lis << setw(8) << setfill(' ') << match;
                    sic << setw(8) << setfill(' ') << match;

                    lis << " " << setw(8) << setfill(' ') << left << opInstruction;
                    sic << " " << setw(8) << setfill(' ') << left << opInstruction;

                    objCode = current.substr(position, 6);
                    cout << objCode << endl;

                    if (opInstruction == "CLEAR2") {
                        lis << setw(9) << setfill(' ') << left << "X";
                        sic << setw(9) << setfill(' ') << left << "X" << endl;
                    }

                    address += 2;
                    position += 6;
                    lis << objCode << endl;
                }
                else if (opFormat == 3) {

                    lis << setw(4) << setfill('0') << hex << right << address << " ";
                    lis << setw(8) << setfill(' ') << left << match;
                    sic << setw(8) << setfill(' ') << match;
                    if (opcode == 4) {
                        lis << " " << setw(8) << setfill(' ') << left << "CLEAR";
                        lis << setw(9) << setfill(' ') << left << "X";
                        objCode = current.substr(position, 6);
                        lis << objCode << endl;
                    }
                    sic << " " << setw(8) << setfill(' ') << left << opInstruction;

                    if (immediate) {
                        sic << setw(9) << setfill(' ') << left << "#0" << endl;
                    }
                    address += 3;
                    position += 6;

                    break;

                }
                break;
            }


        }

        //if modification record
        if (line[0] == 'M') {
            /*
            check for the start of the modification address, along with the number of bits to be modified
            */
            cout << "Modification record found, modify records." << endl;
        }
        //end record found
        if (line[0] == 'E') {
            //exit/finish reading
            cout << "End of Record reached." << endl;
        }
    }
}

/* 
this function will create files for .sic and .lis using the files passed in
*/
void createOutFiles(char* argv[], ofstream& sic, ofstream& lis) {
    
    char buffer[20];
    strcpy(buffer, argv[1]);
    strcat(buffer, ".sic");
    sic.open(buffer);

    strcpy(buffer, argv[1]);
    strcat(buffer, ".lis.txt");
    lis.open(buffer);
}