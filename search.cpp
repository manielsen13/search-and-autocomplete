#include <vector>
#include <iostream>
#include <algorithm>
#include "Trie.cpp"

int main() {

    Trie *trie = new Trie(); // create a Trie object

    cout << "=============" << endl;
    cout << "WELCOME TO MY SEARCH ENGINE. TYPE AN EXAPLAINATION POINT '!' TO END THE PROGRAM. THE ENGINE WILL MAKE RECOMMENDATIONS" << endl;
    cout << "=============" << endl;
    //should make this able to take multiple words as the input

    // interact with the Trie object
    while(1) {

        string input;
        cout << "Please type search queries: " << endl;
        cin >> input;

        if (input == "!") {
            break;
        }

        std::transform(input.begin(), input.end(), input.begin(), ::tolower);

        vector<string> results = trie->Search(input);

        if (trie->IsWord(input) || trie->IsValidPrefix(input)) {
            if (trie->IsWord(input)) {
                auto it = std::remove(results.begin(), results.end(), input);
                results.erase(it, results.end());
                cout << "Your search query is a valid word; however, we still may have recommendations for you:" << endl;

            }
            cout << "AUTOCOMPLETE - Your options are:" << endl;
            if (results.empty()) {
                cout << "Sorry! We don't have any options for you." << endl;
            }
            else {
                for (string result : results) {
                    cout << result << endl;
                }
            }
            
        }
        else {
            cout << "RECOMMENDATION - Do you mean:" << endl;
            if (results.empty()) {
                cout << "Sorry! We don't have any recommendations for you." << endl;
            }
            else {
                for (string result : results) {
                    cout << result << endl;
                }
            }
        }

    }
    return 0;
}