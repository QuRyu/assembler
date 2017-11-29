//
// Created by Quentin Liu on 11/26/17.
//

#ifndef PROJECT8_PARSER_HPP
#define PROJECT8_PARSER_HPP

#include <istream>
#include <ostream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <sstream>

namespace parser {
    // list of functions to parse commands
    // throws `domain_error` if anything could not be parsed correctly
    void op_LOAD(std::istream &, std::ostream &, int n);
    void op_LOADA(std::istream &, std::ostream &, int n);
    void op_STORE(std::istream &, std::ostream &, int n);
    void op_STOREA(std::istream &, std::ostream &, int n);
    void op_BRA(std::istream &, std::ostream &, int n);
    void op_BRAZ(std::istream &, std::ostream &, int n);
    void op_BRAN(std::istream &, std::ostream &, int n);
    void op_BRAO(std::istream &, std::ostream &, int n);
    void op_BRAC(std::istream &, std::ostream &, int n);
    void op_CALL(std::istream &, std::ostream &, int n);
    void op_RETURN(std::istream &, std::ostream &, int n);
    void op_HALT(std::istream &, std::ostream &, int n);
    void op_PUSH(std::istream &, std::ostream &, int n);
    void op_POP(std::istream &, std::ostream &, int n);
    void op_OPORT(std::istream &, std::ostream &, int n);
    void op_IPORT(std::istream &, std::ostream &, int n);
    void op_ADD(std::istream &, std::ostream &, int n);
    void op_SUB(std::istream &, std::ostream &, int n);
    void op_AND(std::istream &, std::ostream &, int n);
    void op_OR(std::istream &, std::ostream &, int n);
    void op_XOR(std::istream &, std::ostream &, int n);
    void op_SHIFTL(std::istream &, std::ostream &, int n);
    void op_SHIFTR(std::istream &, std::ostream &, int n);
    void op_ROTL(std::istream &, std::ostream &, int n);
    void op_ROTR(std::istream &, std::ostream &, int n);
    void op_MOVE(std::istream &, std::ostream &, int n);
    void op_MOVEI(std::istream &, std::ostream &, int n);

    void generateLabelTbl();
    std::string processing();

    // helper functions
    std::string convertToBinary(int n, int line, bool sign);
    std::string convertToHex(int n);
    bool isOperandValid(std::string &, std::map<std::string, std::string> &);
    bool isLabel(const std::string &s);
    std::string getLabel(const std::string &line);
    void checkLabel(const std::string &line, int n); // throws exception if the label is illformed


}


extern std::vector<std::string> lines;
extern std::map<std::string, std::string> labelTbl;
extern std::vector<std::string> operations;
extern std::map<std::string, std::string> tableB;
extern std::map<std::string, std::string> tableC;
extern std::map<std::string, std::string> tableD;
extern std::map<std::string, std::string> tableE;
extern std::map<std::string, std::function<void(std::istream &, std::ostream &, int)>> op_encoding;


#endif //PROJECT8_PARSER_HPP
