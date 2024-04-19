import argparse
import numpy as np
import pandas as pd
import sys
import mykmeanssp 
import importlib

importlib.reload(mykmeanssp)

def validate_arguments(K, max_iter,eps,N):
    if K <= 1 or K >= N:
        print("Invalid number of clusters!")
        sys.exit(1)
    if max_iter<= 1 or max_iter >= 1000:
        print ("Invalid maximum iteration!")
        sys.exit(1)
    if eps < 0:
        print("Invalid epsilon!")
        sys.exit(1)


def main():
    if len(sys.argv) <5:
        print("Usage: python3 kmeans_pp.py K max_iter eps file_name_1 file_name_2")
        sys.exit(1)
    if len(sys.argv) == 5:
        K = int(sys.argv[1])
        max_iter = 200
        eps = float(sys.argv[2])
        file_name_1 = sys.argv[3]
        file_name_2 = sys.argv[4]
    else:
        K = int(sys.argv[1])
        max_iter = int(sys.argv[2])
        eps = float(sys.argv[3])
        file_name_1 = sys.argv[4]
        file_name_2 = sys.argv[5]

    global N
    N=0
    with open(file_name_1) as f:
        num_lines = sum(1 for line in f)
        N += num_lines
    with open(file_name_2) as f:
        num_lines = sum(1 for line in f)
        N += num_lines

    validate_arguments(K, max_iter, eps,N)

    data_1 = pd.read_csv(file_name_1, header=None,delimiter=',')
    data_2 = pd.read_csv(file_name_2, header=None,delimiter=',')

    combined_data = pd.merge(data_1, data_2, on=0, how='inner')
    combined_data.sort_values(by=0, inplace=True) 

    key_values = combined_data.iloc[:, 1:].values.tolist()
    np.random.seed(0)
    initial_centroids_indices = np.random.choice(len(combined_data.values), K, replace=False)
    initial_centroids_indices.sort()
    initial_centroids = combined_data.iloc[initial_centroids_indices, 1:].values
    initial_centroids_list = initial_centroids.tolist()
    

    d = len(combined_data.iloc[0])-1
    indices_str = ', '.join(map(str, initial_centroids_indices))
    print(indices_str)
    mykmeanssp.fit(initial_centroids_list,key_values, K, len(key_values), d,max_iter, eps)

if __name__ == '__main__':
    main()
