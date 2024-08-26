#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  int constant = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);

  constant = addConstant(&chunk, 3.4);
  writeChunk(&chunk, OP_CONSTANT, 124);
  writeChunk(&chunk, constant, 124);

  writeChunk(&chunk, OP_ADD, 124);

  constant = addConstant(&chunk, 5.6);
  writeChunk(&chunk, OP_CONSTANT, 124);
  writeChunk(&chunk, constant, 124);

  writeChunk(&chunk, OP_DIVIDE, 124);

  writeChunk(&chunk, OP_NEGATE, 124);

  writeChunk(&chunk, OP_RETURN, 126);

  // for debugging purposes
  // disassembleChunk(&chunk, "test chunk");
  interpret(&chunk);

  freeVM();
  freeChunk(&chunk);

  return 0;
}
