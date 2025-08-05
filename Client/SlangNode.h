//#include <map>
//#include <boost/noncopyable.hpp>

class KSlangNode : public std::map< wchar_t, KSlangNode* >, public boost::noncopyable
{
private:
    bool m_bLeafNode;

public:
    KSlangNode() : m_bLeafNode( false ) {}
    ~KSlangNode(){
        //DeleteChild();
        //clear(); // Is it right? or
        // delete the child nodes **
        
    }

    KSlangNode* AddChild( wchar_t chWord_ )
    {
        iterator mit = find( chWord_ );
        if( mit == end() )
        {
            mit = insert( make_pair( chWord_, new KSlangNode ) ).first; //리턴값의 [.first] return "iterator"   , second = bool (Success or fail)
        }
        
        return mit->second;
    }
    KSlangNode* FindChild( wchar_t chWord_ ) const 
    {
        const_iterator mit = find( chWord_ );
        if( mit != end() )
        {
            return mit->second;
        }
        else
            return NULL;
    }

    bool DeleteLastNode(KSlangNode *node)
    {
        if( 0 == node->size() )
            return true;

        iterator mit;
        for( mit = node->begin() ; mit != node->end() ; ++mit )
        {
            if( DeleteLastNode(mit->second) )
            {
                //wchar_t temp = (*mit).first;
                SAFE_DELETE( mit->second );
            }
        }
        node->clear();
        return true;
    }

    void DeleteChild()
    {
        DeleteLastNode(this);
    }

    bool isLeafNode() const { return m_bLeafNode; }
    void setLeafNode( bool bValue_ ) { m_bLeafNode = bValue_; }
};