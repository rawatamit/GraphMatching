#include "GraphReader.h"
#include "PreferenceList.h"
#include <cstdlib>
#include <stdexcept>

/// Lexer class defined here

Lexer::Lexer(const char* file_name)
    : ch_(' ')
{
    in_.open(file_name);
    if (not in_) {
        throw new std::runtime_error("error reading file");
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

        while (isalnum(ch_)) {
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
        throw new std::runtime_error("invalid data in file");
    } else {
        consume(); // skip the token
    }
}

/// partion are of the format
/// @Partition (A|B)
/// a, b, c ;
/// @End
void GraphReader::read_partition(VertexMap& vmap) {
    consume(); // assume that the call to this function was sane

    // read the vertices in the partion
    while (curtok_ != TOK_SEMICOLON) {
        std::string v = lexer_->get_lexeme();
        int lower_quota = 0, upper_quota = 1;
        match(TOK_STRING);

        // does this vertex specify quota(s)
        // (upper), (lower, upper)
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
void GraphReader::read_preference_list(VertexMap& vmapA, VertexMap& vmapB, bool partitionA) {
    // read the vertex for which the preference list is given
    std::string a = lexer_->get_lexeme();
    match(TOK_STRING);
    match(TOK_COLON); // skip the colon

    VertexPtr v = partitionA ? vmapA[a] : vmapB[a];
    // if the vertex is in partition A, it gives preferences
    // for vertices in partition B and vice versa
    VertexMap& partners = partitionA ? vmapB : vmapA;

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
void GraphReader::read_preference_lists(VertexMap& vmapA, VertexMap& vmapB, bool partitionA) {
    // skip the directive, we assume the call is correct
    consume();

    // read the lists
    while (curtok_ != TOK_AT) {
        read_preference_list(vmapA, vmapB, partitionA);
    }

    // directive should be properly terminated
    match(TOK_AT);
    match(TOK_END);
}

/// TODO: handle error more gracefully
/// FIXME: what if a directive is specified twice? handle that case
void GraphReader::handle_directive(VertexMap& vmapA, VertexMap& vmapB) {
    match(TOK_AT);

    switch (curtok_) {
        case TOK_PARTITION_A:
            read_partition(vmapA);
            break;
        case TOK_PARTITION_B:
            read_partition(vmapB);
            break;
        case TOK_PREF_LISTS_A:
            read_preference_lists(vmapA, vmapB, true);
            break;
        case TOK_PREF_LISTS_B:
            read_preference_lists(vmapA, vmapB, false);
            break;
        default:
            throw new std::runtime_error("error in read_graph()");
    }
}

std::unique_ptr<BipartiteGraph> GraphReader::read_graph() {
    VertexMap vmapA, vmapB;

    // parse the file
    while (curtok_ != TOK_EOF) {
        handle_directive(vmapA, vmapB);
    }

    // build the graph
    BipartiteGraph::VertexSetType A;
    BipartiteGraph::VertexSetType B;
    for (auto& it : vmapA) {
        A.emplace_back(it.second);
    }

    for (auto& it : vmapB) {
        B.emplace_back(it.second);
    }

    return std::make_unique<BipartiteGraph>(A, B);
}
