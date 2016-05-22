#ifndef MULTIVERSO_C_API_H_
#define MULTIVERSO_C_API_H_

#if defined _WIN32
#define DllExport __declspec(dllexport)
#else
#define DllExport
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void* TableHandler;

DllExport void MV_Init(int* argc, char* argv[]);

DllExport void MV_ShutDown();

DllExport void MV_Barrier();

DllExport int MV_NumWorkers();

DllExport int  MV_WorkerId();

DllExport int  MV_ServerId();

// Array Table
DllExport void MV_NewArrayTable(int size, TableHandler* out);

DllExport void MV_GetArrayTable(TableHandler handler, float* data, int size);

DllExport void MV_AddArrayTable(TableHandler handler, float* data, int size);


// Matrix Table
DllExport void MV_NewMatrixTable(int num_row, int num_col, TableHandler* out);

DllExport void MV_GetMatrixTableAll(TableHandler handler, float* data, int size);

DllExport void MV_AddMatrixTableAll(TableHandler handler, float* data, int size);

DllExport void MV_GetMatrixTableByRows(TableHandler handler, int row_ids[],
                                        int row_ids_n, int num_col, float* data[]);

DllExport void MV_AddMatrixTableByRows(TableHandler handler, int row_ids[],
                                        int row_ids_n, int num_col, float* data[]);


// typedef void* ArrayWorkerFloat;
// typedef void* ArrayServerFloat;
// struct AddOption {
//   float learning_rate;
//   float momentum;
//   float rho;
// };
// ArrayServerFloat newArrayServerFloat(int);
// ArrayWorkerFloat newArrayWorkerFloat(int);
// void getArrayWorkerFloat(ArrayWorkerFloat, float*, int);
// void addArrayWorkerFloat(ArrayServerFloat, float*, int, struct AddOption*);

#ifdef __cplusplus
}  // end extern "C"
#endif

#endif  // MULTIVERSO_C_API_H_
