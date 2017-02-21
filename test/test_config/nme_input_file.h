#ifndef __NME_INPUT_FILE_H__
#define __NME_INPUT_FILE_H__

#include <fstream>
#include <string>
#include <vector>
#include <boost/any.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>


#include "core/config/config.h"

#if !defined (BOOST_LITTLE_ENDIAN) && !defined (BOOST_BIG_ENDIAN)
//#   include <boost/cast.hpp>
    #include <boost/detail/endian.hpp>
    #if !defined (BOOST_LITTLE_ENDIAN) && !defined (BOOST_BIG_ENDIAN)
        #error No Endian defined
    #endif
#endif

#if defined (BOOST_LITTLE_ENDIAN)
    #define HOST_IS_LITTLE_ENDIAN true
#elif defined(BOOST_BIG_ENDIAN)
    #define HOST_IS_LITTLE_ENDIAN false
#else
    #error No Endian defined
#endif


namespace siu {
;

BOOST_STATIC_ASSERT(sizeof(unsigned short) > sizeof(unsigned char));

//template <class T, class U>
struct endian {
    enum { 
        host_is_little_endian = HOST_IS_LITTLE_ENDIAN,
        host_is_big_endian = !host_is_little_endian,

        host_is_network_byte_order = host_is_big_endian
    };

    typedef unsigned char uint8;
    typedef unsigned short uint16;
    typedef unsigned int uint32;
    typedef unsigned long long uint64;
    BOOST_STATIC_ASSERT(sizeof(uint8)==1);
    BOOST_STATIC_ASSERT(sizeof(uint16)==2);
    BOOST_STATIC_ASSERT(sizeof(uint32)==4);
    BOOST_STATIC_ASSERT(sizeof(uint64)==8);

    template <class T>
    static T big_endian_to_host(T big_endian_value) {
        //BOOST_STATIC_ASSERT( 
        //    boost::is_integral<T>::value ||
        //    boost::is_floating_point<T>::value
        //);

        if(host_is_big_endian || sizeof(T) == 1) {
            return big_endian_value;
        }
        return reverse(big_endian_value);
    }

    template <class T>
    static T reverse(T src) {
        if(sizeof(T) == 1) {
            return src;
        }
        return reverser<sizeof(T)>::reverse(src);
    }


    template <int> 
    struct reverser {
        template <class T>
        static T reverse(T src) {
            T dest;
            uint8* src_bytes = (uint8*)&src;
            uint8* dest_bytes = (uint8*)&dest;
            for(size_t i=0; i<sizeof(T); i++) {
                dest_bytes[i] = src_bytes[sizeof(T)-i-1];
            }
            return dest;
        }
    };
    
    template <> 
    struct reverser<2> {
        typedef uint16 uint;
        template <class T>
        static T reverse(T src) {
            BOOST_STATIC_ASSERT( sizeof(T) == sizeof(uint) );
            uint *usrc = (uint*)&src;
            uint dest = (
                (*usrc & 0x00FF) << 8 | 
                (*usrc & 0xFF00) >> 8 
            );
            return *(T*)&dest;
        }
    };

    template <> 
    struct reverser<4> {
        typedef uint32 uint;
        template <class T>
        static T reverse(T src) {
            BOOST_STATIC_ASSERT( sizeof(T) == sizeof(uint) );
            uint *usrc = (uint*)&src;
            uint dest = (
                (*usrc &0x000000FF) << 24 | 
                (*usrc &0x0000FF00) << 8 |
                (*usrc &0x00FF0000) >> 8 | 
                (*usrc &0xFF000000) >> 24 
            );
            return *(T*)&dest;
        }
    };

    template <> 
    struct reverser<8> {
        typedef uint64 uint;
        template <class T>
        static T reverse(T src) {
            BOOST_STATIC_ASSERT( sizeof(T) == sizeof(uint) );
            uint *usrc = (uint*)&src;
            uint dest = (
                (*usrc &0x00000000000000FF) << 56 | 
                (*usrc &0x000000000000FF00) << 40 |
                (*usrc &0x0000000000FF0000) << 24 | 
                (*usrc &0x00000000FF000000) << 8 |
                (*usrc &0x000000FF00000000) >> 8 | 
                (*usrc &0x0000FF0000000000) >> 24 |
                (*usrc &0x00FF000000000000) >> 40 | 
                (*usrc &0xFF00000000000000) >> 56 
            );
            return *(T*)&dest;
        }
    };
};

class input_stream {
    typedef std::ifstream stream_type;
public:
    void open(const char * file_name) {
        stream.open(file_name);
        position = 0;
    }

    void close() {
        stream.close();
    }

    template <class T>
    void operator >>(T& var) {
        read_into(var);
    }

    void read(void* buff, size_t size) {
        stream.read((stream_type::char_type*)buff, size);
        position += size;
    }

    template <class T>
    void read_into(T& var) {
        read(&var, sizeof(var)/sizeof(stream_type::char_type));
        BOOST_STATIC_ASSERT(boost::is_integral<T>::value || boost::is_floating_point<T>::value); //���.... �� ����� �� �����������, �� ���... ����� ���������, ����� ������ ��������� �� ������
        //if(boost::is_integral<T>::value) {
            var = endian::big_endian_to_host(var);
        //}
    }

    template <class T>
    void read_into(std::vector<T>& arr) {
        if(arr.size() > 0) {
            read(&arr[0], arr.size()*sizeof(T)/sizeof(stream_type::char_type));
        }
    }

    template <>
    void read_into(std::wstring& var) {
        var = read_utf();
    }

    std::wstring read_utf() {
        unsigned short size;
        BOOST_STATIC_ASSERT(sizeof(size) == 2);
        this->read_into(size);
        if(size == 0) {
            return L"";
        }

        typedef unsigned char byte;
        BOOST_STATIC_ASSERT(sizeof(byte) == 1);
        std::vector<byte> bytearr(size);
        read(&bytearr[0], size);

        std::wstring str(size+1, L'\0');
        std::wstring::iterator i = str.begin();
        int c, char2, char3;
        int count = 0;
        while(count < size) {
            if(bytearr[count] < 128) {
                *i++ = bytearr[count++];
                continue;
            }

            c = (int)bytearr[count] & 0xff;
            switch(c >> 4) {
                case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                    /* 0xxxxxxx*/
                    count++;
                    str.push_back((wchar_t)c);
                    break;
                case 12: case 13:       //TODO MaximV: ��������� ������ � ��������� ������� ������ �����.
                    /* 110x xxxx   10xx xxxx*/
                    count += 2;
                    if(count > size) {
                        throw new std::exception("UTFDataFormatException");
                    }
                    char2 = (int)bytearr[count-1];
                    if ((char2 & 0xC0) != 0x80) {
                        throw new std::exception("UTFDataFormatException()"); 
                    }
                    str.push_back((wchar_t)(((c & 0x1F) << 6) | (char2 & 0x3F)));
                    break;
                 case 14:
                    /* 1110 xxxx  10xx xxxx  10xx xxxx */
                    count += 3;
                    if (count > size) {
                        throw new std::exception("UTFDataFormatException()");
                    }
                    char2 = (int) bytearr[count-2];
                    char3 = (int) bytearr[count-1];
                    if (((char2 & 0xC0) != 0x80) || ((char3 & 0xC0) != 0x80)) {
                        throw new std::exception("UTFDataFormatException()");
                    }
                    str.push_back((wchar_t)(
                        ((c     & 0x0F) << 12) |
                        ((char2 & 0x3F) << 6)  |
                        ((char3 & 0x3F) << 0)
                    ));
                    break;
                 default:
                    /* 10xx xxxx,  1111 xxxx */
                     throw new std::exception("UTFDataFormatException()");
            }
        }
        return str;    
    }


    bool is_open() {
        return stream.is_open();
    }

    bool fail() {
        return stream.fail();
    }

    bool eof() {
        return stream.eof();
    }

private:
    stream_type stream;
    size_t position;
};

class nme_input_file {

public:
    void read_header() {

        if(is_header_read) {
            return;
        }
        if(!file.is_open()) {
            return;
        }
        try {
            //file_version = dis_.readInt() - 0x27bc86aa;
            file >> file_version; 
            file_version -= 0x27bc86aa;

            if(file_version < 0) {
                //log_.logError("NMEInputFileMsg2");
                close();
                return;
            }
            if(file_version > 1) {
                //log_.logError("NMEInputFileMsg3", new Integer(fileVersion_));
                close();
                return;
            }
            int i; // = dis_.readInt();
            file >> i;
            meta_types.resize(i);
            meta_names.resize(i);
            for(int j = 0; j < i; j++) {
                file >> meta_types[j];// = dis_.readInt();
                file >> meta_names[j];// = dis_.readUTF();
            }

            file >> i;
            if(i != 0x27bc86aa) {
                //System.out.println("Bad magic number in data file " + fname_);
                close();
                return;
            }
            if(file_version != 1) {
                translate_types();
            }
            //getAttributeIndexes();
            is_header_read = true;
        }
        catch(std::exception exc) {
            //log_.logException(Level.ERROR, "NMEInputFileMsg4", fname_, exception);
            close();
        }
    }


    bool next(std::vector<boost::any>& nme) {
        if(nme.size() != meta_types.size()) {
            throw std::exception("Invalid destination NME size"); //TODO MaximV: exception
        }
        if(file.eof()) {
            return false;
        }
        bool flag = true;
        int int_val;
        __int64 long_val;
        float float_val;
        double double_val;
        //std::string s_val;
        std::wstring w_val;
        record_count++;
        //try {
            for(size_t i = 0; i < meta_types.size(); i++) {
                switch(meta_types[i]) {
                case 1: // '\001'
                    file >> int_val;
                    nme[i] = int_val;
                    break;

                case 2: // '\002'
                    file >> w_val;
                    nme[i] = w_val;
                    break;

                case 3: // '\003'
                    file >> long_val;
                    nme[i] = long_val;
                    break;

                case 4: // '\004'
                    file >> float_val;
                    nme[i] = float_val;
                    break;

                case 5: // '\005'
                    file >> double_val;
                    nme[i] = double_val;
                    break;

                case 7: // '\007'
                    file >> w_val;
                    nme[i] = w_val;
                    break;

                case 6: // '\006'
                    {
                        int size;
                        file >> size;
                        std::vector<unsigned char> bytes(size);
                        file >> bytes;
                        //if(size > 0) {
                        //    file.read((char*)(&bytes[0]), size);
                        //    //dis_.read(abyte1, 0, i2);
                        //}
                        break;
                    }
                default:
                    break;
                }
            }

            file >> int_val;
            if(int_val != 0x27bc86aa)
            {
                //log_.logError("NMEInputFileMsg6", new Object[] {
                //    fname_, new Integer(records_)
                //});
                
                //flag = false;
                //close();
                throw std::exception("NMERecord end marker != 0x27bc86aa");
            }
        //}
        //catch(std::exception err)
        //{
        //    throw;
        //    //flag = false;
        //    //close();
        //}
        //catch(IOException ioexception)
        //{
        //    log_.logException(Level.ERROR, "NMEInputFileMsg7", fname_, ioexception);
        //    flag = false;
        //    close();
        //}
        return flag;
    }

    void open(const std::string& file_name){
        try {
            fname = file_name;
            record_count = 0;
            file_version = -1;
            file.open(fname.c_str());
            if(file.fail()) {
                throw std::exception();
            }
            is_header_read = false;
            read_header();
        }
        catch(std::exception ioexception) {
            //log_.logException(Level.ERROR, "NMEInputFileMsg1", s, ioexception);
            throw;
        }
    }

    void close() {
        try {
            if(file.is_open()) {
                file.close();
            }
            //if(bis_ != null) {
            //    bis_.close();
            //    bis_ = null;
            //}
            //if(fis_ != null) {
            //    fis_.close();
            //    fis_ = null;
            //}
        }
        catch(std::exception ioexception) {
            //log_.logException(Level.ERROR, "NMEInputFileMsg5", fname_, ioexception);
            throw;
        }
    }

    const std::vector<int>& get_meta_types() {
        return meta_types;
    }

    const std::vector<std::wstring>& get_meta_names() {
        return meta_names;
    }

private:

    void translate_types() {
        for(size_t i=0; i<meta_types.size(); ++i) {
            if(meta_types[i] == 0) {
                meta_types[i] = 2;
            } else {
                meta_types[i] = 1;
            }
        }
    }


private:
    //private FileInputStream fis_;
    //private BufferedInputStream bis_;
    //private DataInputStream dis_;
    //std::ifstream file;
    input_stream file;
    std::string fname;
    int file_version;
    std::vector<int> meta_types;
    //std::vector<int> attr_indexes;
    std::vector<std::wstring> meta_names;
    bool is_header_read;
    int record_count;
};

}

#endif