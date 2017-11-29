// Created by Quentin Liu on 11/26/17.
//

#include "parser.hpp"

#include <regex>

using namespace std;

string trim_str(const string &line, 
        const string &whitespace = "\n\r ") {

    auto str_begin = line.find_first_not_of(whitespace);
    
    if (str_begin == string::npos) 
        return "";

    auto str_end = line.find_last_not_of(whitespace);
    auto str_len = str_end - str_begin + 1;

    return line.substr(str_begin, str_len);
}


namespace parser {

    bool isOperandValid(string &s, map<string, string> &tbl) {
        return tbl.find(s) != tbl.end();
    }


    void op_LOAD(istream &in, ostream &out, int n) {
        string dst;
        int src;
        in >> dst >> src;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (in.fail())
            throw domain_error("invalid memory address at line " + to_string(n));

        out << "00000" << tableB[dst] << convertToBinary(src, n, false) << endl;
    }

    void op_LOADA(istream &in, ostream &out, int n) {
        string dst;
        int src;
        in >> dst >> src;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (in.fail()) 
            throw domain_error("invalid memory address at line " + to_string(n));

        out << "00001" << tableB[dst] << convertToBinary(src, n, false) << endl;
    }

    void op_STORE(istream &in, ostream &out, int n) {
        int dst;
        string src;
        in >> src >> dst;

        if (!isOperandValid(src, tableB))
            throw domain_error("invalid instruction operand \"" + src + "\" at line " + to_string(n));
        else if (in.fail())
            throw domain_error("invalid memory address at line " + to_string(n));

        out << "00010" << tableB[src] << convertToBinary(dst, n, false) << endl;
    }

    void op_STOREA(istream &in, ostream &out, int n) {
        int dst;
        string src;
        in >> src >> dst;

        if (!isOperandValid(src, tableB))
            throw domain_error("invalid instruction operand \"" + src + "\" at line " + to_string(n));
        else if (in.fail())
            throw domain_error("invalid memory address at line " + to_string(n));

        out << "00011" << tableB[src] << convertToBinary(dst, n, false) << endl;
    }

    void op_BRA(istream &in, ostream &out, int n) {
        string label;
        in >> label;

        if (labelTbl.find(label) == labelTbl.end())
            throw domain_error("label \"" + label + "\" is not present at line" + to_string(n));

        out << "00100000" << labelTbl[label] << endl;
    }

    void op_BRAZ(istream &in, ostream &out, int n) {
        string label;
        in >> label;

        if (labelTbl.find(label) == labelTbl.end())
            throw domain_error("label \"" + label + "\" is not present at line" + to_string(n));

        out << "00110000" << labelTbl[label] << endl;
    }

    void op_BRAN(istream &in, ostream &out, int n) {
        string label;
        in >> label;

        if (labelTbl.find(label) == labelTbl.end())
            throw domain_error("label \"" + label + "\" is not present at line" + to_string(n));

        out << "00110010" << labelTbl[label] << endl;
    }

    void op_BRAO(istream &in, ostream &out, int n) {
        string label;
        in >> label;

        if (labelTbl.find(label) == labelTbl.end())
            throw domain_error("label \"" + label + "\" is not present at line" + to_string(n));

        out << "00110001" << labelTbl[label] << endl;
    }

    void op_BRAC(istream &in, ostream &out, int n) {
        string label;
        in >> label;

        if (labelTbl.find(label) == labelTbl.end())
            throw domain_error("label \"" + label + "\" is not present at line" + to_string(n));

        out << "00110011" << labelTbl[label] << endl;
    }

    void op_CALL(istream &in, ostream &out, int n) {
        string label;
        in >> label;

        if (labelTbl.find(label) == labelTbl.end())
            throw domain_error("label \"" + label + "\" is not present at line" + to_string(n));

        out << "00110100" << labelTbl[label] << endl;
    }

    void op_RETURN(istream &in, ostream &out, int n) {
        out << "0011100000000000" << endl;
    }

    void op_HALT(istream &in, ostream &out, int n) {
        out << "0011110000000000" << endl;
    }

    void op_PUSH(istream &in, ostream &out, int n) {
        string src;
        in >> src;

        if (!isOperandValid(src, tableC))
            throw domain_error("invalid instruction operand \"" + src + "\" at line " + to_string(n));

        out << "0100" << tableC[src] << "000000000" << endl;
    }

    void op_POP(istream &in, ostream &out, int n) {
        string dst;
        in >> dst;

        if (!isOperandValid(dst, tableC))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));

        out << "0101" << tableC[dst] << "000000000" << endl;
    }

    void op_OPORT(istream &in, ostream &out, int n) {
        string src;
        in >> src;

        if (!isOperandValid(src, tableD))
            throw domain_error("invalid instruction operand \"" + src + "\" at line " + to_string(n));

        out << "0110" << tableD[src] << "000000000" << endl;
    }

    void op_IPORT(istream &in, ostream &out, int n) {
        string dst;
        in >> dst;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));

        out << "0111" << tableB[dst] << "000000000" << endl;
    }

    void op_ADD(istream &in, ostream &out, int n) {
        string srcA, srcB, dst;
        in >> srcA >> srcB >> dst;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (!isOperandValid(srcA, tableE))
            throw domain_error("invalid instruction operand \"" + srcA + "\" at line " + to_string(n));
        else if (!isOperandValid(srcB, tableE))
            throw domain_error("invalid instruction operand \"" + srcB + "\" at line " + to_string(n));

        out << "1000" << tableE[srcA] << tableE[srcB] << "000" << tableB[dst] << endl;
    }

    void op_SUB(istream &in, ostream &out, int n) {
        string srcA, srcB, dst;
        in >> srcA >> srcB >> dst;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (!isOperandValid(srcA, tableE))
            throw domain_error("invalid instruction operand \"" + srcA + "\" at line " + to_string(n));
        else if (!isOperandValid(srcB, tableE))
            throw domain_error("invalid instruction operand \"" + srcB + "\" at line " + to_string(n));

        out << "1001" << tableE[srcA] << tableE[srcB] << "000" << tableB[dst] << endl;
    }

    void op_AND(istream &in, ostream &out, int n) {
        string srcA, srcB, dst;
        in >> srcA >> srcB >> dst;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (!isOperandValid(srcA, tableE))
            throw domain_error("invalid instruction operand \"" + srcA + "\" at line " + to_string(n));
        else if (!isOperandValid(srcB, tableE))
            throw domain_error("invalid instruction operand \"" + srcB + "\" at line " + to_string(n));

        out << "1010" << tableE[srcA] << tableE[srcB] << "000" << tableB[dst] << endl;
    }

    void op_OR(istream &in, ostream &out, int n) {
        string srcA, srcB, dst;
        in >> srcA >> srcB >> dst;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (!isOperandValid(srcA, tableE))
            throw domain_error("invalid instruction operand \"" + srcA + "\" at line " + to_string(n));
        else if (!isOperandValid(srcB, tableE))
            throw domain_error("invalid instruction operand \"" + srcB + "\" at line " + to_string(n));

        out << "1011" << tableE[srcA] << tableE[srcB] << "000" << tableB[dst] << endl;
    }

    void op_XOR(istream &in, ostream &out, int n) {
        string srcA, srcB, dst;
        in >> srcA >> srcB >> dst;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (!isOperandValid(srcA, tableE))
            throw domain_error("invalid instruction operand \"" + srcA + "\" at line " + to_string(n));
        else if (!isOperandValid(srcB, tableE))
            throw domain_error("invalid instruction operand \"" + srcB + "\" at line " + to_string(n));

        out << "1100" << tableE[srcA] << tableE[srcB] << "000" << tableB[dst] << endl;
    }

    void op_SHIFTL(istream &in, ostream &out, int n) {
        string src, dst;
        in >> src >> dst;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (!isOperandValid(src, tableE))
            throw domain_error("invalid instruction operand \"" + src + "\" at line " + to_string(n));

        out << "11010" << tableE[src] << "00000" << tableB[dst] << endl;
    }

    void op_SHIFTR(istream &in, ostream &out, int n) {
        string src, dst;
        in >> src >> dst;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (!isOperandValid(src, tableE))
            throw domain_error("invalid instruction operand \"" + src + "\" at line " + to_string(n));

        out << "11011" << tableE[src] << "00000" << tableB[dst] << endl;
    }

    void op_ROTL(istream &in, ostream &out, int n) {
        string src, dst;
        in >> src >> dst;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (!isOperandValid(src, tableE))
            throw domain_error("invalid instruction operand \"" + src + "\" at line " + to_string(n));

        out << "11100" << tableE[src] << "00000" << tableB[dst] << endl;
    }

    void op_ROTR(istream &in, ostream &out, int n) {
        string src, dst;
        in >> src >> dst;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (!isOperandValid(src, tableE))
            throw domain_error("invalid instruction operand \"" + src + "\" at line " + to_string(n));

        out << "11101" << tableE[src] << "00000" << tableB[dst] << endl;
    }

    void op_MOVE(istream &in, ostream &out, int n) {
        string src, dst;
        in >> src >> dst;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (!isOperandValid(src, tableE))
            throw domain_error("invalid instruction operand \"" + src + "\" at line " + to_string(n));

        out << "11110" << tableE[src] << "00000" << tableB[dst] << endl;
    }

    void op_MOVEI(istream &in, ostream &out, int n) {
        string dst;
        int src;
        in >> src >> dst;

        if (!isOperandValid(dst, tableB))
            throw domain_error("invalid instruction operand \"" + dst + "\" at line " + to_string(n));
        else if (in.fail())
            throw domain_error("invalid instruction operand \"" + to_string(src) + "\" at line " + to_string(n));

        out << "11111" << convertToBinary(src, n, true) << tableB[dst] << endl;
    }

    void generateLabelTbl() {
        int n = 0; // line count
        for (auto move=lines.cbegin(); move!=lines.cend(); move++) {
            try {
                if (isLabel(*move)) {
                    checkLabel(*move, n);
                    labelTbl[getLabel(*move)] = convertToBinary(n, n, false);
                }
                else 
                    n++;
            }
            catch (runtime_error &e) {
                throw runtime_error(string(e.what()) + " at line " + to_string(n));
            }
        }
    }

    string processing() {
        ostringstream out;
        string temp;
        int n = 1;

        for (auto move=lines.cbegin(); move!=lines.cend(); move++) {
            if (isLabel(*move)) {
                n++;
                continue;
            }

            istringstream in(*move);
            in >> temp;
            if (!binary_search(begin(operations), end(operations), temp))
                throw domain_error("malformed instruction \"" + *move + "\" at line " + to_string(n));
            else
                op_encoding[temp](in, out, n);
            n++;
        }

        return out.str();
    }

    string convertToBinary(int n, int line, bool sign) {
        bool neg = false;
        int bin[8];
        int index = 0;
        ostringstream out;

        // check the value of n
        if (sign && (n > 127 || n < -128))
            throw domain_error("invalid range [-128, 127] " + to_string(n) + " at line " + to_string(line));
        else if (!sign && (n < 0 || n > 255))
            throw domain_error("invaid range [0, 255] " + to_string(n) + " at line " + to_string(line));

        for (int i = 0; i < 8; i++)
            bin[i] = 0;

        if (n < 0) {
            n = -n;
            neg = true;
        }

        while (n) {
            bin[index++] = n % 2;
            n = n / 2;
        }

        
        if (neg) {
            for (int i=0; i<8; i++) // flip bits 
                if (bin[i])
                    bin[i] = 0;
                else 
                    bin[i] = 1;

            for (int i=0; i<8; i++) // + 1 in binary form
                if (!bin[i]) {
                    bin[i] = 1;
                    break;
                } else 
                    bin[i] = 0;
        }

        for (int i = 7; i >= 0; i--)
            out << bin[i];

        return out.str();
    }

    string convertToHex(int n) {
        char bin[2];
        int index = 0;

        if (n > 255 || n < 0) 
            throw domain_error(to_string(n) + " is out of range");

        bin[0] = bin[1] = '0';

        while (n) {
            int temp = n % 16;
            if (temp >= 0 && temp <= 9)
                bin[index++] = temp + '0';
            else 
                bin[index++] = temp + 'A' - 10;
            n = n / 16;
        }

        string s;
        s.push_back(bin[1]);
        s.push_back(bin[0]);
        return s;
    }

    bool isLabel(const string &s) {
        static const regex regex_exp("(\\S+:)");

        if (regex_search(s, regex_exp)) return true;
        else return false;
    }

    void checkLabel(const string &line, int n) {
        string line_trimed = trim_str(line);

        if (line_trimed[line_trimed.size()-1] != ':')
            throw domain_error("malformed label " + line + " at line " + to_string(n));
    }

    string getLabel(const string &line) {
        const static regex regex_exp("(\\S+):");
    
        auto words_iter = sregex_iterator(line.begin(), line.end(), regex_exp);
    
        return (*words_iter)[1].str();
    }
}

std::map<std::string, std::string> labelTbl;
std::vector<std::string> lines;

std::vector<std::string> operations = {"LOAD", "LOADA", "STORE", "STOREA", "BRA", "BRAZ", "BRAN", "BRAO",
                                       "BRAC", "CALL", "RETURN", "HALT", "PUSH", "POP", "OPORT",
                                       "IPORT", "ADD", "SUB", "AND", "OR", "XOR", "SHIFTL",
                                       "SHIFTR", "ROTL", "ROTR", "MOVE", "MOVEI"};

std::map<std::string, std::string> tableB = {{"RA", "000"},
                                             {"RB", "001"},
                                             {"RC", "010"},
                                             {"RD", "011"},
                                             {"RE", "100"},
                                             {"SP", "101"}};

std::map<std::string, std::string> tableC = {{"RA", "000"},
                                             {"RB", "001"},
                                             {"RC", "010"},
                                             {"RD", "011"},
                                             {"RE", "100"},
                                             {"SP", "101"},
                                             {"PC", "110"},
                                             {"CR", "111"}};;

std::map<std::string, std::string> tableD = {{"RA", "000"},
                                             {"RB", "001"},
                                             {"RC", "010"},
                                             {"RD", "011"},
                                             {"RE", "100"},
                                             {"SP", "101"},
                                             {"PC", "110"},
                                             {"IR", "111"}};;

std::map<std::string, std::string> tableE = {{"RA",    "000"},
                                             {"RB",    "001"},
                                             {"RC",    "010"},
                                             {"RD",    "011"},
                                             {"RE",    "100"},
                                             {"SP",    "101"},
                                             {"Zeros", "110"},
                                             {"Ones",  "111"}};;


using namespace parser;

std::map<std::string, std::function<void(std::istream &, std::ostream &, int)>> op_encoding =
        {{"LOAD",   op_LOAD},
         {"LOADA",  op_LOADA},
         {"STORE",  op_STORE},
         {"STOREA", op_STOREA},
         {"BRA",    op_BRA},
         {"BRAZ",   op_BRAZ},
         {"BRAN",   op_BRAN},
         {"BRAO",   op_BRAO},
         {"BRAC",   op_BRAC},
         {"CALL",   op_CALL},
         {"RETURN", op_RETURN},
         {"HALT",   op_HALT},
         {"PUSH",   op_PUSH},
         {"POP",    op_POP},
         {"OPORT",  op_OPORT},
         {"IPORT",  op_IPORT},
         {"ADD",    op_ADD},
         {"SUB",    op_SUB},
         {"AND",    op_AND},
         {"OR",     op_OR},
         {"XOR",    op_XOR},
         {"SHIFTL", op_SHIFTL},
         {"SHIFTR", op_SHIFTR},
         {"ROTL",   op_ROTL},
         {"ROTR",   op_ROTR},
         {"MOVE",   op_MOVE},
         {"MOVEI",  op_MOVEI}};
