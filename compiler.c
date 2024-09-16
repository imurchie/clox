#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "value.h"

typedef struct {
  Token current;
  Token previous;
  bool hadError;
  bool panicMode;
} Parser;

typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT,  //  =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // . ()
  PREC_PRIMARY
} Precendence;

Parser parser;
Chunk* compilingChunk;

static void advance();
static void consume(TokenType type, const char* message);

static void emitByte(uint8_t byte);
static void emitBytes(uint8_t byte1, uint8_t byte2);
static void emitConstant(Value value);

static void expression();
static void grouping();
static void number();
static void unary();

static void parsePrecendence(Precendence precedence);

static void errorAtCurrent(const char* message);
static void error(const char* message);
static void errorAt(Token* token, const char* message);

static Chunk* currentChunk();
static void endCompiler();
static uint8_t makeConstant(Value value);

bool compile(const char* source, Chunk* chunk) {
  initScanner(source);
  compilingChunk = chunk;

  parser.hadError = false;
  parser.panicMode = false;

  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expression.");
  endCompiler();
  return !parser.hadError;
}

static void advance() {
  parser.previous = parser.current;

  for (;;) {
    parser.current = scanToken();
    if (parser.current.type != TOKEN_ERROR) break;

    errorAtCurrent(parser.current.start);
  }
}

static void consume(TokenType type, const char* message) {
  if (parser.current.type == type) {
    advance();
    return;
  }

  errorAtCurrent(message);
}

static void emitByte(uint8_t byte) {
  writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
  emitByte(byte1);
  emitByte(byte2);
}

static Chunk* currentChunk() {
  return compilingChunk;
}

static void expression() {
  parsePrecendence(PREC_ASSIGNMENT);
}

static void errorAtCurrent(const char* message) {
  errorAt(&parser.current, message);
}

static void error(const char* message) {
  errorAt(&parser.previous, message);
}

static void errorAt(Token* token, const char* message) {
  if (parser.panicMode) return;
  parser.panicMode = true;

  fprintf(stderr, "[line %d] Error", token->line);

  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type != TOKEN_ERROR) {
    fprintf(stderr, " at '%.*s'", token->length, token->start);
  }

  fprintf(stderr, ": %s\n", message);
  parser.hadError = true;
}

static void emitReturn() {
  emitByte(OP_RETURN);
}

static void endCompiler() {
  emitReturn();
}

static void number() {
  double value = strtod(parser.previous.start, NULL);
  emitConstant(value);
}

static void grouping() {
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression");
}

static void unary() {
  TokenType operatorType = parser.previous.type;

  // compile the operand
  parsePrecendence(PREC_UNARY);

  // emit the operator instruction
  switch (operatorType) {
    case TOKEN_MINUS: emitByte(OP_NEGATE); break;
    default: return;  // unreachable
  }
}

static void parsePrecendence(Precendence precedence) {
  // what goes here?
}

static void emitConstant(Value value) {
  emitBytes(OP_CONSTANT, makeConstant(value));
}

static uint8_t makeConstant(Value value) {
  int constant = addConstant(currentChunk(), value);
  if (constant > UINT8_MAX) {
    error("Too many constants in one chunk");
    return 0;
  }

  return (uint8_t)constant;
}