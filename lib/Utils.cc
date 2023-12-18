#include "Utils.h"
#include "Vertex.h"
#include "GraphReader.h"
#include <set>
#include <sstream>

const char* token_to_string(Token tok) {
   static struct {
       const char* desc;
       Token token;
   } array [] = {
           {"@", TOK_AT},
           {"@PartitionA", TOK_PARTITION_A},
           {"@PartitionB", TOK_PARTITION_B},
           {"@PreferenceListsA", TOK_PREF_LISTS_A},
           {"@PreferenceListsB", TOK_PREF_LISTS_B},
           {"@End", TOK_END},
           {"STRING", TOK_STRING},
           {":", TOK_COLON},
           {",", TOK_COMMA},
           {";", TOK_SEMICOLON},
           {"(", TOK_LEFT_BRACE},
           {")", TOK_RIGHT_BRACE},
           {"EOF", TOK_EOF},
           {"ERROR", TOK_ERROR},
           {nullptr}};

   for (int i = 0; array[i].desc != nullptr; ++i) {
       if (array[i].token == tok) {
           return array[i].desc;
       }
   }

   return "UNKNOWN";
}

int to_integer(const std::string& s) {
    return (int) std::strtol(s.c_str(), nullptr, 10);
}

VertexPtr get_vertex_by_id(std::shared_ptr<BipartiteGraph> G, const IdType& id) {
    auto A_it = G->get_A_partition().find(id);

    return A_it == G->get_A_partition().end() ? G->get_B_partition().find(id)->second : A_it->second;
}

std::shared_ptr<BipartiteGraph> read_graph(const std::string& filepath) {
    std::ifstream fin(filepath);
    return GraphReader(fin).read_graph();
}

// a new id is of the form id^k
IdType get_vertex_id(const IdType& id, int k) {
    std::stringstream stmp;
    stmp << id << '^' << k;
    return stmp.str();
}

int get_vertex_level(const IdType& id) {
    return to_integer(id.substr(id.find('^')+1));
}

// a dummy id is of the form d^k_id
IdType get_dummy_id(const IdType& id, int k) {
    std::stringstream stmp;
    stmp << "d^" << k << '_' << id;
    return stmp.str();
}

// return the dummy level from the given id
int get_dummy_level(const IdType& id) {
    auto caret_pos = id.find('^');
    auto underscore_pos = id.find('_');
    return to_integer(id.substr(caret_pos+1, underscore_pos-caret_pos-1));
}

RankType compute_rank(VertexPtr u, const PreferenceList& pref_list) {
    auto index = pref_list.find_index(u);
    return (RankType) ((index + 1) /*+ (level * pref_list.size())*/ );
}

void print_matching(std::shared_ptr<BipartiteGraph> G,
                    const Matching& M, std::ostream& out)
{
    std::stringstream stmp;

    for (const auto& it : G->get_A_partition()) {
        auto u = it.second;
        auto M_u = M.get_partners(u);
        auto u_pref_list = u->get_preference_list();

        for (const auto& i : M_u) {
            auto v = i.vertex;

            auto rank = compute_rank(v, u_pref_list);
            stmp << u->get_id() << ','
                  << v->get_id() << ','
                  << rank << '\n';
        }
    }

    out << stmp.str();
}
