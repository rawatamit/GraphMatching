#ifndef GRAPH_READER_H
#define GRAPH_READER_H

#include "Vertex.h"
#include "BipartiteGraph.h"
#include <map>
#include <string>
#include <fstream>

enum Token {
    TOK_AT = 0,
    TOK_PARTITION_A,
    TOK_PARTITION_B,
    TOK_PREF_LISTS_A,
    TOK_PREF_LISTS_B,
    TOK_END, /* 5 */
    TOK_STRING,
    TOK_COLON,
    TOK_COMMA,
    TOK_SEMICOLON,
    TOK_NEWLINE, /* 10 */
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
    typedef std::map<Vertex::IdType, Vertex*> VertexMap;

private:
    Lexer* lexer_;
    Token curtok_;
    
    void consume();
    void match(Token expected);
    void read_partition(VertexMap& vmap);
    void read_preference_list(VertexMap& vmapA, VertexMap& vmapB, bool partitionA);
    void read_preference_lists(VertexMap& vmapA, VertexMap& vmapB, bool partitionA);
    void handle_directive(VertexMap& vmapA, VertexMap& vmapB);

public:
    GraphReader(const char* file_name);
    virtual ~GraphReader();
    BipartiteGraph* read_graph();
};

#endif
