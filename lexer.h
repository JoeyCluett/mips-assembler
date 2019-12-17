#pragma once

#include <iostream>
#include <vector>
#include "instruction_list.h"
#include "lex-data.h"
#include "lex-exception.h"

#define LEX_DEBUG 0

c_iter lex_comment(c_iter cur, std::vector<char>& src, std::vector<lexeme>& tokens) {
    // comments dont get a lexeme entry
    while(*cur != '\n') {
        cur++;
    }
    cur++;
    return cur;
}

c_iter lex_register_ref(c_iter cur, std::vector<char>& src, std::vector<lexeme>& tokens) {

    lexeme l;
    l.type = register_reference;
    l.start = cur - src.begin();

    do {
        cur++; // move past the '$' before testing
    } while(
        (*cur >= 'a' && *cur <= 'z') ||
        (*cur >= 'A' && *cur <= 'Z') ||
        (*cur >= '0' && *cur <= '9'));

    l.end = cur - src.begin();
    tokens.push_back(l);

    return cur;
}

c_iter lex_directive(c_iter cur, std::vector<char>& src, std::vector<lexeme>& tokens) {

    lexeme l;
    l.type = directive;
    l.start = cur - src.begin();

    do {
        cur++; // move past the '$' before testing
    } while(
        (*cur >= 'a' && *cur <= 'z') ||
        (*cur >= 'A' && *cur <= 'Z') ||
        (*cur >= '0' && *cur <= '9') || *cur == '_');

    l.end = cur - src.begin();

    tokens.push_back(l);
    return cur;
}

c_iter lex_punctuation(c_iter cur, std::vector<char>& src, std::vector<lexeme>& tokens) {

    if(*cur == '.') {
        // start of a directive
        return lex_directive(cur, src, tokens);
    }
    else if(*cur == ',') {

        lexeme l;
        l.type = comma;
        l.start = cur - src.begin();
        l.end = l.start + 1;

        tokens.push_back(l);

        cur++;
        return cur;
    }
    else if(*cur == '=') {

        lexeme l;
        l.type = equals;
        l.start = cur - src.begin();
        l.end = l.start + 1;

        tokens.push_back(l);

        cur++;
        return cur;
    }
    else if(*cur == '(') {
        lexeme l;
        l.type = open_paren;
        l.start = cur - src.begin();
        l.end = l.start + 1;

        tokens.push_back(l);

        cur++;
        return cur;
    }
    else if(*cur == ')') {
        lexeme l;
        l.type = close_paren;
        l.start = cur - src.begin();
        l.end = l.start + 1;

        tokens.push_back(l);

        cur++;
        return cur;
    }
    else {
        throw std::runtime_error("unrecognized lexeme: " + std::to_string(*cur));
    }
}

c_iter lex_identifier(c_iter cur, std::vector<char>& src, std::vector<lexeme>& tokens) {
    lexeme l;
    //l.type = instruction;
    //l.type = constant_ident;
    //l.type = jump_label

    l.start = cur - src.begin();

    do {
        cur++; // move past all of the regular ident chars
    } while(
        (*cur >= 'a' && *cur <= 'z') ||
        (*cur >= 'A' && *cur <= 'Z') ||
        (*cur >= '0' && *cur <= '9') || *cur == '_');

    if(*cur == ':') { // label
        l.type = jump_label;
        cur++;
        l.end = cur - src.begin();
    }
    else if(
            instruction_list.find(
                std::string(src.begin()+l.start, cur)) !=
            instruction_list.end()) {
        // instruction identifier
        l.type = instruction;
        l.end = cur - src.begin();
    }
    else {
        // everything else is a symbolic constant
        l.type = symbolic_constant;
        l.end = cur - src.begin();
    }

    tokens.push_back(l);
    return cur;
}

c_iter lex_string(c_iter cur, std::vector<char>& src, std::vector<lexeme>& tokens) {

    cur++;

    lexeme l;
    l.type = string_literal;
    l.start = cur - src.begin();

    while(*cur != '"') {
        cur++;
    }

    l.end = cur - src.begin();
    tokens.push_back(l);
    cur++; // advance past the terminating quote
    return cur;
}

c_iter lex_number(c_iter cur, std::vector<char>& src, std::vector<lexeme>& tokens) {
    lexeme l;
    l.type = number_literal;
    l.start = cur - src.begin();

    do {
        ++cur;
    } while(
        (*cur >= '0' && *cur <= '9') ||
        *cur == '.' || // real nunbers
        *cur == 'e' || // scientific notation
        *cur == 'x'    // hex numbers
    );

    l.end = cur - src.begin();

    tokens.push_back(l);
    return cur;
}

void lex(std::vector<char>& src, std::vector<lexeme>& tokens) {

    auto chariter = src.begin();
    auto enditer = src.end();

    while(chariter != enditer) {

        char c = *chariter;

        if(c == '#') {
            // comment
            #if LEX_DEBUG == 1
            std::cout << "Comment\n" << std::flush;
            #endif

            chariter = lex_comment(chariter, src, tokens);
        }
        else if(c == '$') {
            // register reference
            #if LEX_DEBUG == 1
            std::cout << "Register\n" << std::flush;
            #endif
            
            chariter = lex_register_ref(chariter, src, tokens);
        }
        else if(c == ',' || c == '.' || c == '=' || c == '(' || c == ')') {
            // commas should be by themselves, periods have context-dependent meaning
            #if LEX_DEBUG == 1
            std::cout << ", or . or =\n" << std::flush;
            #endif

            chariter = lex_punctuation(chariter, src, tokens);
        }
        else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
            // context dependent lex_identifier
            // labels end with ':'
            #if LEX_DEBUG == 1
            std::cout << "Identifier\n" << std::flush;
            #endif

            chariter = lex_identifier(chariter, src, tokens);
        }
        else if(c == '"') {
            // string literal
            #if LEX_DEBUG == 1
            std::cout << "String\n" << std::flush;
            #endif
            
            chariter = lex_string(chariter, src, tokens);
        }
        else if((c >= '0' && c <= '9') || c == '-') {
            // this is some kind of number literal
            #if LEX_DEBUG == 1
            std::cout << "Number\n" << std::flush;
            #endif

            chariter = lex_number(chariter, src, tokens);

            if(chariter > enditer) {
                throw std::runtime_error("lexer has exceeded source array boundaries");
            }
        }
        else if(c == ' ' || c == '\n' || c == '\t') {
            // by default skip all whitespace
            chariter++;
        }
        else {
            throw std::runtime_error("unknown token: " + std::to_string(c));
            //throw LexException(chariter, src);
        }
    }
}
