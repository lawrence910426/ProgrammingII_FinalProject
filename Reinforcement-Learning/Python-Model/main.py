from RL_Brain import RLBrain
import tensorflow as tf


def main():
    sess = tf.compat.v1.Session()
    brain = RLBrain(sess, 1)
    brain.save()


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    main()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
