"""
-- nn INPUT --

The status of the board after the tile has dropped down
one hot -> 20 (rows) * 10 (columns) * 4 (directions) * 10 (offset)

An additional tag to determine if the board is invalid
one hot -> 1 (valid or invalid) * 10 (offset) * 4 (directions)


-- nn OUTPUT --

10 (drop down position) * 4 (direction)

"""
import numpy as np


class NNTranslate:
    features = 20 * 10 * 4 * 10 + 1 * 10 * 4
    actions = 10 * 4

    @staticmethod
    def drop_tetro(tetro, offset, board):
        valid, bottom = False, 0
        for dropdown in range(20):
            def allowed():
                for x in range(4):
                    for y in range(4):
                        if tetro[x, y]:
                            if dropdown + x >= 20 or offset + y >= 10 or board[dropdown + x, offset + y]:
                                return False
                return True

            if not allowed():
                break
            valid, bottom = True, dropdown
        return valid, bottom

    @staticmethod
    def state_to_nn(state):
        board, tetromino = state
        ans = np.zeros(NNTranslate.features, dtype=np.double)
        for direction in range(4):
            for offset in range(10):
                tetro = tetromino[direction]
                valid, bottom = NNTranslate.drop_tetro(tetro, offset, board)
                temp = np.array(board, copy=True).astype(np.double)
                i = direction * 10 + offset
                if valid:
                    for x in range(4):
                        for y in range(4):
                            if tetro[x, y]:
                                temp[bottom + x, offset + y] = 1.0
                else:
                    ans[20 * 10 * 4 * 10 + i] = 1.0
                ans[i * 20 * 10:(i + 1) * 20 * 10] = temp.reshape(20 * 10)
        return ans

    @staticmethod
    def nn_to_action(nn):
        return nn

    @staticmethod
    def action_to_nn(action):
        return action

    @staticmethod
    def get_available_mask(state):
        return 1 - NNTranslate.state_to_nn(state)[20 * 10 * 4 * 10:]
