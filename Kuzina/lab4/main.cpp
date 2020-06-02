#include <iostream>
#include <string>
using namespace std;
int output = 0;

int* prefix(string& text) {
	//calculate the prefix function for the input string
	if (output) { 
		cout << "Calculating prefix function for: " << text << "\n"; }

	int* pi = (int*)malloc(text.length()*sizeof(int));
	pi[0] = 0; //array for prefix function values

	for (int i = 1; i < text.length(); i++){ //for each string elem:
		int tmpLen = pi[i - 1]; //current suffix length = prefix function of the previous element
		if (output) { 
			cout << "\n  Step " << i << ":"; }
		if (output) {
			cout << "\n\tWatching text[" << i << "] value => " << text[i] << "; current prefix length = " << tmpLen << "\n";
		}
		while (tmpLen > 0 && text[tmpLen] != text[i]){
		    if (output) {
				cout << "\t  text[" << tmpLen << "] value => " << text[tmpLen] << " != text[" << i << "] value => " << text[i] << "\n";
			}
			tmpLen = pi[tmpLen - 1]; //can’t increase the current suffix - go to the suffix of shorter length
			if (output) { 
				cout << "\t    Can't expend previous prefix, new prefix length = " << tmpLen << "\n"; }
			
		}	
		if (text[tmpLen] == text[i]){
		    if (output) {
				cout << "\t  text[" << tmpLen << "] value => " << text[tmpLen] << " == text[" << i << "] value => " << text[i] << "\n";
			}
			if (output) { 
				cout << "\t    Expend previous prefix, new prefix length = " << tmpLen + 1 << "\n"; }
			tmpLen++; //can increase the current suffix - the elements does match
			
		}
		if (output) {
			cout << "\tPrefix for text[" << i << "] value => " << text[i] << " = " << tmpLen << "\n";
		}
		pi[i] = tmpLen; //prefix function for the current item
	}

	for (int i = 0; i < text.length(); i++) {
		if (output) { 
			cout << "\tstr[" << i << "] value => " << text[i] << "; prefix = " << pi[i] << "\n"; }
	}
	if (output) cout << "\n";
	return pi;
}

void Pattern(string& pattern, string& text) {
	int* pi = prefix(pattern); //prefix function values for pattern
	int n = text.length();
	int pLen = pattern.length();
	string answ = "";

	if (pLen > n) {
		cout << -1;
		return;
	}
	int tmpLen = 0;
	int test = 0;
	if (output) { 
		cout << "Serching for pattern: " << pattern << " in the text: " << text << "\n"; }

	for (int i = 0; i < n; i++) { // for each element of second string
		//current suffix length = prefix function of the previous element
		if (output) { 
			cout << "\n  Step " << i << ":"; }
		if (output) {
			cout << "\n\tWatching text[" << i << "] value => " << text[i] << "; current prefix length = " << tmpLen << "\n";
		}
		while (tmpLen > 0 && pattern[tmpLen] != text[i]) {
			// can’t increase the current suffix - element does not match
			if (output) {
				cout << "\t  pattern[" << tmpLen << "] value => " << pattern[tmpLen] << " != text[" << i << "] value => " << text[i] << "\n";
			}
			tmpLen = pi[tmpLen - 1];  // try suffix of shorter length
			if (output) { 
				cout << "\t    Can't expend previous prefix, new prefix length = " << tmpLen << "\n"; }
		}				
		if (pattern[tmpLen] == text[i]) {//can increase the current suffix - the elements does match
			if (output) {
				cout << "\t  pattern[" << tmpLen << "] value => " << pattern[tmpLen] << " == text[" << i << "] value => " << text[i] << "\n";
			}
			if (output) { 
				cout << "\t    Expend previous prefix, new prefix length = " << tmpLen + 1 << "\n"; }
			tmpLen++;
		}
		if (output) {
			cout << "\tPrefix for text[" << i << "] value => " << text[i] << " = " << tmpLen << "\n";
		}
		if (tmpLen == pLen) {//the suffix length macth with the length of the pattern
	
			if (output) { 
				cout << "\nFind a pattern in the text at "; }
			if (test > 0 && !output) cout << ",";
			if (test > 0 ) answ.push_back(',');
			test++;
			cout <<  i - tmpLen + 1;// the occurrence of the pattern in the text was found
			answ = answ + to_string(i - tmpLen + 1);
			if (output) {
				cout << " position\n"; }
		}
	}
	cout << answ << "\n";
	if (test == 0) {
		if (output) { 
			cout << "There is on pattern in the next!\n"; }
		cout << -1;//No occurrence of the pattern in the text was found
	}
}

void Shift(string& pattern, string& text) {
	//if the lengths of the lines do not match, one is definitely not a shift of the other
	if (text.length() != pattern.length()) {
		if (output) { 
			cout << "Two strings has different sizes! " << text.length() << " != " << pattern.length() << "\n"; }
		else cout << -1;
		return;
	}

	int* pi = prefix(pattern); //prefix function values for first string
	int textLength = text.length();
	int tmpLen = 0;
	if (output) { 
		cout << "Check is str2 \"" << text << "\" shift of str1 \"" << pattern << "\"\n"; }

	for (int i = 0; i < textLength * 2; i++) { //loop around the second string twice
		if (output) { 
			cout << "\n  Step " << i << ":"; }
		int j = i % textLength; //for looping around, when i = textLength -> j = 0 ...
		//current suffix length = prefix function of the previous element
		if (output) {
			cout << "\n\tWatching str2[" << j << "] value => " << text[j] << "; current prefix length = " << tmpLen << "\n";
		}

		while (tmpLen > 0 && pattern[tmpLen] != text[j]) {//can’t increase the current suffix - element does not match
			if (output) {
				cout << "\t  str1[" << tmpLen << "] value => " << pattern[tmpLen] << " != str2[" << j << "] value => " << text[j] << "\n";
			}
			tmpLen = pi[tmpLen - 1];  // try suffix of shorter length
			if (output) {
				cout << "\t    Can't expend previous prefix, new prefix length = " << tmpLen << "\n";
			}
		}

		if (pattern[tmpLen] == text[j]) {//can increase the current suffix - the elements does match
			if (output) {
				cout << "\t  str1[" << tmpLen << "] value => " << pattern[tmpLen] << " == str2[" << j << "] value => " << text[j] << "\n";
			}
			if (output){ 
				cout << "\t    Expend previous prefix, new prefix length = " << tmpLen + 1 << "\n"; 
			}
			tmpLen++;
		}
		if (output) { 
			cout << "\tPrefix for str2[" << j << "] value => " << text[j] << " = " << tmpLen << "\n"; }
		if (tmpLen == textLength) {//the suffix length macth with the length of the pattern
			if (output) { 
				cout << "\nSecond string IS the first string shift! Shift = "; }
			cout << i-	textLength  + 1;  //found cyclic first string shift
			if (output) cout << "\n";
			cout << i-	textLength  + 1 << "\n";
			return;
		}
	}
	if (output) { 
		cout << "\nSecond string ISN'T the first string shift!\n"; }
	cout << -1; //the second string is not a shift of the first string
}


int main() {
	string pattern;
	string text;
	output = 1; //If = 1 there will be a detailed conclusion of the course of the decision. if = 0 there will only be an answer
	if (output) { 
		cout << "Enter first text, then press enter-button and enter second text\n"; }
	cin  >>  pattern >> text;//Input

	//You can use only one or both functions at once
	Pattern(pattern, text); //finds all occurrences of the first row in the second
	Shift(pattern, text); //checks if the second line is a shift of the first
	 

	return 0;
}