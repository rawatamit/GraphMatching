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
public:
    typedef std::map<IdType, VertexPtr> VertexMap;

private:
    std::unique_ptr<Lexer> lexer_;
    Token curtok_;

    void consume();
    void match(Token expected);
    void read_partition(VertexMap& vmap);
    void read_preference_list(VertexMap& vmapA, VertexMap& vmapB,
                              bool partitionA);
    void read_preference_lists(VertexMap& vmapA, VertexMap& vmapB,
                               bool partitionA);
    void handle_directive(VertexMap& vmapA, VertexMap& vmapB);

public:
    GraphReader(const char* file_name);
    virtual ~GraphReader();
    std::unique_ptr<BipartiteGraph> read_graph();
};

#endif
