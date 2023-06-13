#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class Node {

    private: 
    
        char letter;
        bool endOfWord;
        Node *children[26];


    public:
    
        Node(char c) {

            letter = c;
            endOfWord = false;

            for (int i = 0; i < 26; i++) {
                children[i] = NULL;
            }
        }

        void AddChild(char c) {
            if (children[c - 'a'] == NULL) {
                children[c - 'a'] = new Node(c);
            }
        }

        void SetAsEndOfWord() {
            endOfWord = true;
        }

        bool IsEndOfWord() {
            return endOfWord;
        }

        Node* GetChild(char c) {
            return children[c - 'a'];
        }

        void SetLetter(char c) {
            letter = c;
        }
        char GetLetter() {
            return letter;
        }

        bool HasChild() {
            for (int i = 0; i < 26; i++) {
                if (children[i] != NULL) {
                    return true;
                }
            }
            return false;
        }





};

class Trie {

    private:
    
        Node *root;

        void Configure() {
            fstream wordFile;
            wordFile.open("Dictionary.txt",ios::in); 
            if (wordFile.is_open()){ 
                string word;
                while(getline(wordFile, word)){ 
                    this->Insert(word);
                }
                wordFile.close();
            };
        }



        void AggregateWords(Node *node, string prefix, vector<string> &words) {
            if (node->IsEndOfWord()) {
                words.push_back(prefix);
            }
            
            for (int i = 0; i < 26; i++) {
                Node *child = node->GetChild('a' + i);
                if (child != NULL) {
                    AggregateWords(child, prefix + child->GetLetter(), words);
                }
            }
        }

        vector<string> GetAutoCompleteOptions(string input) {
            vector<string> options;
            
            Node *current = root;
            for (int i = 0; i < input.length(); i++) {
                current = current->GetChild(input[i]);
                if (current == NULL) {
                    return options;
                }
            }
            AggregateWords(current, input, options);
            sort(options.begin(), options.end());
            
            if (options.size() > 5) {
                options.resize(3);
            }
            
            return options;
        }



        void GetAllWords(Node* node, vector<string>& words, string prefix = "") {
            if (node == NULL) {
                return;
            }
            if (node->IsEndOfWord()) {
                words.push_back(prefix + node->GetLetter());
            }
            for (int i = 0; i < 26; i++) {
                char c = 'a' + i;
                GetAllWords(node->GetChild(c), words, prefix + node->GetLetter());
            }
        }


    public:
    
        Trie() {
            root = new Node('\0');
            this->Configure();
        }

        void Insert(string word) {
            Node *current = root;
            word.erase(remove_if(word.begin(), word.end(), ::isspace), word.end());
            for (int i = 0; i < word.length(); i++) {   
                current->AddChild(word[i]);
                current = current->GetChild(word[i]);
                if (i == word.length()-1) {
                    current->SetAsEndOfWord();
                }
            }  
        }

        
        bool IsWord(string word) {
            Node *current = root;
            for (int i = 0; i < word.length(); i++) {
                current = current->GetChild(word[i]);
                if (current == NULL) {
                    return false;
                }
            }
            return current->IsEndOfWord();
        }

        vector<string> Search(string input) {
            if(this->IsValidPrefix(input)) {   
                return this->GetAutoCompleteOptions(input);
            }
            if(this->IsWord(input)) {
                return this->GetAutoCompleteOptions(input);
            }
            return this->GetWordRecommendations(input);

        }

        bool IsValidPrefix(string input){
            Node *current = root;
            for (int i = 0; i < input.length(); i++) {
                current = current->GetChild(input[i]);
                if (current == NULL) {
                    return false;
                }
            }
            if (current->HasChild()) {
            return true;
            }
            return false;  
        }

        vector<string> GetAllWords() {
                    vector<string> words;
                    GetAllWords(root, words);
                    return words;
                }

        vector<string> GetWordRecommendations(string input) {
            vector<string> recommendations;

            vector<string> allWords = GetAllWords();

            vector<pair <string, int> > rankedWords;
            for (string word : allWords) {
                int letterSimilarity = 0;
                int countSimilarity = 0;
                for (int i = 0; i < input.length(); i++) {
                    if (word.find(input[i]) != string::npos) {
                        letterSimilarity++;
                    }
                    if (count(word.begin(), word.end(), input[i]) == count(input.begin(), input.end(), input[i])) {
                        countSimilarity++;
                    }
                }
                int totalSimilarity = letterSimilarity * countSimilarity;
                rankedWords.push_back(make_pair(word, totalSimilarity));
            }

            vector<pair <string, int> > topThree;
            for (int i = 0; i < min(3, (int)rankedWords.size()); i++) {
                topThree.push_back(make_pair(rankedWords[i].first, rankedWords[i].second));
            }
            for (int i = 3; i < rankedWords.size(); i++) {
                int j;
                for (j = 0; j < topThree.size(); j++) {
                    if (rankedWords[i].second > topThree[j].second) {
                        topThree[j] = make_pair(rankedWords[i].first, rankedWords[i].second);
                        break;
                    }
                }
            }
            for (auto word : topThree) {
                recommendations.push_back(word.first);
            }

            return recommendations;
        }
        

};
