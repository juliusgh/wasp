
template<class NodeStorage>
Interpreter<NodeStorage>::Interpreter(std::ostream& err)
    : AbstractInterpreter()
    , m_start_column(1)
    , m_start_line(1)
    , m_stream_name("stream")
    , m_error_stream(err)
    , m_root_index(-1)
{
    // All documents have a root.
    // However, if no elements are parsed
    // this staged root will not be committed.
    push_staged(wasp::DOCUMENT_ROOT, "document", {});
}
template<class NodeStorage>
Interpreter<NodeStorage>::~Interpreter()
{
}
template<class NodeStorage>
NodeView Interpreter<NodeStorage>::root() const
{
    // have any nodes?
    if (m_nodes.size() == 0)
    {
        return NodeView(m_nodes.size(), *this);
    }
    return NodeView(m_root_index, *this);
}
template<class NodeStorage>
NodeView Interpreter<NodeStorage>::node_at(node_index_size index) const
{
    // have any nodes?
    if (m_nodes.size() == 0)
    {
        return NodeView(m_nodes.size(), *this);
    }
    return NodeView(index, *this);
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::child_count(size_t node_index) const
{
    return this->m_nodes.child_count(node_index);
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::column(size_t node_index) const
{
    return this->m_nodes.column(node_index);
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::last_column(size_t node_index) const
{
    return this->m_nodes.last_column(node_index);
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::line(size_t node_index) const
{
    return this->m_nodes.line(node_index);
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::last_line(size_t node_index) const
{
    return this->m_nodes.last_line(node_index);
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::parent_node_index(size_t node_index) const
{
    return this->m_nodes.parent_node_index(node_index);
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::child_index_at(size_t node_index,
                                        size_t child_index) const
{
    return this->m_nodes.child_at(node_index, child_index);
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::push_leaf(size_t      node_type,
                                   const char* node_name,
                                   size_t      token_index)
{
    size_t node_index = node_count();
    m_nodes.push_leaf(node_type, node_name, token_index);
    return node_index;
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::push_parent(size_t                     node_type,
                                     const char*                node_name,
                                     const std::vector<size_t>& child_indices)
{
    size_t node_index = node_count();
    m_nodes.push_parent(node_type, node_name, child_indices);
    return node_index;
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::type(size_t index) const
{
    // have any nodes?
    if (m_nodes.size() == 0)
    {
        return wasp::UNKNOWN;
    }
    return this->m_nodes.type(index);
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::node_token_type(size_t index) const
{
    // have any nodes?
    if (m_nodes.size() == 0)
    {
        return wasp::UNKNOWN;
    }
    return this->m_nodes.node_token_type(index);
}
template<class NodeStorage>
const char* Interpreter<NodeStorage>::name(size_t index) const
{
    // have any nodes?
    if (m_nodes.size() == 0)
    {
        return nullptr;
    }
    return this->m_nodes.name(index);
}
template<class NodeStorage>
bool Interpreter<NodeStorage>::set_name(size_t node_index, const char* node_name)
{
    return this->m_nodes.set_name(node_index, node_name);
}
template<class NodeStorage>
void Interpreter<NodeStorage>::set_type(size_t node_index, size_t node_type)
{
    return this->m_nodes.set_type(node_index, node_type);
}
template<class NodeStorage>
std::string Interpreter<NodeStorage>::data(size_t index) const
{
    // have any nodes?
    if (m_nodes.size() == 0)
    {
        return "";
    }
    return this->m_nodes.data(index);
}
template<class NodeStorage>
const char* Interpreter<NodeStorage>::token_data(size_t index) const
{
    // have any tokens?
    const auto& token_pool = m_nodes.token_data();
    if (token_pool.size() == 0)
    {
        return nullptr;
    }
    return token_pool.str(index);
}

template<class NodeStorage>
template<class PARSER_IMPL>
bool Interpreter<NodeStorage>::parse_impl(std::istream&      in,
                                  const std::string& stream_name,
                                  size_t             start_line,
                                  size_t             start_column)
{
    //    lexer.set_debug(m_trace_lexing);
    //    lexer.set_debug(true);

    m_stream_name  = stream_name;
    m_start_line   = start_line;
    m_start_column = start_column;
    m_nodes.set_start_line(start_line);
    m_nodes.set_start_column(start_column);
    PARSER_IMPL parser(*this, in, nullptr);
    //    parser.set_debug_level(true);

    bool parsed = parser.parse() == 0;

    commit_stages();

    return parsed;
}

template<class NodeStorage>
const size_t& Interpreter<NodeStorage>::staged_type(size_t staged_index) const
{
    wasp_require(staged_index < m_staged.size());
    return m_staged[staged_index].m_type;
}
template<class NodeStorage>
size_t& Interpreter<NodeStorage>::staged_type(size_t staged_index)
{
    wasp_require(staged_index < m_staged.size());
    return m_staged[staged_index].m_type;
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::push_staged(size_t                     node_type,
                                     const std::string&         node_name,
                                     const std::vector<size_t>& child_indices)
{
    m_staged.push_back(Stage());
    auto& back           = m_staged.back();
    back.m_type          = node_type;
    back.m_name          = node_name;
    back.m_child_indices = child_indices;
    return m_staged.size() - 1;
}

template<class NodeStorage>
size_t Interpreter<NodeStorage>::push_staged_child(size_t child_index)
{
    wasp_require(m_staged.empty() == false);
    auto& back = m_staged.back();
    back.m_child_indices.push_back(child_index);
    return back.m_child_indices.size();
}
template<class NodeStorage>
size_t Interpreter<NodeStorage>::commit_staged(size_t stage_index)
{
    wasp_require(m_staged.empty() == false);
    wasp_require(stage_index < m_staged.size());

    Stage& stage = m_staged[stage_index];
    size_t node_index =
        push_parent(stage.m_type, stage.m_name.c_str(), stage.m_child_indices);

    wasp_ensure(node_index >= 0 && node_index < m_nodes.size());
    m_staged.pop_back();
    // make sure to add newly realized tree node
    // to existing staged parent
    if (!m_staged.empty())
        push_staged_child(node_index);
    return node_index;
}

