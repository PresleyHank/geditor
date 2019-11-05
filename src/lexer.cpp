//
// Created by Alex on 2019/10/7.
//

#include "lexer.h"
#include "document.h"
bool IsSpace(GChar ch){
    return ch == _HM_C(' ') || ch == _HM_C('\r');
}
bool IsNumber(GChar ch) {
    return ch >= _HM_C('0') && ch <= _HM_C('9');
}
bool IsAlpha(GChar ch) {
    return (ch >= _HM_C('a') && ch <= _HM_C('z')) || (ch >= _HM_C('A') && ch <= _HM_C('Z'));
}
bool IsCodeChar(GChar ch) {
    return IsAlpha(ch) || ch == _HM_C('_') || (ch > _GT('\u4E00') && ch < _GT('\u9FA5'));
}

void Lexer::enter(EventContext *ctx, int column) {
    context = ctx;
    viewer = context->getLineViewer(column);
    string = viewer.c_str();
    length = viewer.length();
    position = 0;
}

bool Lexer::has() {
    return position < length;
}

void Lexer::ParseSpace() {
    TOKEN_START();
    do {
        NEXT();
    } while (IsSpace(CURRENT_CHAR));
    TOKEN_END(TokenSpace, StyleDeafaultFont);
}

void Lexer::ParseIdentifier() {
    TOKEN_START();
    do {
        NEXT();
    } while (IsCodeChar(CURRENT_CHAR) || IsNumber(CURRENT_CHAR));
    TOKEN_END(TokenIdentifier, StyleDeafaultFont);
    GString identifier(current.start, current.length);
    current.style = keywords.count(identifier) ? keywords[identifier] : StyleDeafaultFont;
}

void Lexer::ParseString() {
    TOKEN_START();
    GChar first = CURRENT_CHAR;
    NEXT();
    while (CURRENT_CHAR != first) {
        NEXT();
        if (!has()) {
            TOKEN_END(TokenString, StyleErrorFont);
            return;
        }
    }
    NEXT();
    TOKEN_END(TokenString, StyleStringFont);
}

void Lexer::ParseNumber() {
    TOKEN_START();
    do {
        NEXT();
    } while (IsNumber(CURRENT_CHAR) || CURRENT_CHAR == _HM_C('.'));
    TOKEN_END(TokenNumber, StyleNumberFont);
}

Token Lexer::next() {
    switch (CURRENT_CHAR) {
        case _HM_C(';'):case _HM_C('('):case _HM_C(')'):case _HM_C('['):
        case _HM_C(']'):case _HM_C('{'):case _HM_C('}'):case _HM_C('.'):
        case _HM_C('@'):case _HM_C(':'):case _HM_C('!'):case _HM_C('|'):
        case _HM_C('&'):case _HM_C('+'):case _HM_C('>'):case _HM_C('<'):
        case _HM_C('='):case _HM_C('%'):case _HM_C('-'):case _HM_C('*'):
        case _HM_C(','):
        case _HM_C('/'):
            TOKEN_START();
            NEXT();
            if (NEXT_CHAR == _HM_C('/') || NEXT_CHAR == _HM_C('*')) {
                NEXT();
            }
            TOKEN_END(TokenOperator, StyleOperatorFont);
            break;
        default:
            if (IsCodeChar(CURRENT_CHAR)) {
                ParseIdentifier();
            } else if (IsSpace(CURRENT_CHAR)) {
                ParseSpace();
            } else if (IsNumber(CURRENT_CHAR)) {
                ParseNumber();
            } else if (CURRENT_CHAR == _HM_C('"') || CURRENT_CHAR == _HM_C('\'')) {
                ParseString();
            } else {
                NEXT();
            }
    }
    return current;
}
