# wordle-bot
bot that plays wordle. instructions for use:
1. download list of words (e.g. from [here](https://boardgames.stackexchange.com/questions/38366/latest-collins-scrabble-words-list-in-text-file)) and possibly run clean.py to keep only words of length 5
2. run basic_guess.py or minimax_guess.py (the bot will only guess words from the provided list)

it's pretty straightforward to build a program that lets you play wordle, but in the spirit of the game i will not include one here. go make one yourself if you so desire :)

### methods

currently, the bot assumes adversarial play from the computer, and does a depth 2 minimax search with some pruning to determine optimal moves. sadly, it takes 5 turns to beat [absurdle](https://qntm.org/wordle).
