#include <iostream>
#include <unordered_map>
#include <fstream>

// maps node ids so it starts at 0
int main(int argc, char const *argv[])
{
   std::ifstream infile;
   std::unordered_map<int,int> ht;
   std::ofstream outfile("mapped_output.dat");

   auto filename = argv[1];
   std::cout << "Input file name: " << filename << std::endl;
   infile.open(filename);

   if(!infile)
   {
      std::cerr << "Unable to open " << filename << std::endl;
      std::cerr << "Exiting.." << std::endl;
      exit(EXIT_FAILURE);
   }

   int u;
   int v;
   int mapped_id = 0;

   while(infile >> u >> v)
   {
      if(ht.find(u) == ht.end())
      {
         ht[u] = mapped_id;
         mapped_id++;
      }
      u = ht[u];
      if(ht.find(v) == ht.end())
      {
         ht[v] = mapped_id;
         mapped_id++;
      }
      v = ht[v];

      outfile << u << " " << v << std::endl;
   }

   infile.close();
   outfile.close();

   return 0;
}