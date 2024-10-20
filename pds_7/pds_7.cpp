#include <iostream>
#include <fstream>

using namespace std;

void readGraph(const string& filename, int& n, int& m, int**& edges) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }

    inputFile >> n >> m;

    edges = new int* [m];
    for (int i = 0; i < m; ++i) {
        edges[i] = new int[2];
        inputFile >> edges[i][0] >> edges[i][1];
    }

    inputFile.close();
}

int** createAdjacencyMatrix(int n, int m, int** edges) {
    int** adjacencyMatrix = new int* [n];
    for (int i = 0; i < n; ++i) {
        adjacencyMatrix[i] = new int[n]();
    }

    for (int i = 0; i < m; ++i) {
        int v = edges[i][0] - 1;
        int u = edges[i][1] - 1;
        adjacencyMatrix[v][u] = 1;
    }

    return adjacencyMatrix;
}

void calculateDegrees(int** adjacencyMatrix, int n, int* inDegree, int* outDegree) {
    for (int i = 0; i < n; ++i) {
        inDegree[i] = 0;
        outDegree[i] = 0;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (adjacencyMatrix[i][j] == 1) {
                outDegree[i]++;
                inDegree[j]++;
            }
        }
    }
}

bool isHomogeneous(int* inDegree, int* outDegree, int n) {
    int degree = inDegree[0] + outDegree[0];
    for (int i = 1; i < n; ++i) {
        if (inDegree[i] + outDegree[i] != degree) {
            return false;
        }
    }
    return true;
}

void findIsolatedAndHangingVertices(int* inDegree, int* outDegree, int n, int*& isolated, int*& hanging, int& isolatedCount, int& hangingCount) {
    isolatedCount = 0;
    hangingCount = 0;

    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == 0 && outDegree[i] == 0) {
            isolated[isolatedCount++] = i + 1;
        }
        else if (inDegree[i] + outDegree[i] == 1) {
            hanging[hangingCount++] = i + 1;
        }
    }
}

void printDegrees(int* inDegree, int* outDegree, int n, const string& outputFile) {
    ofstream outFile(outputFile, ios::app);
    cout << "Stepeni vershyn:\n";
    outFile << "Stepeni vershyn:\n";
    for (int i = 0; i < n; ++i) {
        cout << "Vershyna " << i + 1 << ": In-degree = " << inDegree[i] << ", Out-degree = " << outDegree[i] << endl;
        outFile << "Vershyna " << i + 1 << ": In-degree = " << inDegree[i] << ", Out-degree = " << outDegree[i] << endl;
    }
    cout << endl;
    outFile << endl;
    outFile.close();
}

void printIsolatedAndHangingVertices(int* isolated, int isolatedCount, int* hanging, int hangingCount, const string& outputFile) {
    ofstream outFile(outputFile, ios::app);

    cout << "Izoliovani vershyny: ";
    outFile << "Izoliovani vershyny: ";
    for (int i = 0; i < isolatedCount; ++i) {
        cout << isolated[i];
        outFile << isolated[i];
        if (i < isolatedCount - 1) {
            cout << ", ";
            outFile << ", ";
        }
    }
    cout << endl;
    outFile << endl;

    cout << "Vysiachi vershyny: ";
    outFile << "Vysiachi vershyny: ";
    for (int i = 0; i < hangingCount; ++i) {
        cout << hanging[i];
        outFile << hanging[i];
        if (i < hangingCount - 1) {
            cout << ", ";
            outFile << ", ";
        }
    }
    cout << endl;
    outFile << endl;

    outFile.close();
}

int main() {
    int n, m;
    int** edges = nullptr;
    string inputFile, outputFile;

    cout << "Enter input file: ";
    cin >> inputFile;
    cout << "Enter output file: ";
    cin >> outputFile;

    readGraph(inputFile, n, m, edges);

    int** adjacencyMatrix = createAdjacencyMatrix(n, m, edges);

    int* inDegree = new int[n];
    int* outDegree = new int[n];
    calculateDegrees(adjacencyMatrix, n, inDegree, outDegree);
    printDegrees(inDegree, outDegree, n, outputFile);

    if (isHomogeneous(inDegree, outDegree, n)) {
        cout << "Graph is odnoridnyj. Degree of odnoridnosti: " << inDegree[0] + outDegree[0] << endl;
        ofstream outFile(outputFile, ios::app);
        outFile << "Graph is odnoridnyj. Degree of odnoridnosti: " << inDegree[0] + outDegree[0] << endl;
        outFile.close();
    }
    else {
        cout << "Graph is not odnoridnyj." << endl;
        ofstream outFile(outputFile, ios::app);
        outFile << "Graph is not odnoridnyj." << endl;
        outFile.close();
    }

    int* isolated = new int[n];
    int* hanging = new int[n];
    int isolatedCount = 0, hangingCount = 0;
    findIsolatedAndHangingVertices(inDegree, outDegree, n, isolated, hanging, isolatedCount, hangingCount);
    printIsolatedAndHangingVertices(isolated, isolatedCount, hanging, hangingCount, outputFile);

    for (int i = 0; i < m; ++i) {
        delete[] edges[i];
    }
    delete[] edges;

    for (int i = 0; i < n; ++i) {
        delete[] adjacencyMatrix[i];
    }
    delete[] adjacencyMatrix;

    delete[] inDegree;
    delete[] outDegree;
    delete[] isolated;
    delete[] hanging;

    return 0;
}