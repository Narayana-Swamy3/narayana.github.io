#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    int frequency;

    TrieNode() {
        isEndOfWord = false;
        frequency = 0;
    }
};

class Trie {
private:
    TrieNode* root;

    void dfs(TrieNode* node, const string& prefix, string current, vector<pair<string, int>>& results) {
        if (node->isEndOfWord) {
            results.push_back({prefix + current, node->frequency});
        }
        for (auto& child : node->children) {
            dfs(child.second, prefix, current + child.first, results);
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children.count(c))
                node->children[c] = new TrieNode();
            node = node->children[c];
        }
        node->isEndOfWord = true;
        node->frequency++;
    }

    vector<string> autocomplete(const string& prefix, int k = 5) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (!node->children.count(c)) return {};
            node = node->children[c];
        }

        vector<pair<string, int>> results;
        dfs(node, prefix, "", results); // pass empty current suffix

        // Sort results by frequency (descending), then lexicographically
        sort(results.begin(), results.end(), [](auto& a, auto& b) {
            if (a.second == b.second) return a.first < b.first;
            return a.second > b.second;
        });

        // Extract top-k suggestions
        vector<string> suggestions;
        for (int i = 0; i < min(k, (int)results.size()); ++i)
            suggestions.push_back(results[i].first);
        return suggestions;
    }
};

int main() {
    Trie trie;

    // Hardcoded dictionary with words starting from every alphabet
    vector<string> sampleWords = {
        "apple", "ant", "apex",
        "banana", "bat", "ball",
        "cat", "cap", "can",
        "dog", "door", "deer",
        "elephant", "eagle", "exit",
        "fish", "frog", "fan",
        "goat", "grape", "glass",
        "hat", "hen", "hope",
        "ice", "ink", "ivy",
        "jungle", "jar", "jewel",
        "kite", "kangaroo", "kitten",
        "lion", "lamp", "lemon",
        "monkey", "mouse", "mango",
        "nest", "net", "night",
        "owl", "ox", "orange",
        "pig", "pen", "peach",
        "queen", "quiz", "quill",
        "rabbit", "rat", "rope",
        "sun", "snake", "sand",
        "tiger", "top", "tent",
        "umbrella", "urn", "use",
        "vase", "violin", "village",
        "wolf", "window", "water",
        "xray", "xenon", "xylophone",
        "yak", "yarn", "yellow",
        "zebra", "zip", "zone"
    };

    // Insert all words into the trie
    for (const auto& word : sampleWords) {
        trie.insert(word);
    }

    string input;
    cout << "Enter a prefix to autocomplete (type 'exit' to quit):\n";
    while (true) {
        cout << "> ";
        cin >> input;
        if (input == "exit") break;

        vector<string> suggestions = trie.autocomplete(input);
        if (suggestions.empty()) {
            cout << "No suggestions found.\n";
        } else {
            cout << "Suggestions:\n";
            for (auto& word : suggestions)
                cout << " - " << word << '\n';
        }
    }

    return 0;
}
