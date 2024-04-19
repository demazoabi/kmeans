import numpy as np
import sys
def k_means(filePath,k,n,d,iterNum=200,epsilon=0.001):
    try :
        data=readFile(filePath,d,n);
        iter=0;
        if(k<1 or k>n or not isinstance(k, int)):
            print("Invalid number of clusters!");
            return 1;
        if (n<=1 or not isinstance(n, int)):
            print("Invalid number of points!");
            return 1;
        if (d<=0 or not isinstance(d, int)):
            print("Invalid dimension of point!");
            return 1;
        if(iterNum<1 or iterNum>1000 or not isinstance(iterNum, int)):
            print("Invalid maximum iteration!");
            return 1;
        centroids=data[:k];
        while iter<iterNum:
            newCentroids=[];
            clusters =[[] for i in range(k)];
            for point in data:
                minDist=np.inf;
                for i in range(k):
                    distance=euclideanDistance(point,centroids[i]);
                    if (distance<minDist):
                        minDist=distance;
                        closeC=i;
                clusters[closeC].append(point);
            for cluster in clusters:
                newCentroids.append(np.mean(cluster,axis=0));
            e=0;
            for c in range(k):
                e+=euclideanDistance(newCentroids[c],centroids[c]);
            if(e<epsilon):
                break;
            centroids=newCentroids;
            iter+=1;
            
        formattedCentroids = [[f'%.4f' % float(val) for val in sublist] for sublist in centroids];
        returnValue = [','.join(sublist) for sublist in formattedCentroids];
        for value in returnValue :
            print(value,'\n');
        return 0;
    except Exception as e:
        print("An Error Has Occurred");
        return 1;


def euclideanDistance(x,y): 
    result=np.sqrt(np.sum((x-y)**2));
    return result;

def readFile(filePath, d, n):
    Fdata = []
    try:
        with open(filePath, 'r') as file:
            for line in file:
                values = line.strip().split(",")
                point = []
                for value in values:
                    point.append(float(value))
                if len(point) != d:
                    print("Error: Dimension of point doesn't match specified value (d).")
                    return None
                Fdata.append(point)
            if len(Fdata) < n:
                print("Error: Number of points in file is less than specified value (n).")
                return None
            if len(Fdata) > n:
                print("Warning: Number of points in file is greater than specified value (n).")
                Fdata = Fdata[:n]
        return np.array(Fdata)
    except FileNotFoundError:
        print(f"Error: File '{filePath}' not found.")
        return None;
    except Exception as e:
        print(f"An error occurred while reading the file: {e}")
        return None;

def main():
    if len(sys.argv) >= 5:
        k = int(sys.argv[1])
        n = int(sys.argv[2])
        d = int(sys.argv[3])
        if len(sys.argv) >= 6:
            filePath = sys.argv[5]
            iterNum = int(sys.argv[4])
        else:
            filePath = sys.argv[4]
            iterNum=200
    else:
        input_values = input("Enter k n d maximum iteration and file path separated by spaces: ").split();
        k, n, d = map(int, input_values[:3]);
        iterNum = int(input_values[3]) if len(input_values) > 4 else 200;
        filePath=input_values[-1];
    result=k_means(filePath,k,n,d,iterNum);

if __name__ == "__main__":
    main()  