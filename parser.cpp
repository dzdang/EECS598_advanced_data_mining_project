#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <queue>

std::string find_node_id(const std::string &year, const std::string &filename,
   std::queue<std::string> &q, bool flag = false);
std::multimap<int,int> partition_edges(const int upper_node_id, const int lower_node_id, 
   const std::string &filename);
void put_next_500(std::queue<std::string> &q, size_t low_bnd, size_t up_bnd, 
   const std::multimap<int,int> &future);

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
   std::string year_lower(argv[3]);
   std::string year_upper(argv[4]);

   std::cout << "---------------INPUT PARAMETERS-------------------------" << std::endl;
   std::cout << "Connectivity input file name: " << edge_filename << std::endl;
   std::cout << "Nodal info input file name: " << node_info_filename << std::endl;
   std::cout << "Year lower input: " << year_lower << std::endl;
   std::cout << "Year upper input: " << year_upper << std::endl;
   std::cout << "--------------------------------------------------------" << std::endl;

   std::queue<std::string> q;
   auto lower_node_id = find_node_id(year_lower, node_info_filename, q);
   auto upper_node_id = find_node_id(year_upper, node_info_filename, q, true);
   std::cout << q.size() << std::endl;

   auto future = partition_edges(std::stoi(lower_node_id), std::stoi(upper_node_id), edge_filename);

   std::cout << future.size() << std::endl;

   put_next_500(q, std::stoi(lower_node_id), std::stoi(upper_node_id), future);

   return 0;
}

void put_next_500(std::queue<std::string> &q, size_t low_bnd, size_t up_bnd, 
   const std::multimap<int,int> &future)
{
   std::ofstream trainfile("train.dat", std::ios_base::app);
   if(!trainfile)
   {
      std::cerr << "Unable to open " << "train.dat" << std::endl;
      std::cerr << "Exiting.." << std::endl;
      exit(EXIT_FAILURE);
   }

   std::string u, v;
   //u is citing and v is cited
   //we need to check if the front of the q exists in the first column. If it doesn't exist
   //then we throw it out. If it exists, then we need to check if the value is < up_bnd. If it's not
   //we throw it out
   //Otherwise we
   //add this popped value to our hash table upt o date. The trick is there can be multiple key-value
   //pairs with the same key
   size_t counter = 0;
   size_t popped;
   std::unordered_set<size_t> future500;
   while(!q.empty() && counter < 500)
   {
      popped = std::stoi(q.front());
      q.pop();

      if(future.find(popped) != future.end())
      {
         auto my_pair = future.equal_range(popped);

         if(std::distance(my_pair.first, my_pair.second) >= 2)//we only want nodes that have 2 or more edges
         {  
            auto not_found = true;

            for(auto it = my_pair.first; it != my_pair.second; it++)
            {
               if(it->second < up_bnd && future500.find(it->second) == future500.end()){}
               else not_found = false;
            }

            if(not_found)
            {
               for(auto it = my_pair.first; it != my_pair.second; it++)
               {
                  future500.emplace(popped);
                  trainfile << it->first << " " << it->second << std::endl;
               }            
               counter++;
            }
         }
      }
      // else std::cout << "popped not found; this shouldn't happen; node id is: " << popped << std::endl;

   }
}

//function for partioning the provide edge list file into train and test data based on node_id
std::multimap<int,int> partition_edges(const int low_bnd, const int up_bnd, const std::string &filename)
{
   const std::string trainfilename("train.dat");
   // const std::string test_filename("test.dat");
   std::multimap<int,int> future;

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
   else if(u >= up_bnd && v >= low_bnd) future.emplace(u,v); 

   while(infile >> u >> v)
   {
      if(u < up_bnd && v < up_bnd && u >= low_bnd && v >= low_bnd) trainfile << u << " " << v << std::endl;
      else if(u >= up_bnd && v >= low_bnd) future.emplace(u,v); 
   }

   trainfile.close();
   // testfile.close();
   infile.close();

   std::cout << "Partitioning done." << std::endl;

   return future; 
}

std::string find_node_id(const std::string &year, const std::string &filename, 
   std::queue<std::string> &q, bool flag)
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

   while(node_id == "-1" && std::getline(infile, str, '\n'))
   {
      if(!str.find("\"")) continue;     //skips the header lines begining with a quote, modify later

      auto pos = str.find(',');

      if(!str.substr(pos + 1).find(year))   //find node_id corresponding to provided year
      {
         node_id = str.substr(0,pos);
      }
   }

   //now store the next nodes for future link prediction
   if(flag)
   {
      q.emplace(node_id);         
      while(std::getline(infile, str, '\n'))
      {
         if(!str.find("\"")) continue;     //skips the header lines begining with a quote, modify later

         auto pos = str.find(',');
         pos = std::min(pos, str.find(' '));
         pos = std::min(pos, str.find('\t'));
         q.emplace(str.substr(0, pos));
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
