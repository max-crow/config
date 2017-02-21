#pragma once

namespace config {


struct archive {
    //archive_path
    virtual archive& operator&(aggregation& t) = 0;
    virtual archive& operator&(archive_name& t) = 0;
    virtual archive& operator&(archive_safe_reference& t) = 0;
    virtual archive& operator&(nvp& t) = 0;
};

struct in_archive :archive {
    archive& operator&(aggregation& t) {
        return operator>>(t);
    }
    archive& operator&(archive_name& t) {
        return operator>>(t);
    }
    archive& operator&(archive_safe_reference& t) {
        return operator>>(t);
    }
    archive& operator&(nvp& t) {
        return operator>>(t);
    }

    virtual archive& operator>>(aggregation& t) = 0;
    virtual archive& operator>>(archive_name& t) = 0;
    virtual archive& operator>>(archive_safe_reference& t) = 0;
    virtual archive& operator>>(nvp& t) = 0;
};

struct out_archive :archive {
    archive& operator&(const nvp& t) {
        return operator<<(t);
    }
    virtual archive& operator<<(const nvp& t) = 0;
};

//-------------------------------------------------------------------------------------------------
class xml_read_archive :public in_archive{
    typedef xml_read_archive this_type;
public:
    xml_read_archive(config::element_ptr element) 
        :element(element) {
    }

    xml_read_archive& operator>>(config::aggregation& t)
    {
        bool get_all_childs = (t.name == L"*");
        for(config::element_ptr child = element->first_child(); !child->is_null(); child = child->next_sibling()) {
            if(get_all_childs || child->name() == t.name) {
                t.value.serialize(this_type(child));
            }
        }
        return *this;
    }

    xml_read_archive& operator>>(config::archive_name& name)
    {
        name.value = element->name();
        return *this;
    }

    xml_read_archive& operator>>(config::archive_safe_reference& ref)
    {
        ref.value = element->get_safe_reference();
        return *this;
    }

    xml_read_archive& operator>>(config::nvp& t)
    {
        t.set_value(element->attribute(t.name));
        return *this;
    }
private:
    config::element_ptr element;
};
//-------------------------------------------------------------------------------------------------



}
