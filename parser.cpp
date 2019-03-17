#include <iostream>
#include <string>
#include <fstream>

std::string find_node_id(const std::string &year, const std::string &filename);
void partition_edges(const int upper_node_id, const int lower_node_id, const std::string &filename);

int main(int argc, char const *argv[])
{
   if(argc != 5)
   {
      std::cerr << "Expected 5 argument arguments: received: " << argc << std::endl;
      std::cerr << "Provide arguments in following order: " << 
         "1) ./executable "
         "2) Edge list file name " 
         "3) Node info file name "
         "4) Input year (lower) to partition"
         "5) Input year (upper) to partition"
         << std::endl;
      exit(EXIT_FAILURE);
   }

   std::string edge_filename(argv[1]);
   std::string node_info_filename(argv[2]);
   std::string year_upper(argv[3]);
   std::string year_lower(argv[4]);

   std::cout << "---------------INPUT PARAMETERS-------------------------" << std::endl;
   std::cout << "Connectivity input file name: " << edge_filename << std::endl;
   std::cout << "Nodal info input file name: " << node_info_filename << std::endl;
   std::cout << "Year lower input: " << year_lower << std::endl;
   std::cout << "Year upper input: " << year_upper << std::endl;
   std::cout << "--------------------------------------------------------" << std::endl;

   auto upper_node_id = find_node_id(year_upper, node_info_filename);
   auto lower_node_id = find_node_id(year_lower, node_info_filename);

   partition_edges(std::stoi(lower_node_id), std::stoi(upper_node_id), edge_filename);

   return 0;
}

//function for partioning the provide edge list file into train and test data based on node_id
void partition_edges(const int low_bnd, const int up_bnd, const std::string &filename)
{
   const std::string trainfilename("train.dat");
   const std::string test_filename("test.dat");

   std::cout << "Partioning edges into train and test.." << std::endl;

   std::ifstream infile;
   infile.open(filename);

   if(!infile)
   {
      std::cerr << "Unable to open " << filename << std::endl;
      std::cerr << "Exiting.." << std::endl;
      exit(EXIT_FAILURE);
   }

   std::string str;
   bool found_numeric = false;
   int i = 0;
   while(!found_numeric)
   {
      std::getline(infile, str, '\n');

      if(!str.find("#")) continue;     //skips the header lines begining with a #, modify later
      else found_numeric = true;
   }

   auto u = std::stoi(str.substr(0, str.find('\t')));     //cit-Patents.txt uses tabs -.-
   auto v = std::stoi(str.substr(str.find('\t')));

   std::ofstream trainfile("train.dat");
   std::ofstream testfile("test.dat");

   if(u < up_bnd && v < up_bnd && u >= low_bnd && v >= low_bnd) trainfile << u << " " << v << std::endl;
   else if(u >= up_bnd && v >= up_bnd) testfile << u << " " << v << std::endl; 

   while(infile >> u >> v)
   {
      if(u < up_bnd && v < up_bnd && u >= low_bnd && v >= low_bnd) trainfile << u << " " << v << std::endl;
      else if(u >= up_bnd && v >= up_bnd) testfile << u << " " << v << std::endl; 
   }

   trainfile.close();
   testfile.close();
   infile.close();

   std::cout << "Partitioning done." << std::endl;
}

std::string find_node_id(const std::string &year, const std::string &filename)
{
   std::cout << "Finding first node id corresponding to input year" << std::endl;

   std::ifstream infile(filename);

   if(!infile)
   {
      std::cerr << "Unable to open " << filename << std::endl;
      std::cerr << "Exiting.." << std::endl;
      exit(EXIT_FAILURE);
   }

   std::string node_id = "-1";
   std::string str;

   int i = 0;

   while(std::getline(infile, str, '\n') && node_id == "-1")
   {
      if(!str.find("\"")) continue;     //skips the header lines begining with a quote, modify later

      auto pos = str.find(',');

      if(!str.substr(pos + 1).find(year))   //find node_id corresponding to provided year
      {
         node_id = str.substr(0,pos);
      }
   }

   infile.close();

   if(node_id == "-1")
   {
      std::cerr << "Year not found! Exiting.." << std::endl;
      exit(EXIT_FAILURE);
   }

   std::cout << "Found first node id corresponding to year " << year << ": " << node_id << std::endl;

   return node_id;
}
