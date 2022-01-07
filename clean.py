import argparse

parser = argparse.ArgumentParser(description="Deletes all lines from a file, except those of a specific length")
parser.add_argument("--in_path", help="path to input file", required=True)
parser.add_argument("--out_path", help="path to output file", required=True)
parser.add_argument("--word_length", help="length of lines to keep (default: 5)", default=5)
args = parser.parse_args()

in_path = args.in_path
out_path = args.out_path
word_length = args.word_length

f = open(in_path, 'r')

allwords = []

for line in f.readlines():
    current = line.strip('\n').strip()
    if len(current) == word_length:
        allwords.append(current)

f.close()

g = open(out_path, 'w')

for w in allwords:
    g.write(w + '\n')

g.close()
