#ifndef GRAPH_READER_H
#define GRAPH_READER_H

#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "BipartiteGraph.h"

enum Token {
    TOK_AT = 0, /* 0 */
    TOK_PARTITION_A,
    TOK_PARTITION_B,
    TOK_PREF_LISTS_A,
    TOK_PREF_LISTS_B,
    TOK_END, /* 5 */
    TOK_STRING,
    TOK_COLON,
    TOK_COMMA,
    TOK_SEMICOLON,
    TOK_LEFT_BRACE, /* 10 */
    TOK_RIGHT_BRACE,
    TOK_EOF,
    TOK_ERROR,
};

class ReaderException: public std::exception {
private:
    const char* msg;

public:
    explicit ReaderException(const char* msg) : msg(msg) {}
    ~ReaderException() override = default;
    const char* what() const noexcept override { return msg; }
};

class Lexer {
private:
    int ch_;
    int lineno_;
    std::istream& in_;
    std::string lexeme_;

private:
    void read_character();

public:
    explicit Lexer(std::istream& in);
    virtual ~Lexer() = default;
    int line_number();
    Token next_token();
    std::string const& get_lexeme() const;
};

class GraphReader {
private:
    std::unique_ptr<Lexer> lexer_;
    Token curtok_;

    void consume();
    void match(Token expected);
    const char* error_message(const char* prefix, Token got, const std::vector<Token>& expected);
    void read_partition(BipartiteGraph::ContainerType& vmap);
    void read_preference_lists(BipartiteGraph::ContainerType& A, BipartiteGraph::ContainerType& B);
    void handle_partition(BipartiteGraph::ContainerType& A, BipartiteGraph::ContainerType& B);
    void handle_preference_lists(BipartiteGraph::ContainerType& A, BipartiteGraph::ContainerType& B);

public:
    explicit GraphReader(std::istream& in);
    virtual ~GraphReader() = default;
    std::shared_ptr<BipartiteGraph> read_graph();
};

#endif
