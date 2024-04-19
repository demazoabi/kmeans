#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#ifndef INFINITY 
#define INFINITY (__builtin_inf())
#endif

int k_means(char *filename, int k, int n, int d, int maxIter, double epsilon);

double** readFile(char *filePath, int d, int n);
double euclideanDistance(double *x, double *y, int d);
double* calMean(double **cluster, int num, int d);

int main(int argc, char *argv[]) {
    int k, n, d, maxIter=200;
    double epsilon=0.001;
    char filename[FILENAME_MAX];
    int result;
    if (argc >= 5) {
        sscanf(argv[1], "%d", &k);
        sscanf(argv[2], "%d", &n);
        sscanf(argv[3], "%d", &d);
        if (argc >=6) {
            sscanf(argv[4], "%d", &maxIter);
            sscanf(argv[5], "%s", filename);
        } else {
            maxIter = 200; 
            sscanf(argv[4], "%s", filename);
        }
    } 
    else {
        char maxIterInput[CHAR_MAX];
        printf("Enter filename, k, n, d: ");
        scanf("\"%99[^\"]\" %d %d %d", filename, &k, &n, &d);
        while (getchar() != '\n');
        printf("Enter max iteration: ");
        fgets(maxIterInput, sizeof(maxIterInput), stdin);
        if (maxIterInput[0] != '\n') {
            sscanf(maxIterInput,"%d", &maxIter);
        }
    }
    if ((int) k < 1 || (int) k>=n) {
        printf("Invalid number of clusters!\n");
        return 1;
    }
    if (n <= 1) {
        printf("Invalid number of points!\n");
        return 1;
    }
    if (d <= 0) {
        printf("Invalid dimension of point!\n");
        return 1;
    }
    if (maxIter <= 0 || maxIter >= 1000) {
        printf("Invalid maximum iteration!\n");
        return 1;
    }
    result =k_means(filename, k, n, d, maxIter, epsilon);
    return result;
}

int k_means(char *filename, int k, int n, int d, int maxIter, double epsilon){
    double **data=readFile(filename,d,n);
    double **centroids = (double **)malloc(k * sizeof(double *));
    int i,j,i1,b,z;
    int iter=0;
    if (data == NULL) {
        printf("An Error Has Occurred\n");
        return 1;
    }
    for (i = 0; i < k; i++) {
        centroids[i] = (double *)malloc(d* sizeof(double));
        for (j = 0; j < d; j++) {
            centroids[i][j] = data[i][j];
        }
    }
    while (iter < maxIter) {
        double **newCentroids = (double **)malloc(k * sizeof(double *));
        double ***clusters = (double ***)malloc(k * sizeof(double **));
        int *PointsInCluster = (int *)calloc(k, sizeof(int));
        int u,r,g,f,s,a,t;
        double e=0;
        for (u = 0; u < k; u++) {
            newCentroids[u] = (double *)malloc(d * sizeof(double));
        }
        for (r = 0; r < k; r++) {
            int l;
            clusters[r] = (double **)malloc(n * sizeof(double *));  
            for (l = 0; l < n; l++) {
                clusters[r][l]=(double *)malloc(d * sizeof(double));  
                }
            }
        for (g= 0; g< n; g++) {
            double minDist = INFINITY;
            int closestC;
            int w,ind;
            e=0;
            for (w = 0; w < k; w++) {
                double distance = euclideanDistance(data[g], centroids[w], d);
                if (distance < minDist) {
                    minDist = distance;
                    closestC = w;
                }
            }
            for(ind=0;ind<d;ind++){
                clusters[closestC][PointsInCluster[closestC]][ind] = data[g][ind];
            }
             PointsInCluster[closestC]++;
        }
        for (f = 0; f< k; f++) {
            double *mean=calMean(clusters[f],PointsInCluster[f], d);
            int m;
            for(m=0;m<d;m++){
                newCentroids[f][m]=mean[m];
        }
        }
        for (s = 0; s< k; s++) {
            e += euclideanDistance(newCentroids[s], centroids[s], d);
        }
        if (e < epsilon) {
            break;
        }
        for (a = 0; a < k; a++) {
            int v;
            for (v = 0; v < d; v++) {
                centroids[a][v] = newCentroids[a][v];
            }
        }
        for (t= 0; t< k; t++) {
            int h;
            free(newCentroids[t]);
            for (h = 0; h< PointsInCluster[t]; h++) {
                free(clusters[t][h]);
            }
            free(clusters[t]);
        }
        free(newCentroids);
        free(clusters);
        free(PointsInCluster);
        iter++;
    }
    for (i1 = 0; i1< k; i1++) {
        int j1;
        for (j1 = 0; j1< d; j1++) {
            printf("%.4f ", centroids[i1][j1]);
        }
        printf("\n");
    }
    for (b= 0; b < n; b++) {
        free(data[b]);
    }
    free(data);
    for (z = 0; z < k; z++) {
        free(centroids[z]);
    }
    free(centroids);
    return 0;
}

double** readFile(char *filePath, int d, int n) {
    double **data = (double **)malloc(n * sizeof(double *));
    int q;
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        free(data);
        return NULL;
    }
    for(q = 0; q < n; q++){
        double *row = (double *)malloc(d * sizeof(double));
        char newline;
        int p,s,z;
        if (row == NULL) {
            fclose(file);
            for (p = 0; p < q; p++) {
                free(data[p]);
            }
            free(data);
            return NULL; 
        }
        for (s = 0; s < d; s++) {
            int t;
            if (fscanf(file, "%lf,", &row[s]) != 1) {
                fclose(file);
                for (t= 0; t<= q; t++) {
                    free(data[t]);
                 }
                free(data);
                free(row);
                return NULL; 
            }
        }
        if (fscanf(file, "%c", &newline) != 1 || newline != '\n') {
            fclose(file);
            for (z = 0; z <= q; z++) {
                free(data[z]);
            }
            free(data);
            free(row);
            return NULL; 
        }

        data[q] = row;
    }

    fclose(file);
    return data;
} 
double euclideanDistance(double *x, double *y, int d) {
    double sum = 0.0;
    int i;
    for (i = 0; i < d; i++) {
        sum =sum +((x[i] - y[i]) * (x[i] - y[i]));
    }
    return sqrt(sum);
}

double* calMean(double **cluster, int num, int d) {
    double *mean = (double *)malloc(d * sizeof(double));
    int i,p,y;
    for (i = 0; i < d; i++) {
        mean[i] = 0.0;
    }
    for (p = 0; p < num; p++) {
        int j;
        for (j = 0; j < d; j++) {
            mean[j] += cluster[p][j];
        }
    }
    for(y = 0;y< d;y++) {
        mean[y] /= num;
    }
    return mean;
}