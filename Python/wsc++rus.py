### wsc++rus.py

#   бери .txt'шку отсюда:
#   https://github.com/Poliklot/russian-words/raw/refs/heads/master/russian.txt

### 


import random
from typing import Set, Optional


def load_words() -> Optional[Set[str]]:
    try:
        with open('russian.txt', 'r', encoding='utf-8') as word_file:
            return set(word_file.read().split())
    except FileNotFoundError:
        print("не нашёл russian.txt, скачивай отсюда:\n"
              "https://github.com/Poliklot/russian-words/raw/refs/heads/master/russian.txt")
        return None

def separate_punctuation(word: str) -> (str, str):
    prefix, suffix = '', ''
    
    while word and not word[0].isalpha():
        prefix += word[0]
        word = word[1:]
    while word and not word[-1].isalpha():
        suffix = word[-1] + suffix
        word = word[:-1]

    return prefix, suffix, word

def process_word(word: str, english_words: Set[str], index: int) -> str:
    prefix, suffix, clean_word = separate_punctuation(word)
    if not clean_word:
        return prefix + suffix

    new_word = [
        random.choice([w for w in english_words if len(w) > index and w[index] == letter]) 
        if letter.isalpha() else letter 
        for letter in clean_word
    ]

    return prefix + ' '.join(new_word) + suffix


if __name__ == '__main__':
    english_words = load_words()
    if english_words is None:
        exit(1)

    while True:
        sentence = str(input("ввод: ")).lower()
        try:
            index = max(int(input("индекс: ")) - 1, 0)
        except ValueError:
            print("неверный индекс")
            continue

        new_sentence = [process_word(word, english_words, index) for word in sentence.split()]
        print(' '.join(new_sentence))
