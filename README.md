# EECS598_advanced_data_mining_project
A collaboration repo for the EECS598 - Advanced Data Mining project.

This branch contains the preprocessing tools for the patent citation network.

The `mapper` execuable maps the existing node ids in the input file to 0...|V|-1
The `parser` excutable extacts a subgraph given the node and edge files, as well as upper and lower bounded years. It also contains capabillity to extract the future 500 node set.
