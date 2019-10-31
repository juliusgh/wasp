#ifndef WASP_GETPOTNODE_VIEW_H
#define WASP_GETPOTNODE_VIEW_H

#include <string>
#include <sstream>
#include <ostream>
#include <iostream>
#include "waspcore/Interpreter.h"
#include "waspcore/decl.h"
namespace wasp
{
/**
 * @brief The GetPotNodeView class provides light weight interface to GetPot
 * Nodes
 * Allows traversing child nodes and parent as well as acquire node information
 * *
 */
class WASP_PUBLIC GetPotNodeView
{
  public:
    using Collection = std::vector<GetPotNodeView>;
    GetPotNodeView() : m_node_index(-1), m_pool(nullptr) {}
    GetPotNodeView(std::size_t node_index, const AbstractInterpreter& data);
    template<class NV>
    GetPotNodeView(const NV& orig);
    GetPotNodeView(const GetPotNodeView& orig);

    ~GetPotNodeView();

    GetPotNodeView& operator=(const GetPotNodeView& b);
    template<class NV>
    GetPotNodeView& operator=(const NV& b);

    bool operator==(const GetPotNodeView& b) const;
    bool operator!=(const GetPotNodeView& b) const { return !(*this == b); }
    /**
     * @brief operator < orders by pool index
     * @param b the node to compare pool index
     * @return true, iff this views index is strictly less than b.index
     */
    bool operator<(const GetPotNodeView& b) const;
    /**
     * @brief equal determines if this is equal to the provides GetPotNodeView
     * @return true, iff and only if the nodes are the same
     */
    bool equal(const GetPotNodeView& b) const { return *this == b; }

    /**
     * @brief data acquire the node's data
     * @return the node's data
     */
    std::string data() const;
    /**
     * @brief parent acquire the parent view of the current node
     * @return
     */
    GetPotNodeView parent() const;
    /**
     * @brief has_parent determine if this node has a parent
     * @return true, iff this node has a parent
     */
    bool has_parent() const;

    /**
     * @brief id will return an empty string as it doesn't apply to GetPot nodes
     * @return empty string
     */
    std::string id() const { return ""; }
    /**
     * @brief id_child will return a null view as it doesn't apply to GetPOt nodes
     * @return null view
     */
    GetPotNodeView id_child() const { return GetPotNodeView(); }

    /**
     * @brief is_decorative determine if this node is decorative
     * @return true, iff the node is a decorative '(','[','{', etc.
     * Declarators and terminators are decorative
     */
    bool is_decorative() const;

    bool is_leaf() const;

    /**
     * @brief is_declarator determine if this node represents a declarator
     * @return true, iff the node is a declarator
     */
    bool is_declarator() const;

    /**
     * @brief is_terminator determine if this node represents a terminator
     * @return true, iff the node is a terminator
     */
    bool is_terminator() const;

    /**
     * @brief non_decorative_children acquires all non decorative children
     * @return collection of GetPotNodeViews
     */
    GetPotNodeView::Collection non_decorative_children() const;
    /**
     * @brief first_non_decorative_child_by_name acquires the first non
     * decorative child
     * @return GetPotNodeView - need to check is_null to ensure valid node
     */
    GetPotNodeView
    first_non_decorative_child_by_name(const std::string& name) const;
    /**
     * @brief non_decorative_children_count convenience to determine number of
     * decorative
     * @return number of decorative children
     */
    size_t non_decorative_children_count() const;

    /**
     * @brief is_null determines if this view is backed by a storage pool
     * @return true, iff the view has no storage backing
     */
    bool is_null() const
    {
        return m_pool == nullptr || m_node_index == m_pool->size();
    }

    /**
     * @brief path acquire the path of this node from the document root
     * @return path to node, e.g., '/object/key/value'
     */
    std::string path() const;
    /**
     * @brief paths acquire the paths of this node and its children
     * @param out the output stream to capture the node paths
     * The node paths are written, new line delimited
     */
    void paths(std::ostream& out) const;
    /**
     * @brief child_count acquire the number of nodes for which this node is a
     * parent
     * @return child count
     */
    std::size_t child_count() const;
    /**
     * @brief child_count_by_name determines the number of children with the
     * given name
     * @param name the name of the child nodes to count
     * @param limit the limit (0 reserved as no limit) which can be used to
     * optimize determination of named children
     * @return the number of children with the given name
     */
    std::size_t child_count_by_name(const std::string& name,
                                    size_t             limit = 0) const;

    /**
     * @brief child_at acquire the child node view at the given index
     * @param index the index of the child [0-child_count())
     * @return GetPotNodeView describing the child node
     */
    GetPotNodeView child_at(std::size_t index) const;

    /**
     * @brief child_by_name acquire child nodes by the given name
     * @param name the name of the children to be retrieved
     * @param limit the limit on the number of children ( 0 := no limit )
     * @return A collection of views. Empty if no match occurrs
     */
    GetPotNodeView::Collection child_by_name(const std::string& name,
                                             size_t limit = 0) const;
    /**
     * @brief first_child_by_name acquires the first child with the given name
     * @param name the name of the requested child
     * @return Named NodeView as requestd. is_null indicates if none was
     * found
     */
    GetPotNodeView first_child_by_name(const std::string& name) const;
    /**
     * @brief type acquire the type of the node
     * @return the node's type
     */
    std::size_t type() const;
    /**
     * @brief name acquire the name of the node
     * @return the node's name
     */
    const char* name() const;

    /**
     * @brief line acquire the node's starting line
     * @return the starting line of the node
     */
    std::size_t line() const;
    /**
     * @brief column acquire the node's starting column
     * @return the starting column of the node
     */
    std::size_t column() const;

    /**
     * @brief last_line acquire the node's ending line
     * @return the ending line of the node
     */
    std::size_t last_line() const;

    /**
     * @brief last_column acquire the node's ending column
     * @return the ending column of the node
     */
    std::size_t last_column() const;

    /**
     * @brief node_index acquire the index into the tree node data pool
     * @return the index into the data pool
     */
    std::size_t node_index() const { return m_node_index; }

    /**
     * @brief node_pool acquire the pointer to the backend storage
     * @return the TreeNodePool that backs this view
     */
    const AbstractInterpreter* node_pool() const { return m_pool; }

    // !> Type operators
    /**
     * @brief to_bool converts the data to a bool
     * @return the data as a bool
     */
    bool to_bool(bool* ok = nullptr) const;

    /**
     * @brief to_int converts the data to an integer
     * @return the data as an integer
     */
    int to_int(bool* ok = nullptr) const;

    /**
     * @brief to_double converts the data to a double
     * @return the data as a double
     */
    double to_double(bool* ok = nullptr) const;

    /**
     * @brief to_string converts the data to a string
     * @return the data as a string (single and double quotes are removed from
     * front and back).
     */
    std::string to_string(bool* ok = nullptr) const;

    /**
     * @brief last_as_string acquires this node or last child's node as string
     * @param ok parameter to allow determination of failed conversion, etc.
     * @return the last child's data as a string, or this node's data as a
     * string if no children
     */
    std::string last_as_string(bool* ok = nullptr) const;

    // Friendly stream operator
    friend std::ostream& operator<<(std::ostream&               str,
                                    const wasp::GetPotNodeView& view)
    {
        str << "GetPotNodeView(node_index=" << view.m_node_index
            << ", &pool=" << view.m_pool << ")";
        return str;
    }

  private:
    size_t                     m_node_index;
    const AbstractInterpreter* m_pool;

    /**
     * @brief value_node_index when the value is requested (to_int, string,
     * etc) this returns the index
     * @return index of the node considered this node's value
     * I.e.,  key = value
     *  key
     *  |_decl
     *  |_=
     *  |_value
     * When key's value is requested, the index of the 'value' node will be
     * returned.
     * If no children, the m_node_index is returned
     */
    size_t value_node_index() const;
};
#include "waspgetpot/GetPotNodeView.i.h"
}  // end of namespace

#endif
