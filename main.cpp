#include <iostream>
#include <fstream>
#include <vector>
#include "lexer.h"
#include "evaluator.h"
#include "main.h"

using namespace std;

int main(int argc, char* argv[]) {

    //std::__cxx11::basic_string<char> mystring;

    if(argc != 2) {
        cout << "Usage: \n" << argv[1] << " <input file>\n";
    }

    ifstream is(argv[1]);
    vector<char>  src_chars((istreambuf_iterator<char>(is)), istreambuf_iterator<char>());
    vector<lexeme> token_list;

    try {
    lex(src_chars, token_list);
    }
    catch(LexException& le) {
        le.print();
        throw;
    }
    
    //print_lexeme_list(token_list, src_chars);

    try {
        evaluate(token_list, src_chars);
    }
    catch(EvalException& exc) {
        // print all relevant information
        for(auto p : exc.ad->constant_lookup) {
            cout << p.first << " -> " << p.second.to_string(src_chars) << endl;
        }

        cout << "\n\n";

        for(char c : exc.ad->data_vector) {
            if(c == 0)
                cout << '%';
            else
                cout << c;
        }
        cout << endl << flush;

        throw std::runtime_error("exception caught in main");

    }

    return 0;
}
