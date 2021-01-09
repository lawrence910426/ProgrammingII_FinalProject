"""
-- nn INPUT --

one hot -> [ 13 (cards) * 4 (colors) + 2(black card) ] * 2 (is multi)
numbers -> 5 (color statistics of hand card)
numbers -> 4 (colors) * 2 (is functional) + 2 (black cards)

one hot -> prev_steps * [ 4 (colors) * 2(is chosen) + 1(None) ]

number -> opponent cards
number -> accumulate penalty

one hot -> prev_steps * 1(is functional card)

numbers -> 1 (functional cards sum)


-- nn OUTPUT

15 (cards) * 4 (colors)

"""
import numpy as np


class NNTranslate:
    prev_steps = 8
    # features = (13 * 4 + 2) * 2 + 5 + (4 * 2 + 2) + prev_steps * 9 + 1 + 1 + prev_steps + 1
    actions = 15 * 4 + 1
    features = 100

    @staticmethod
    def state_to_nn(state):
        """
        [self.used, len(self.unused), self.hand_card[player],
        len(self.hand_card[1 - player]), self.accumulate_penalty]
        """
        ans = np.zeros(NNTranslate.features)
        return ans

    @staticmethod
    def nn_to_state(value):
        return None

    @staticmethod
    def card_to_nn(card):
        return 15 * 4 if card is None else (card.color - 1) * 15 + card.value

    @staticmethod
    def get_available_mask(available_cards):
        ans = np.zeros(NNTranslate.actions)
        ans[15 * 4] = 1
        for card in available_cards:
            if card.color == 0:
                for i in range(4):
                    ans[i * 15 + card.value] = 1
            else:
                ans[(card.color - 1) * 15 + card.value] = 1
        return ans
