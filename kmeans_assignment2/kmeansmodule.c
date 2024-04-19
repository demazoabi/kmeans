#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int k_means_impl(double **initial_centroids, double **combined_data,int k, int n, int d, int maxIter, double epsilon);
double** readFile(char *filePath, int d, int n);
double euclideanDistance(double *x, double *y, int d);
double* calMean(double **cluster, int num, int d);
static PyObject* fit(PyObject* self, PyObject* args);
static PyMethodDef kmeans_methods[];


static struct PyModuleDef kmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp", 
    NULL, 
    -1,
    kmeans_methods 
};

PyMODINIT_FUNC PyInit_mykmeanssp(void) {
    return PyModule_Create(&kmeansmodule);
}


static PyObject* fit(PyObject* self, PyObject* args) {
    PyObject *initial_centroids_obj;
    PyObject *key_values_obj;  
    int k, n, d, maxIter;
    double epsilon;

    if (!PyArg_ParseTuple(args, "OOiiiid", &initial_centroids_obj, &key_values_obj, &k, &n, &d,&maxIter, &epsilon)) {
        return NULL;
    }
    double **initial_centroids = (double **)malloc(k * sizeof(double *));
    if (initial_centroids == NULL) {
        printf("An Error Has Occurred\n");
        return NULL;
    }
    for (int i = 0; i < k; i++) {
        PyObject *centroid_list = PyList_GetItem(initial_centroids_obj, i);
        initial_centroids[i] = (double *)malloc(d * sizeof(double));
        if (initial_centroids[i] == NULL) {
            printf("An Error Has Occurred\n");
            for (int j = 0; j < i; j++) {
                free(initial_centroids[j]);
            }
            free(initial_centroids);
            return NULL;
        }
        for (int j = 0; j < d; j++) {
            PyObject *item = PyList_GetItem(centroid_list, j);
            initial_centroids[i][j] = PyFloat_AsDouble(item);
        }
    }
    double **key_values = (double **)malloc(n * sizeof(double *));
    if (key_values == NULL) {
        printf("An Error Has Occurred\n");
        for (int i = 0; i < k; i++) {
            free(initial_centroids[i]);
        }
        free(initial_centroids);
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        PyObject *row_obj = PyList_GetItem(key_values_obj, i);
        if (!PyList_Check(row_obj)) {
            printf("An Error Has Occurred\n");
            for (int j = 0; j < i; j++) {
                free(key_values[j]);
            }
            free(key_values);
            for (int j = 0; j < k; j++) {
                free(initial_centroids[j]);
            }
            free(initial_centroids);
            return NULL;
        }
        int cols = PyList_Size(row_obj);
        key_values[i] = (double *)malloc(cols * sizeof(double));
        if (key_values[i] == NULL) {
            printf("An Error Has Occurred\n");
            for (int j = 0; j < i; j++) {
                free(key_values[j]);
            }
            free(key_values);
            for (int j = 0; j < k; j++) {
                free(initial_centroids[j]);
            }
            free(initial_centroids);
            return NULL;
        }
        for (int j = 0; j < cols; j++) {
            PyObject *item = PyList_GetItem(row_obj, j);
            key_values[i][j] = PyFloat_AsDouble(item);
        }
    }
    k_means_impl(initial_centroids, key_values, k, n, d, maxIter, epsilon);
    return Py_None;
}

int k_means_impl(double **initial_centroids,double **combined_data, int k, int n, int d, int maxIter, double epsilon) {
    double **centroids = initial_centroids;
    int i1,b,z;
    int iter=0;
    if (combined_data == NULL) {
        printf("An Error Has Occurred\n");
        return 1;
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
            int closestC=0;
            int w,ind;
            e=0;
            for (w = 0; w < k; w++) {
                double distance = euclideanDistance(combined_data[g], centroids[w], d);
                if (distance < minDist) {
                    minDist = distance;
                    closestC = w;
                }
            }
            for(ind=0;ind<d;ind++){
                clusters[closestC][PointsInCluster[closestC]][ind] = combined_data[g][ind];
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
            if (j1 < d - 1) {
                printf(", ");
        }
        }
        printf("\n");
    }
    for (b= 0; b < n; b++) {
        free(combined_data[b]);
    }
    free(combined_data);
    for (z = 0; z < k; z++) {
        free(centroids[z]);
    }
    free(centroids);
    return 0;
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

static PyMethodDef kmeans_methods[] = {
    {"fit", fit, METH_VARARGS, "K-means algorithm"},
    {NULL, NULL, 0, NULL} 
};


