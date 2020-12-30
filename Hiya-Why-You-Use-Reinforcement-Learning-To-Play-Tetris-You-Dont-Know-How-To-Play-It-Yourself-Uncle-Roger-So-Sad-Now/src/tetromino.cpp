#include "tetromino.h"

#include <cstring>

Tetromino::Tetromino(int type) {
    memcpy(block, block_types[type], sizeof(block));
    block_size = block_sizes[type];

    if (type == 6)
        is_I_block = true;
}
