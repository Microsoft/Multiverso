// Copyright 2015 Microsoft
#ifndef INCLUDE_MULTIVERSO_TABLE_SPARSE_MATRIX_TABLE_H_
#define INCLUDE_MULTIVERSO_TABLE_SPARSE_MATRIX_TABLE_H_

#include <vector>
#include <bitset>
#include "multiverso/multiverso.h"
#include "multiverso/table_interface.h"
#include "multiverso/util/log.h"
#include "multiverso/table/matrix_table.h"

namespace multiverso {

template <typename T>
class SparseMatrixWorkerTable : public MatrixWorkerTable<T> {
 public:
   SparseMatrixWorkerTable(int num_row, int num_col)
     : MatrixWorkerTable<T>(num_row, num_col) { }
    int Partition(const std::vector<Blob>& kv,
        std::unordered_map<int, std::vector<Blob>>* out) override;
    void ProcessReplyGet(std::vector<Blob>& reply_data) override;

    // get whole table, data is user-allocated memory
    void Get(T* data, size_t size,
      const GetOption* option = nullptr);

    // data is user-allocated memory
    void Get(int row_id, T* data, size_t size,
      const GetOption* option = nullptr);

    void Get(const std::vector<int>& row_ids,
        const std::vector<T*>& data_vec, size_t size,
        const GetOption* option = nullptr);

 private:
    // get whole table, data is user-allocated memory
    void Get(T* data, size_t size) = delete;

    // data is user-allocated memory
    void Get(int row_id, T* data, size_t size) = delete;

    void Get(const std::vector<int>& row_ids,
        const std::vector<T*>& data_vec, size_t size) = delete;
};

template <typename T>
class Updater;
template <typename T>
class SparseMatrixServerTable : public MatrixServerTable<T> {
 public:
     SparseMatrixServerTable(int num_row, int num_col, bool using_pipeline);
     ~SparseMatrixServerTable();
    void ProcessAdd(const std::vector<Blob>& data) override;
    void ProcessGet(const std::vector<Blob>& data,
        std::vector<Blob>* result) override;
 private:
     void UpdateAddState(int worker_id, Blob keys);
     void UpdateGetState(int worker_id, int* keys, size_t key_size,
       std::vector<int>* out_rows);
     int GetLogicalRow(int local_row_id) {
       return this->row_offset_ + local_row_id;
     }
     int GetPhysicalRow(int global_row_id) {
       return global_row_id - this->row_offset_;
     }
 private:
   bool** up_to_date_;
   int workers_nums_;
   // std::vector<std::vector<bool>> up_to_date_;
};

}   // namespace multiverso
#endif  // INCLUDE_MULTIVERSO_TABLE_SPARSE_MATRIX_TABLE_H_
