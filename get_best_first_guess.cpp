// g++ -Wall -O2 get_best_first_guess.cpp -o get_best_first_guess.exe 
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

const int WORD_LENGTH = 5;
const int THREE_POW = 243;
const int TWO_POW = 32;
const int WORDS_UB = 15000; // upper bound on number of words
const int ANSWERS_UB = 2500; // upper bound on number of answers
const int MINIMAX_LENIENCY = 50;

short results[WORDS_UB][ANSWERS_UB];
std::vector<int> counts;
std::vector<std::string> words;
std::vector<std::string> answers;
std::vector<int> answer_indices[THREE_POW];

void input(std::string word_file, std::string answer_file) {
	counts.resize(THREE_POW);
	std::ifstream word_input(word_file);
	std::string s;
	while (word_input >> s && int(words.size()) <= WORDS_UB) {
		words.push_back(s);
	}
	word_input.close();
	std::ifstream answer_input(answer_file);
	while (answer_input >> s && int(answers.size()) <= ANSWERS_UB) {
		answers.push_back(s);
	}
	answer_input.close();
	std::cout << "Read input files, found " << int(words.size()) << " words and " << int(answers.size()) << " valid answers." << std::endl;
}

int get_res(int guess_index, int ans_index) {
	std::string guess = words[guess_index];
	std::string ans = answers[ans_index];
	int to_return = 0;
	int guess_frequencies[WORD_LENGTH];
	int ans_frequencies[WORD_LENGTH];
	for (int idx = 0; idx < WORD_LENGTH; idx++) {
		if (guess[idx] == ans[idx]) {
			guess_frequencies[idx] = 0;
			ans_frequencies[idx] = 0;
		}
		else {
			guess_frequencies[idx] = 1;
			ans_frequencies[idx] = 1;
			for (int jdx = idx-1; jdx >= 0; jdx--) {
				if (guess[idx] == guess[jdx] && guess_frequencies[idx] == 1 && guess_frequencies[jdx] >= 1) {
					guess_frequencies[idx] = guess_frequencies[jdx] + 1;
				}
				if (ans[idx] == ans[jdx] && ans_frequencies[idx] == 1 && ans_frequencies[jdx] >= 1) {
					ans_frequencies[idx] = ans_frequencies[jdx] + 1;
				}
			}
		}
	}
	for (int idx = 0; idx < WORD_LENGTH; idx++) {
		int current = 2;
		if (guess[idx] == ans[idx]) {
			current = 0;
		}
		else {
			for (int jdx = 0; jdx < WORD_LENGTH; jdx++) {
				if (guess[idx] == ans[jdx] && guess_frequencies[idx] == ans_frequencies[jdx]) {
					current = 1;
					break;
				}
			}
		}
		to_return = 3 * to_return + current;
	}
	return to_return;
}

void preprocess() {
	for (int idx = 0; idx < int(words.size()); idx++) {
		if (idx % 2000 == 0 && idx > 0) {
			std::cout << "Preprocessed " << idx << " of " << int(words.size()) << " words." << std::endl;
		}
		for (int jdx = 0; jdx < int(answers.size()); jdx++) {
			results[idx][jdx] = get_res(idx, jdx);
		}
	}
}

void work() {
	int num_words = int(words.size());
	int num_answers = int(answers.size());
	counts.resize(THREE_POW);
	int best_minimax = num_words;
	int best_double_minimax = num_words;
	int best_idx = -1;
	for (int idx = 0; idx < num_words; idx++) {
		if (idx % 2000 == 0 && idx > 0) {
			std::cout << "Processed " << idx << " words, ";
			std::cout << "current best word: " << words[best_idx] << " (minimax score: " << best_minimax;
			std::cout << ", double minimax score: " << best_double_minimax << ")" << std::endl;
		}
		for (int ctr = 0; ctr < THREE_POW; ctr++) {
			answer_indices[ctr].clear();
		}
		std::fill(counts.begin(), counts.end(), 0);
		for (int jdx = 0; jdx < num_answers; jdx++) {
			answer_indices[results[idx][jdx]].push_back(jdx);
			counts[results[idx][jdx]]++;
		}
		int minimax_score = *max_element(counts.begin(), counts.end());
		if (minimax_score >= best_minimax + MINIMAX_LENIENCY) {
			continue;
		}
		int double_minimax_score = -1;
		for (int ctr = 0; ctr < THREE_POW; ctr++) {
			int best_option = num_words;
			for (int kdx = 0; kdx < num_words; kdx++) {
				std::fill(counts.begin(), counts.end(), 0);
				for (auto &ldx : answer_indices[ctr]) {
					counts[results[kdx][ldx]]++;
					if (counts[results[kdx][ldx]] >= best_option) {
						break;
					}
				}
				best_option = std::min(best_option, *max_element(counts.begin(), counts.end()));
			}
			double_minimax_score = std::max(double_minimax_score, best_option);
			if (double_minimax_score > best_double_minimax) {
				break;
			}
		}
		if (double_minimax_score < best_double_minimax) {
			best_double_minimax = double_minimax_score;
			best_minimax = minimax_score;
			best_idx = idx;
		}
		std::cout << "Current word: " << words[idx] << " (minimax score: " << minimax_score;
		std::cout << ", double minimax score: " << double_minimax_score << ")" << std::endl;
	}
	std::cout << "Best word: " << words[best_idx] << " (minimax score: " << best_minimax;
	std::cout << ", double minimax score: " << best_double_minimax << ")" << std::endl;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cout << "Usage: get_best_first_guess word_file answer_file" << std::endl;
		return 0;	
	}
	std::string word_file = argv[1];
	std::string answer_file = argv[2];	
	input(word_file, answer_file);
	preprocess();
	work();
}

