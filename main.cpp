#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char const *argv[])
{
   //argv should be size 2 consisting of the file names of the connectivity list and the nodal info, respectively
   if(argc != 3)
   {
      std::cerr << "Expected 3 argument arguments: received: " << argc << std::endl;
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

   // std::string u, v;    //edge defined as (u,v), i.e., u cites v
   int u, v;    //edge defined as (u,v), i.e., u cites v
   inFile.ignore(1000,'\n');

   while(inFile >> u >> v)
   {
      std::cout << u << v << std::endl;
   }

   return 0;
}

