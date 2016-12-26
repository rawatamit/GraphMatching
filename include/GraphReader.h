#ifndef GRAPH_READER_H
#define GRAPH_READER_H

#include <fstream>
#include <map>
#include <string>
#include "BipartiteGraph.h"
#include "TDefs.h"

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
    TOK_NEWLINE,
    TOK_EOF,
    TOK_ERROR,
};

class Lexer {
    int ch_;
    std::ifstream in_;
    std::string lexeme_;

public:
    Lexer(const char* file_name);
    virtual ~Lexer();
    Token next_token();
    std::string const& get_lexeme() const;
};

class GraphReader {
private:
    std::unique_ptr<Lexer> lexer_;
    Token curtok_;

    void consume();
    void match(Token expected);
    void read_partition(BipartiteGraph::ContainerType& vmap);
    void read_preference_list(BipartiteGraph::ContainerType& A,
                              BipartiteGraph::ContainerType& B,
                              bool partitionA);
    void read_preference_lists(BipartiteGraph::ContainerType& A,
                               BipartiteGraph::ContainerType& B,
                               bool partitionA);
    void handle_directive(BipartiteGraph::ContainerType& A,
                          BipartiteGraph::ContainerType& B);

public:
    GraphReader(const char* file_name);
    virtual ~GraphReader();
    std::unique_ptr<BipartiteGraph> read_graph();
};

#endif
