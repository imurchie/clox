#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
  Chunk chunk;
  initChunk(&chunk);
  writeChunk(&chunk, OP_RETURN);

  // for debugging purposes
  disassembleChunk(&chunk, "test chunk");

  freeChunk(&chunk);

  return 0;
}
