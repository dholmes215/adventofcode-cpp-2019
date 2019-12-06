// Copyright (C) 2019 David Holmes <dholmes@dholmes.us>. All rights reserved.

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace std;

struct orbit
{
    string parent;
    string satellite;
};

const char * get_filename(int argc, char ** argv)
{
    if (argc < 2) {
        cerr << "USAGE: " << argv[0] << " filename.txt";
        exit(-1);
    }
    return argv[1];
}

orbit read_pair(string_view str)
{
    regex input_regex(R"((.{1,3})\)(.{1,3}))");
    cmatch match;
    bool found = regex_search(begin(str), end(str), match, input_regex);
    assert(found); // TODO: Less horrible error checking
    return {{match[1].first, match[1].second},
            {match[2].first, match[2].second}};
}

int count_ancestors(const map<string, string> & parents, const string & object)
{
    auto found = parents.find(object);
    if (found == end(parents)) {
        return 0;
    }
    return 1 + count_ancestors(parents, found->second);
}

vector<string> get_ancestors(const map<string, string> & parents,
                             const string & object)
{
    auto found = parents.find(object);
    if (found == end(parents)) {
        return {object};
    }
    auto out = get_ancestors(parents, found->second);
    out.push_back(object);
    return out;
}

template <typename Iterator>
void print_range(ostream & s, Iterator b, Iterator e)
{
    s << '[';
    if (b != e) {
        s << *b;
        for (auto iter = next(b); iter != e; iter++) {
            s << ',' << *iter;
        }
    }
    s << "]\n";
}

template <typename Container>
void print_container(ostream & s, const Container & c)
{
    print_range(s, begin(c), end(c));
}

int main(int argc, char ** argv)
{
    ifstream input_file(get_filename(argc, argv));
    string line;
    set<string> objects;
    map<string, string> parents;
    while (getline(input_file, line)) {
        if (line.empty()) {
            continue;
        }
        orbit o = read_pair(line);
        parents[o.satellite] = o.parent;
        objects.insert(o.parent);
        objects.insert(o.satellite);
    }

    int count = 0;
    for (const string & object : objects) {
        count += count_ancestors(parents, object);
    }
    cout << "Part 1: total direct and indirect orbits: " << count << '\n';

    // Part 2
    auto you_ancestors = get_ancestors(parents, "YOU");
    auto san_ancestors = get_ancestors(parents, "SAN");

    auto first_mismatch = mismatch(begin(you_ancestors), end(you_ancestors),
                                   begin(san_ancestors), end(san_ancestors));
    auto common_ancestor_count = first_mismatch.first - begin(you_ancestors);
    auto transfers_required = you_ancestors.size() + san_ancestors.size() -
                              (common_ancestor_count * 2) - 2;
    cout << "Part 2: orbital transfers required: " << transfers_required
         << '\n';

    return 0;
}
