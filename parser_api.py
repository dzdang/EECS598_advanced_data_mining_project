import os
import numpy as np
import pandas as pd
import time

executable_name = "parser"
node_filename = "apat63_99.txt"
edge_filename = "cit-Patents.txt"
year2partition = "1990"
path_to_executable = "/Users/dzdang/Desktop/eecs598/project"

os.system('cd {0}'.format(path_to_executable))
# cmd = "parser {0} {1} {2}".format(node_filename, edge_filename, year2partition)
os.system("./parser {0} {1} {2}".format(edge_filename, node_filename, year2partition))

#write stuff for loading train and test data into python
# start = time.time()
# train_adj_list_np = np.loadtxt("train.dat")
# end = time.time()
# print("Numpy load time: ", end - start)
# start = time.time()
train_adj_list_pd = pd.read_csv("train.dat",' ', header=None)      #pandas appears to be much faster
# end = time.time()
# print("Pandas load time: ", end - start)

#convert pandas dataframe to numpy array
train_adj_list_pd = train_adj_list_pd.values


def load_adj_list(filename):
   pass
