#include "../parser.hpp"

#include <iostream>
#include <utility>
#include <iterator>
#include <algorithm>

using namespace std;
using namespace parser;

void op_func_test(string &&input, string &&output, int n, function<void(istream &, ostream &, int)> func);

void op_func_except_test(string &&input, string &&errMsg, int n, function<void(istream &, ostream &, int)> func);

void convertToBinary_test(int n, int line, bool sign, string &&output);
void convertToHex_test(int n, int line, string &&output);

void isLabel_test(string &&input, bool expected);
void getLabel_test(string &&input, string &&expected);
void checkLabel_test(string &&input, int n=0);

void generateLabelTbl_test(vector<string> &lines, map<string, string> &expectedTbl, int n=0);

void processing_test(vector<string> &input, map<string, string> &tbl, string &output, int n=0);

int main() {
    convertToBinary_test(-2, 0, true, "11111110");
    convertToBinary_test(1, 0, false, "00000001");
    convertToBinary_test(13, 0, false, "00001101");
    convertToBinary_test(12, 0, false, "00001100");

    convertToHex_test(13, 0, "0D");
    convertToHex_test(25, 0, "19");

    isLabel_test(" this: ", true);
    isLabel_test(" wait:", true);
    isLabel_test(" is:?", true);
    isLabel_test("  iii", false);
    isLabel_test("\n this: ?\r?", true);
    isLabel_test(" :", false);

    getLabel_test(" this:", "this");
    getLabel_test(" this: ", "this");
    getLabel_test(" \n is: ?", "is");

    checkLabel_test(" tht:?");

    vector<string> lines1 = {"hi: \n", "this", "is", "a", " wait:", "line", "correct:", "but"};
    map<string, string> expectedTbl1 = {{"hi", convertToBinary(0, 0, false)}, {"wait", convertToBinary(3, 0, false)}, {"correct", convertToBinary(4, 0, false)}};
    generateLabelTbl_test(lines1, expectedTbl1);

    vector<string> lines2 = {"this", "is:", "a", " wait:", "line", "correct:", "but"};
    map<string, string> expectedTbl2 = {{"is", convertToBinary(1, 0, false)}, 
        {"wait", convertToBinary(2, 0, false)}, 
        {"correct", convertToBinary(3, 0, false)}};
    generateLabelTbl_test(lines2, expectedTbl2, 1);

    vector<string> lines4 = {"MOVEI 2 RA", "MOVEI 1 RB", "MOVEI 0 RC", "MOVEI -1 RD", 
                             "MOVEI 9 RE", "LOOP:", "ADD RA RB RC", "MOVE RA RB",
                             "MOVE RC RA", "OPORT RA", "ADD RE RD RE", "BRAZ TERMINATE",
                             "BRA LOOP", "TERMINATE:", "HALT"};
    map<string, string> expectedTbl4 = {{"LOOP", "00000101"}, {"TERMINATE", "00001100"}};
    generateLabelTbl_test(lines4, expectedTbl4, 2);

    vector<string> lines3 = {"this:", "ADD RA RA RB", "RETURN", "SUB RB RB RA", "CALL this"};
    map<string, string> tbl3 = {{"this", "00000000"}};
    string output = "1000000000000001\n0011100000000000\n1001001001000000\n0011010000000000\n";
    processing_test(lines3, tbl3, output);


    labelTbl = {{"13", "00001101"}};
    op_func_test(" RA 13 ", "0000000000001101", 1, op_LOAD);
    op_func_test(" RB 13 ", "0000100100001101", 2, op_LOADA);
    op_func_test(" RA 13 ", "0001000000001101", 3, op_STORE);
    op_func_test(" RA 13 ", "0001100000001101", 4, op_STOREA);
    op_func_test(" 13 ", "0010000000001101", 5, op_BRA);
    op_func_test(" 13 ", "0011000000001101", 6, op_BRAZ);
    op_func_test("  13 ", "0011001000001101", 7, op_BRAN);
    op_func_test("  13 ", "0011000100001101", 8, op_BRAO);
    op_func_test("  13 ", "0011001100001101", 9, op_BRAC);
    op_func_test(" 13 ", "0011010000001101", 10, op_CALL);
    op_func_test(" ", "0011100000000000", 11, op_RETURN);
    op_func_test(" ", "0011110000000000", 12, op_HALT);
    op_func_test(" RA ", "0100000000000000", 13, op_PUSH);
    op_func_test(" RA ", "0101000000000000", 14, op_POP);
    op_func_test(" RA  ", "0110000000000000", 15, op_OPORT);
    op_func_test(" RA ", "0111000000000000", 16, op_IPORT);
    op_func_test(" RA RA RB ", "1000000000000001", 17, op_ADD);
    op_func_test(" RA RA RB ", "1001000000000001", 18, op_SUB);
    op_func_test(" RA RA RB ", "1010000000000001", 19, op_AND);
    op_func_test(" RA RA RB ", "1011000000000001", 20, op_OR);
    op_func_test(" RA RA RB ", "1100000000000001", 21, op_XOR);
    op_func_test(" RA RB ", "1101000000000001", 22, op_SHIFTL);
    op_func_test(" RA RB ", "1101100000000001", 23, op_SHIFTR);
    op_func_test(" RA RB ", "1110000000000001", 24, op_ROTL);
    op_func_test(" RA RB ", "1110100000000001", 25, op_ROTR);
    op_func_test(" RA RB ", "1111000000000001", 26, op_MOVE);
    op_func_test(" 2 RB ", "1111100000010001", 27, op_MOVEI);
    op_func_test(" -1 RB ", "1111111111111001", 28, op_MOVEI);
    
}


void op_func_test(string &&input, string &&output, int n, function<void(istream &, ostream &, int)> func) {
    istringstream sin(input);
    ostringstream sout;

    func(sin, sout, n);

    if (sout.str() != (output + "\n"))
        cout << input << " to the function does not produce correct output \"" << sout.str() << "\", which should be " << output << endl;
}


void op_func_except_test(string &&input, string &&errMsg, int n, function<void(istream &, ostream &, int)> func) {
    istringstream sin(input);
    ostringstream sout;

    try {
        func(sin, sout, n);
    }
    catch (domain_error e) {
        if (e.what() != errMsg) 
            cout << input << " to function does not produce correct error message \"" << e.what() << "\", which should be " << errMsg << endl;
    }

}

void convertToBinary_test(int n, int line, bool sign, string &&output) {
    string temp = convertToBinary(n, line, sign);
    if (temp != output) 
        cout << "expected: " << output << ", only got " << temp << endl;
}

void convertToHex_test(int n, int line, string &&output) {
    string temp = convertToHex(n);
    if (temp != output) 
        cout << "expected: " << output << ", only got " << temp << endl;
}

void isLabel_test(string &&input, bool expected) { 
    if (expected != isLabel(input))
            cout << input << " does not produce expected output " << (expected ? "true" : "false") << endl;

}

void getLabel_test(string &&input, string &&expected) {
    if (expected != getLabel(input))
        cout << input << " does not produce correct output but " << getLabel(input) << endl;
}

void checkLabel_test(string &&input, int n) {
    try {
        checkLabel(input, n);
        cout << " for " << input << " no except is thrown" << endl;
    }
    catch (domain_error e) {
        
    }
}

template<typename A, typename B> 
pair<B,A> flip_pair(const pair<A, B> &p) {
    return pair<B,A>(p.second, p.first);
}

template<typename A, typename B>
multimap<A, B> flip_map(const map<A, B> &src) {
    multimap<A, B> dst;
    transform(src.begin(), src.end(), 
              inserter(dst, dst.begin()),
              flip_pair<A,B>);
    return dst;
}

void generateLabelTbl_test(vector<string> &lines, map<string, string> &expectedTbl, int n) {
    ::lines = lines;

    generateLabelTbl();
    
    auto tbl = flip_map(labelTbl);
    auto expected = flip_map(expectedTbl);

    if (tbl != expected) {
        cout << "test case " + to_string(n) + " did not succeed" << endl;
        cout << "the length of generated table is " << to_string(tbl.size()) 
             << ", while that of" << "expected is " << to_string(expectedTbl.size()) << endl;

        for (auto iter=tbl.cbegin(); iter!=tbl.cend(); iter++) 
            cout << iter->first << " " << iter->second << endl;

    }
    
    ::lines.clear();
    labelTbl.clear();
}


void processing_test(vector<string> &input, map<string, string> &tbl, string &expectedOutput, int n) {
    string output;
    if (output !=(output = processing())) {
        cout << "test case " << to_string(n) << " failed" << endl;
        cout << "expected: " << expectedOutput << endl << "got: " << output << endl;
    }

}
