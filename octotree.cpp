#include "octotree.h"

int OctoTree::size = 9;

OctoModel::OctoModel(int count, const BoundingBox & bbox)
    : m_count(count)
{
    int lenght = bbox.max.x() - bbox.min.x();
    m_root = new OctoTree;
    m_root->middle = BoundingBox::getMiddlePoint(bbox);
    m_root->lenght = lenght;
    m_root->bbox = bbox;
    createNode(&m_root, m_count, m_root->middle, lenght, 0, bbox);
}

OctoModel::~OctoModel()
{
    clear();
}

const OctoTree *OctoModel::root() const
{
    return m_root;
}

const BoundingBox &OctoModel::getBoundingBox()
{
    return m_root->bbox;
}

int OctoModel::getType()
{
    return Model::OCTOTREE;
}

void OctoModel::delimSpace(OctoTree **el, int count)
{
    PointD _middle;
    BoundingBox _bbox;
    const double _lenght = (*el)->lenght / 4;
    const int _count = count / 8;
    const double lenght = (*el)->lenght / 2;

    // one cube
    _middle[0] = (*el)->middle[0] - _lenght;
    _middle[1] = (*el)->middle[1] + _lenght;
    _middle[2] = (*el)->middle[2] + _lenght;
    _bbox.min = PointD(_middle[0] - _lenght, _middle[1] - _lenght, _middle[2] - _lenght);
    _bbox.max = PointD(_middle[0] + _lenght, _middle[1] + _lenght, _middle[2] + _lenght);
    createNode(&((*el)->nodes[1]),  _count, _middle, lenght, *el, _bbox);

    // two cube
    _middle[0] = (*el)->middle[0] + _lenght;
    _middle[1] = (*el)->middle[1] + _lenght;
    _middle[2] = (*el)->middle[2] + _lenght;
    _bbox.min = PointD(_middle[0] - _lenght, _middle[1] - _lenght, _middle[2] - _lenght);
    _bbox.max = PointD(_middle[0] + _lenght, _middle[1] + _lenght, _middle[2] + _lenght);
    createNode(&((*el)->nodes[2]),  _count, _middle, lenght, *el, _bbox);

    // three cube
    _middle[0] = (*el)->middle[0] - _lenght;
    _middle[1] = (*el)->middle[1] - _lenght;
    _middle[2] = (*el)->middle[2] + _lenght;
    _bbox.min = PointD(_middle[0] - _lenght, _middle[1] - _lenght, _middle[2] - _lenght);
    _bbox.max = PointD(_middle[0] + _lenght, _middle[1] + _lenght, _middle[2] + _lenght);
    createNode(&((*el)->nodes[3]),  _count, _middle, lenght, *el, _bbox);

    // four cube
    _middle[0] = (*el)->middle[0] + _lenght;
    _middle[1] = (*el)->middle[1] - _lenght;
    _middle[2] = (*el)->middle[2] + _lenght;
    _bbox.min = PointD(_middle[0] - _lenght, _middle[1] - _lenght, _middle[2] - _lenght);
    _bbox.max = PointD(_middle[0] + _lenght, _middle[1] + _lenght, _middle[2] + _lenght);
    createNode(&((*el)->nodes[4]),  _count, _middle, lenght, *el, _bbox);

    // five cube
    _middle[0] = (*el)->middle[0] + _lenght;
    _middle[1] = (*el)->middle[1] - _lenght;
    _middle[2] = (*el)->middle[2] - _lenght;
    _bbox.min = PointD(_middle[0] - _lenght, _middle[1] - _lenght, _middle[2] - _lenght);
    _bbox.max = PointD(_middle[0] + _lenght, _middle[1] + _lenght, _middle[2] + _lenght);
    createNode(&((*el)->nodes[5]),  _count, _middle, lenght, *el, _bbox);

    // six cube
    _middle[0] = (*el)->middle[0] + _lenght;
    _middle[1] = (*el)->middle[1] + _lenght;
    _middle[2] = (*el)->middle[2] - _lenght;
    _bbox.min = PointD(_middle[0] - _lenght, _middle[1] - _lenght, _middle[2] - _lenght);
    _bbox.max = PointD(_middle[0] + _lenght, _middle[1] + _lenght, _middle[2] + _lenght);
    createNode(&((*el)->nodes[6]),  _count, _middle, lenght, *el, _bbox);

    // seven cube
    _middle[0] = (*el)->middle[0] - _lenght;
    _middle[1] = (*el)->middle[1] - _lenght;
    _middle[2] = (*el)->middle[2] - _lenght;
    _bbox.min = PointD(_middle[0] - _lenght, _middle[1] - _lenght, _middle[2] - _lenght);
    _bbox.max = PointD(_middle[0] + _lenght, _middle[1] + _lenght, _middle[2] + _lenght);
    createNode(&((*el)->nodes[7]),  _count, _middle, lenght, *el, _bbox);

    // eight cube
    _middle[0] = (*el)->middle[0] - _lenght;
    _middle[1] = (*el)->middle[1] + _lenght;
    _middle[2] = (*el)->middle[2] - _lenght;
    _bbox.min = PointD(_middle[0] - _lenght, _middle[1] - _lenght, _middle[2] - _lenght);
    _bbox.max = PointD(_middle[0] + _lenght, _middle[1] + _lenght, _middle[2] + _lenght);
    createNode(&((*el)->nodes[8]),  _count, _middle, lenght, *el, _bbox);
}

void OctoModel::intersectBBox(const BoundingBox &bbox,
                              OctoTree *el,
                              std::list<OctoTree * > & nodeL)
{
    if(el)
    {
        if(BoundingBox::intersected(el->bbox, bbox))
        {
            nodeL.push_back(el);
            for(int i = 1; i < OctoTree::size; ++i)
                intersectBBox(bbox, el->nodes[i], nodeL);
        }
    }
}

void OctoModel::clear()
{
    delete m_root;
}

void OctoModel::search(OctoTree *el,
                       ItemUS & itemUS,
                       const Ray & ray)
{
    if(el)
    {
        const BoundingBox & bbox = el->bbox;
        PointDV v;
        v.push_back(bbox.min);
        v.push_back(bbox.max);
        if(ray.intersected(v))
        {
            for(auto & v : el->items)
                itemUS.insert(v);
            for(int i = 1; i < OctoTree::size; ++i)
                search(el->nodes[i], itemUS, ray);
        }
    }
}

bool OctoModel::containsItem(Item *item, const ItemUS & itemL)
{
    for(auto & v : itemL)
    {
        if(item == v)
            return true;
    }
    return false;
}

void OctoModel::fillVector(Ray::Type type, PointDV & v, Item * item)
{
    switch (type)
    {
    case Ray::BBOX:
    {
        const BoundingBox & bbox = item->getBoundingBox();
        v.push_back(bbox.min);
        v.push_back(bbox.max);
    }break;
    case Ray::POINT:
    {
//        if(item->getType() == Item::POINT)
//            v.push_back(((PointItem*)item)->getPoint());
    } break;
    default:
        break;
    }
    return;
}

void OctoModel::createNode(OctoTree **el, int count,
                         const PointD& middle,
                         double lenght,
                         OctoTree *up,
                         const BoundingBox & bbox)
{
    if(*el == 0)
    {
        *el = new OctoTree;
        (*el)->lenght = lenght;
        (*el)->middle = middle;
        (*el)->bbox = bbox;
        (*el)->clear();
        (*el)->nodes[0] = up;
        if(!(count % 8))
        {
            delimSpace(el, count);
        }
        else
        {
            (*el)->type = OctoTree::LEAF;
        }
    }
    else if(*el == m_root)
    {
        delimSpace(el, count);
    }
}

void OctoModel::setItemL(const ItemUS &items)
{
    for(auto & item : items)
    {
        const BoundingBox & bbox = item->getBoundingBox();
        std::list<OctoTree * > nodeL;
        intersectBBox(bbox, m_root, nodeL);
        for(auto & node : nodeL)
        {
            if(node->type == OctoTree::LEAF)
                node->items.insert(item);
        }
    }
    m_itemUS = cmerge(m_itemUS, items);
}

void OctoModel::searchItems(const Ray & ray,
                            Item_tL &item_tL)
{

    ItemUS itemUS;
    search(m_root, itemUS, RayBBox(ray.origin(), ray.direction()));
    for(Item * item : itemUS)
    {
        PointDV v;
        fillVector(ray.getType(), v, item);
        if(v.empty())
            continue;
        if(Item_t::Ptr item_t = ray.intersected(v))
        {
            item_t->item = item;
            item_tL.push_back(item_t);
        }
    }
    item_tL.sort(SortItem_t);
}

Item_t::Ptr OctoModel::searchFirstItem(const Ray & ray)
{
    Item_tL item_tL;
    searchItems(ray, item_tL);
    return item_tL.empty() ? 0 : *item_tL.begin();
}

OctoTree::OctoTree()
    : type(BRANCH)
{
    clear();
}

OctoTree::~OctoTree()
{
    for(int i = 1; i < size; ++i)
    {
        if(nodes[i])
        {
            delete nodes[i];
            nodes[i] = 0;
        }
    }
}

void OctoTree::clear()
{
    std::fill(nodes, nodes + size, (OctoTree*)0);
}
