#include <iostream>
#include <fstream>
#include <vector>
#include <istream>
#include <string>
#include <stdexcept>
#include <map>
#include <sstream>
#include "parser.hpp"

using namespace std;


void readFile(istream &in);


int main(int argc, char *argv[]) {

    if (argc < 2)
        throw runtime_error("fewer arguments than expected");

    sort(begin(operations), end(operations));
    for (int i=1; i<argc; i++) {
        ifstream fin(argv[i]);
        string output;

        if (!fin.is_open()) {
            cout << "fail to open file: " << argv[i] << endl;
            return 1;
        }
        readFile(fin);
        try {
            parser::generateLabelTbl();
        }
        catch (runtime_error e) {
            cout << "an error happened while processing labels:\n   " << e.what() << endl;
            return 1;
        }

        try {
            output = parser::processing();
        }
        catch (domain_error e) {
            cout << "an error happened while converting instructions into machine code:\n   " << e.what() << endl;
            return 1;
        }

        int n = 0; // line count
        string temp;
        istringstream sin(output);
        ofstream fout(string(argv[i]) + ".mif");

        fout << "DEPTH = 256;" << endl;
        fout << "WIDTH = 16;" << endl;
        fout << "ADDRESS_RADIX = HEX;" << endl;
        fout << "DATA_RADIX = BIN;" << endl;
        fout << "CONTENT" << endl;
        fout << "BEGIN" << endl;

        while (getline(sin, temp))
            fout << parser::convertToHex(n++) << " : " << temp << ";" << endl;
        if (n < 254)
            fout << "[" << parser::convertToHex(n) << ".." << "FF] : 11111111111111111;" << endl;

        fout << "END" << endl;
    }
    return 0;
}

void readFile(istream &in) {
    string temp;
    while (getline(in, temp))
        lines.push_back(temp);
}





