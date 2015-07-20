"""Markov chain generator.

Demonstrate use of bigram module.
"""

from __future__ import print_function

from random import choice
from bigram import Bigram
from collections import defaultdict


def make_chains(in_string):
    """Make markov chains from text in in_string."""

    chains = defaultdict(list)
    words = in_string.strip().split()

    for i in range(len(words) - 2):
        chains[Bigram(words[i], words[i + 1])].append(words[i + 2])

    # Add marker for end-of-text
    chains[Bigram(words[i + 1], words[i + 2])].append(None)   

    return chains
        

def make_text(chains):
    """Generate markov-chain-generated text from chains."""

    bigram = choice(list(chains))
    print (bigram, end=' ')

    while True:
        follows = choice(chains[bigram])
        if follows is None:
            break
        print(follows, end=' ')
        bigram = Bigram(bigram.word2, follows)

    print()


if __name__ == "__main__":
    chains = make_chains("say hello there joel i can see that you say" +
                         "hello there cynthia joel i say hello there besha")
    make_text(chains)
