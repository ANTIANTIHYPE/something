### wsc++.py

# get words .txt from
# https://raw.githubusercontent.com/dwyl/english-words/refs/heads/master/words_alpha.txt

### 


import random
import string


def load_words():
    with open('words_alpha.txt') as word_file:
        valid_words = set(word_file.read().split())
    return valid_words

if __name__ == '__main__':
    while True:
        english_words = load_words()
        sentence = str(input("enter: ")).lower()
        index = int(input("index: ")) - 1

        if index < 0:
            index = 0

        new = []

        # i guess this works for numbers too
        for word in sentence.split():
            #separate punctuation
            prefix = ''
            suffix = ''
            if word and not word.isalpha():
                #handle punctuation
                while word and not word[0].isalpha():
                    prefix += word[0]
                    word = word[1:]
                while word and not word[-1].isalpha():
                    suffix = word[-1] + suffix
                    word = word[:-1]

            if not word:
                new.append(prefix + suffix)
                continue

            for letter in word:
                if letter.isalpha():
                    matching = [w for w in english_words if len(w) > index and w[index] == letter]
                    if matching:
                        new.append(random.choice(matching))
                    else:
                        new.append(random.choice(list(english_words)))
                else:
                    new.append(letter)

            #reattach garbagew
            new[-1] = prefix + new[-1] + suffix

        print(' '.join(new))
