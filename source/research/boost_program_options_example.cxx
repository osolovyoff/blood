#pragma once
#include <iostream>
#include <string>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

#include "reginfo.hxx"

int main(int ac, char* av[])
{
    boost::program_options::options_description general_desc("\r\nUsage");
    try
    {
        general_desc.add_options()
            ("help,h", "produce a help message")
            ("scan,s", boost::program_options::value<std::string>()->implicit_value(""), "just the system registry scanning")
            ("user,u", "get usernames from the system")
            ("auto,a", "get autorun variables from the system")
            ("diff,d", "options for Difference two file");

        boost::program_options::options_description difference_desc("Difference");
        difference_desc.add_options()
            ("file_1,1", boost::program_options::value<std::string>(), "first file for a difference")
            ("file_2,2", boost::program_options::value<std::string>(), "second file for a difference")
            ("match,m", boost::program_options::value<std::string>()->implicit_value("match.txt"), "output mismatch name");
        general_desc.add(difference_desc);

        boost::program_options::variables_map vm;
        boost::program_options::parsed_options parsed = boost::program_options::command_line_parser(ac, av).options(general_desc).allow_unregistered().run();
        boost::program_options::store(parsed, vm);
        boost::program_options::notify(vm);

        bool one_flag_has_been_run = false;

        if (vm.count("help"))
        {
            one_flag_has_been_run = true;
            std::cout << general_desc;
            
        }

        if (vm.count("scan"))
        {
            one_flag_has_been_run = true;
            const std::string filepath = vm["scan"].as<std::string>();
            RegInfo::get_all_keys(filepath);
        }

        if (vm.count("user"))
        {
            one_flag_has_been_run = true;
            const std::string filepath = vm["user"].as<std::string>();
            RegInfo::get_profile_list();
        }

        if (vm.count("auto"))
        {
            one_flag_has_been_run = true;
            const std::string filepath = vm["auto"].as<std::string>();
            RegInfo::get_autorun();
        }

        if (!one_flag_has_been_run)
        {
            std::cout << general_desc;
        }

        if (vm.count("diff"))
        {
            one_flag_has_been_run = true;
            const std::string file_1 = vm["file_1"].as<std::string>();
            const std::string file_2 = vm["file_2"].as<std::string>();
            const std::string match = vm["match"].as<std::string>();
            RegInfo::mismatch(file_1, file_2, match);
        }

       

    }
    catch (boost::program_options::error& e)
    {
        std::cout << std::endl << "error: " << e.what() << std::endl;
    }

    return 0;
}

