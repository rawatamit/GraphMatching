#include "GraphReader.h"
#include "Vertex.h"
#include "PreferenceList.h"
#include <cstdlib>
#include <stdexcept>

/// Lexer class defined here

Lexer::Lexer(const char* file_name)
    : ch_(' ')
{
    in_.open(file_name);
    if (not in_) {
        throw std::runtime_error("error opening file.");
    }
}

Lexer::~Lexer() {
    in_.close();
}

Token Lexer::next_token() {
    // skip whitespace
    while (isspace(ch_)) {
        ch_ = in_.get();
    }

    // skip comments
    if (ch_ == '#') {
        while (ch_ != '\n' and ch_ != EOF) {
            ch_ = in_.get();
        }
    }

    // skip newline and spaces
    if (isspace(ch_)) { ch_ = in_.get(); return next_token(); }
    if (ch_ == EOF)   { return TOK_EOF; }
    if (ch_ == ':')   { ch_ = in_.get(); return TOK_COLON; }
    if (ch_ == '@')   { ch_ = in_.get(); return TOK_AT; }
    if (ch_ == ',')   { ch_ = in_.get(); return TOK_COMMA; }
    if (ch_ == ';')   { ch_ = in_.get(); return TOK_SEMICOLON; }
    if (ch_ == '(')   { ch_ = in_.get(); return TOK_LEFT_BRACE; }
    if (ch_ == ')')   { ch_ = in_.get(); return TOK_RIGHT_BRACE; }

    // a directive or a string
    if (isalnum(ch_)) {
        lexeme_.clear();

        while (isalnum(ch_) or ch_ == '+') {
            lexeme_.push_back(ch_);
            ch_ = in_.get();
        }

        if (lexeme_ == "End") return TOK_END;
        if (lexeme_ == "PartitionA") return TOK_PARTITION_A;
        if (lexeme_ == "PartitionB") return TOK_PARTITION_B;
        if (lexeme_ == "PreferenceListsA") return TOK_PREF_LISTS_A;
        if (lexeme_ == "PreferenceListsB") return TOK_PREF_LISTS_B;
        return TOK_STRING;
    }

    // flag error, return the erraneous character
    lexeme_.clear();
    lexeme_.push_back(ch_);
    ch_ = in_.get();
    return TOK_ERROR;
}

std::string const& Lexer::get_lexeme() const {
    return lexeme_;
}

/// GraphReader class defined here

GraphReader::GraphReader(const char* file_name) {
    lexer_ = std::make_unique<Lexer>(file_name);
    consume(); // read first token
}

GraphReader::~GraphReader()
{}

void GraphReader::consume() {
    curtok_ = lexer_->next_token();
}

/// TODO: handle errors more gracefully
void GraphReader::match(Token expected) {
    if (curtok_ != expected) {
        throw std::runtime_error("invalid data in file");
    } else {
        consume(); // skip the token
    }
}

/// partition are of the format
/// @Partition (A|B)
/// a, b, c ;
/// @End
void GraphReader::read_partition(BipartiteGraph::ContainerType& vmap) {
    consume(); // assume that the call to this function was sane

    // read the vertices in the partion
    while (curtok_ != TOK_SEMICOLON) {
        std::string v = lexer_->get_lexeme();
        int lower_quota = 0, upper_quota = 1;
        match(TOK_STRING);

        // does this vertex specify quota(s)
        // (upper) or (lower, upper)
        if (curtok_ == TOK_LEFT_BRACE) {
            // eat '('
            match(TOK_LEFT_BRACE);

            // read the upper quota
            upper_quota = std::atoi(lexer_->get_lexeme().c_str());
            match(TOK_STRING);

            // check if this vertex has a lower quota as well
            if (curtok_ == TOK_COMMA) {
                match(TOK_COMMA);

                // the quota read first was the lower quota
                lower_quota = upper_quota;
                upper_quota = std::atoi(lexer_->get_lexeme().c_str());
                match(TOK_STRING);
            }

            // eat ')'
            match(TOK_RIGHT_BRACE);
        }

        // add this vertex with the required quotas
        vmap.emplace(v, std::make_shared<Vertex>(v, lower_quota, upper_quota));

        // if there are more vertices, they must
        // be delimited using commas
        if (curtok_ != TOK_SEMICOLON) {
            match(TOK_COMMA);
        }
    }

    // list should be delimited by a semicolon
    match(TOK_SEMICOLON);

    // end of this directive
    match(TOK_AT);
    match(TOK_END);
}

/// preference lists for a vertex are given in this format
/// the v: part has already been parsed by read_preference_lists()
/// v: a, b, c ;
void GraphReader::read_preference_list(BipartiteGraph::ContainerType& A,
                                       BipartiteGraph::ContainerType& B,
                                       bool partitionA)
{
    // read the vertex for which the preference list is given
    std::string a = lexer_->get_lexeme();
    match(TOK_STRING);
    match(TOK_COLON); // skip the colon

    // if the vertex is in partition A, it gives preferences
    // for vertices in partition B and vice versa
    VertexPtr v = partitionA ? A[a] : B[a];
    BipartiteGraph::ContainerType& partners = partitionA ? B : A;

    // read and store the preference list
    PreferenceList& pref_list = v->get_preference_list();
    while (curtok_ != TOK_SEMICOLON) {
        std::string b = lexer_->get_lexeme();
        match(TOK_STRING);
        pref_list.emplace_back(partners[b]);

        // if there are more vertices, they must
        // be delimited using commas
        if (curtok_ != TOK_SEMICOLON) {
            match(TOK_COMMA);
        }
    }

    // preference list should be delimited by a semicolon
    match(TOK_SEMICOLON);
    //v->set_preference_list(pref_list);
}

/// @PreferenceLists (A|B)
/// for preference list format, see read_preference_list()
/// @End
void GraphReader::read_preference_lists(BipartiteGraph::ContainerType& A,
                                        BipartiteGraph::ContainerType& B,
                                        bool partitionA)
{
    // skip the directive, we assume the call is correct
    consume();

    // read the lists
    while (curtok_ != TOK_AT) {
        read_preference_list(A, B, partitionA);
    }

    // directive should be properly terminated
    match(TOK_AT);
    match(TOK_END);
}

/// TODO: handle error more gracefully
/// FIXME: what if a directive is specified twice? handle that case
void GraphReader::handle_directive(BipartiteGraph::ContainerType& A,
                                   BipartiteGraph::ContainerType& B)
{
    match(TOK_AT);

    switch (curtok_) {
        case TOK_PARTITION_A:
            read_partition(A);
            break;
        case TOK_PARTITION_B:
            read_partition(B);
            break;
        case TOK_PREF_LISTS_A:
            read_preference_lists(A, B, true);
            break;
        case TOK_PREF_LISTS_B:
            read_preference_lists(A, B, false);
            break;
        default:
            throw std::runtime_error("error in read_graph()");
    }
}

std::unique_ptr<BipartiteGraph> GraphReader::read_graph() {
    BipartiteGraph::ContainerType A, B;

    // parse the file
    while (curtok_ != TOK_EOF) {
        handle_directive(A, B);
    }

    return std::make_unique<BipartiteGraph>(A, B);
}
