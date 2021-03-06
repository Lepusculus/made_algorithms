//
// Created by Yulia Kondratyeva on 12/5/19.
//

/* Задача 9.Задача 9. Алгоритм сжатия данных Хаффмана
(15 баллов)
Реализуйте алгоритм построения оптимального префиксного кода Хаффмана. При помощи алгоритма реализуйте две функции для создания архива из одного файла и 
извлечения файла из архива.
*/

#include "Huffman.h"
#include <set>
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>

typedef uint32_t ui;
typedef unsigned char uc;

const std::size_t BUFFER_SIZE = 32768;
const std::size_t ALPHABET_SIZE = 256;
const ui BITS_IN_CHAR = 8;

namespace {
    struct huffman {
    public:
        static void encode(IInputStream &input, IOutputStream &output);
        static void decode(IInputStream &input, IOutputStream &output);

    private:
        struct node;       struct node_wrapper;
        struct compare;
        struct code;
        static void delete_node(node *node);
        static void get_codes(node *root, const code &cur_code, std::vector<code> &codes);
        static node_wrapper build_huffman_tree(ui *freq);
        static void gen_codes(ui *freq, std::vector<code> &codes);
        static code print_full_chars_from_code(code &c, IOutputStream &out);
        static void process_code(node *&cur_node, node *&root, code &code, IOutputStream &out, ui *freq);
    };

    struct huffman::node {
            uc value;
            ui freq;
            node *left, *right;

            node(uc value, ui freq) {
                left = nullptr;
                right = nullptr;
                this->value = value;
                this->freq = freq;
            }

            bool is_leaf() {
                return !(this->left || this->right);
            }
    };

    void huffman::delete_node(huffman::node *node) {
        if (!node) {
            return;
        }
        delete_node(node->left);
        delete_node(node->right);
        delete node;
    }

    struct huffman::node_wrapper {
        node *root;

        explicit node_wrapper(huffman::node *root) : root(root) {}

        ~node_wrapper() {
            delete_node(root);
        }
    };

    struct huffman::code {
        ui size_;
        ui value_;

        code() : size_(0), value_(0) {}

        code(ui size_, ui value) : size_(size_), value_(value) {}

        void add_one() {
            add_zero();
            value_ += 1;
        }

        void add_zero() {
            value_ <<= 1u;
            size_++;
        }

        ui size() {
            return size_;
        }

        ui value() {
            return value_;
        }

        bool get(size_t j) {
            return static_cast<bool>((value_ >> j) & 1u);
        }

        void add(code &code) {
            value_ <<= code.size();
            size_ += code.size();
            value_ += code.value_;
        }

    };

    struct huffman::compare {
        bool operator()(node *l, node *r) {
            return (l->freq > r->freq);
        }
    };

    void huffman::get_codes(node *root, const code &cur_code, std::vector<code> &codes) {
    if (root->is_leaf()) {
    codes[root->value] = cur_code;
    return;
}
code left = cur_code;
code right = cur_code;
left.add_zero();
right.add_one();
get_codes(root->left, left, codes);
get_codes(root->right, right, codes);
}

huffman::node_wrapper huffman::build_huffman_tree(ui *freq) {
    std::priority_queue<huffman::node *, std::vector<huffman::node *>, compare> build;
    for (uc c = 0;; c++) {
        build.push(new huffman::node(c, freq[c]));
        if (c == 255) {
            break;
        }
    }
    while (build.size() > 1) {
        node *left = build.top();
        build.pop();
        node *right = build.top();
        build.pop();
        node *parent = new huffman::node('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        build.push(parent);
    }
    return node_wrapper(build.top());
}

void huffman::gen_codes(ui *freq, std::vector<code> &codes) {
    node_wrapper root_wrapper = build_huffman_tree(freq);
    get_codes(root_wrapper.root, code(), codes);
}

void get_freq(IInputStream &in, ui *freq, std::vector<uc>& vc) {
    uc c;
    while (in.Read(c)) {

        freq[c]++;
        vc.push_back(c);
    }
}

huffman::code huffman::print_full_chars_from_code(code &c, IOutputStream &out) {
    ui value = c.value();
    ui size = c.size();
    for (ui i = 0; i < size; i++) {
        if (size < BITS_IN_CHAR * (i + 1)) {
            ui new_size = (size - BITS_IN_CHAR * i);
            ui new_val = value & ((1u << new_size) - 1);
            return {new_size, new_val};
        } else {
            ui byte = (value >> (size - BITS_IN_CHAR * (i + 1)));
            auto ch = static_cast<uc>(byte & 0xffu);
            out.Write(ch);
        }
    }
    return {};
}

void print_number(ui n, IOutputStream &out) {
    for (ui i = 4; i-- > 0;) {
        out.Write(static_cast<uc>((n >> (BITS_IN_CHAR * i)) & 0xffu));
    }
}

void huffman::encode(IInputStream &in, IOutputStream &out) {
    ui freq[ALPHABET_SIZE];
    for (ui &i : freq) {
        i = 0;
    }

    // calculate frequencies
    std::vector<uc> vc;
    get_freq(in, freq, vc);


    // generate char codes
    std::vector<code> codes(ALPHABET_SIZE);
    gen_codes(freq, codes);

    // print frequencies
    for (ui i : freq) {
        print_number(i, out);
    }

    // print last byte's size
    ui last_bits = 0;
    uc c;
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        last_bits += (1ull * codes[i].size() * freq[i]) & 0b111u;
        last_bits &= 0b111u;
    }
    out.Write(static_cast<uc>(last_bits));

    // print encoded string
    code cur_code, rest;
    for(auto sym:vc) {
        cur_code.add(codes[sym]);
        rest = print_full_chars_from_code(cur_code, out);
        cur_code = rest;
    }
    if (cur_code.size() > 0) {
        out.Write(static_cast<uc>(cur_code.value()));
    }
    
}


ui read_number(IInputStream &in) {
    ui res = 0;
    uc c;
    for (ui i = 0; i < 4; i++) {
        if (!in.Read(c)) {
            throw std::invalid_argument("Encoded file corrupted");
        }
        res <<= BITS_IN_CHAR;
        res += c;
    }
    return res;
}

void huffman::process_code(node *&cur_node, node *&root, code &code, IOutputStream &out, ui *freq) {
    for (size_t i = code.size(); i-- > 0;) {
        if (!code.get(i)) {
            cur_node = cur_node->left;
        } else {
            cur_node = cur_node->right;
        }
        if (cur_node->is_leaf()) {
            freq[cur_node->value]++;
            out.Write(cur_node->value);
            cur_node = root;
        }
    }
}

void huffman::decode(IInputStream &in, IOutputStream &out) {
    ui freq[ALPHABET_SIZE];
    for (ui &i : freq) {
        i = 0;
    }

    // read frequencies
    for (ui &i : freq) {
        i = read_number(in);
    }

    // read last byte's size
    uc c;
    if (!in.Read(c)) {
        throw std::invalid_argument("Encoded file corrupted");
    }
    ui last_bits = c;
    if (last_bits > 7) {
        throw std::invalid_argument("Encoded file corrupted");
    }

    // restore huffman tree
    node_wrapper root_wrapper = build_huffman_tree(freq);
    node *root = root_wrapper.root;

    ui res_freq[ALPHABET_SIZE];
    for (ui &i : res_freq) {
        i = 0;
    }

    // decode the input
    node *cur_node = root;
    code cur_code, last_code;
    bool started = false;
    while (in.Read(c)) {
        cur_code = code(BITS_IN_CHAR, c);
        if (started) {
            process_code(cur_node, root, last_code, out, res_freq);
        }
        last_code = cur_code;
        started = true;
    }
    if (last_bits != 0) {
        last_code = code(last_bits, c);
    }
    process_code(cur_node, root, last_code, out, res_freq);

    // check if frequencies match
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        if (freq[i] != res_freq[i]) {
            throw std::invalid_argument("Encoded file corrupted");
        }
    }
}



}




void Encode(IInputStream& original, IOutputStream& compressed)
{

    huffman::encode(original, compressed);
}

void Decode(IInputStream& compressed, IOutputStream& original)
{
    huffman::decode(compressed, original);
}
