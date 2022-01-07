import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--word_file", help="path to file of all words", required=True)
parser.add_argument("--debug", help="flag to run in debug mode", action="store_true")
args = parser.parse_args()

word_file = args.word_file
debug = args.debug

f = open(word_file, 'r')
words = [s.strip('\n').strip() for s in f.readlines()]
f.close()

guessed = []

def occ(string, char):
    total = 0
    for c in string:
        if c == char:
            total += 1
    return total

def compute_weights():
    weights = [0] * 26
    for word in words:
        for i in range(len(word)):
            if word[i] not in guessed:
                weights[ord(word[i]) - ord('A')] += 1/(1+occ(word[:i], word[i]))
    for i in range(26):
        weights[i] = int(weights[i]/len(words) * 1000) 
    if debug:
        print(weights)
    return weights

def get_highest_weight_word(weights):
    best_weight = -1
    best_word = ""
    print_all = debug and len(words) < 11
    for word in words:
        current_weight = 0
        for i in range(len(word)):
            if occ(word[:i], word[i]) == 0:
                current_weight += weights[ord(word[i]) - ord('A')]
        if print_all:
            print(word, current_weight)
        if current_weight > best_weight:
            best_weight = current_weight
            best_word = word
    return best_word, best_weight
    
def process(guess, res):
    for char in guess:
        if char not in guessed:
            guessed.append(char)
    for word in words[:]:
        valid = True
        reason = 0
        for i in range(len(guess)):
            if res[i] == 'c':
                if word[i] != guess[i]:
                    valid = False
                    reason = 1
            if res[i] == 'e':
                if occ(word, guess[i]) == 0 or word[i] == guess[i]:
                    valid = False
                    reason = 2
            if res[i] == 'n':
                if occ(word, guess[i]) >= occ(guess, guess[i]):
                    valid = False
                    reason = 3
        if not valid:
            words.remove(word)

guesses = 0

while True:
    guesses += 1
    best_word, best_weight = get_highest_weight_word(compute_weights())
    print(f'Guess {guesses}: {best_word} (weight {best_weight})')
    while True:
        res = input('Result? ')
        if len(res) == len(words[0]):
            break
    if res == "ccccc":
        exit()
    process(best_word, res)
