#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char const *argv[])
{
   //argv should be size 2 consisting of the file names of the connectivity list and the nodal info, respectively
   if(argc != 3)
   {
      std::cerr << "Expected 3 argument arguments: received: " << argc << std::endl;
      std::cerr << "Provide arguments in following order: " << std::endl;
      exit(EXIT_FAILURE);
   }

   std::string conn_file_name(argv[1]);
   std::string node_file_name(argv[2]);

   std::cout << "Connectivity input file name: " << conn_file_name << std::endl;
   std::cout << "Nodal info input file name " << node_file_name << std::endl;

   std::ifstream inFile;

   inFile.open(conn_file_name);

   if(!inFile)
   {
      std::cerr << "Unable to open " << conn_file_name << std::endl;
      exit(EXIT_FAILURE);
   }

   int u, v;    //edge defined as (u,v), i.e., u cites v
   std::string str;
   std::string year = "1964";
   std::string node_id = "-1";

   while(!inFile.eof() && node_id == "-1")
   {
      std::getline(inFile, str, '\n');

      if(!str.find("\"")) continue;     //skips the header lines begining with a quote, modify later

      auto pos = str.find(',');

      if(!str.substr(pos + 1).find(year))   //find node_id corresponding to provided year
      {
         node_id = str.substr(0,pos);
         std::cout << node_id << std::endl;
      }
   }
   inFile.close();

   if(node_id == "-1")
   {
      std::cout << "Year not found!" << std::endl;
      exit(EXIT_FAILURE);
   }

   return 0;
}

