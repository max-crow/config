// test_config.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "nme_input_file.h"


int print_config_struct(int argc, _TCHAR* argv[]);
int simple_read(int argc, _TCHAR* argv[]);


int _tmain(int argc, _TCHAR* argv[])
{
    //std::wstring config_name(argv[0]);
    //config_name += L".cfg";
    //config::config_ptr cfg_reader = config::xml_config_reader::load_from_file(config_name);

    //std::fstream f("d:\\temp\\1.txt", std::ios_base::out, std::ios_base::binary);
    //bool b = f.fail();
    //b = f.is_open();
    //f << -1;
    //b = f.fail();
    //f.close();

    siu::nme_input_file file;
    file.open("d:\\temp\\cdr.cdr");
    std::vector<boost::any> nme(file.get_meta_types().size());
    while(file.next(nme)) {
        int i = boost::any_cast<int>(nme[0]);
        i = boost::any_cast<int>(nme[1]);
        i = boost::any_cast<int>(nme[2]);
        std::wstring str = boost::any_cast<std::wstring>(nme[3]);
    }

    simple_read(argc, argv);
    print_config_struct(argc, argv);
    return 0;
}

