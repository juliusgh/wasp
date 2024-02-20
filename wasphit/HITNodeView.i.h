#ifndef WASP_HITNODE_VIEW_I_H
#define WASP_HITNODE_VIEW_I_H
template<class NV>
HITNodeView& HITNodeView::operator=(const NV& b)
{
    m_node_index = b.node_index();
    m_pool       = b.node_pool();
    return *this;
}
#endif
